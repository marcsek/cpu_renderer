#include "pipeline.h"
#include "app.h"
#include "helpers/debug_info.h"
#include "screen_transformer.h"
#include "shaders/vertex.h"
#include "triangle_rasterizer.h"
#include "vec3.h"
#include "vector.h"
#include "z_buffer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void process_vertices(pipeline *p, const vector *verts,
                             const vector *inds);
static void assemble_triangles(pipeline *p, const vector *verts,
                               const vector *inds);
static void process_triangles(pipeline *p, const vertex *v0, const vertex *v1,
                              const vertex *v2, unsigned int index);
static void post_process_tverts(pipeline *p, triangle tr);

void verts_cf(void *to_free) {
  vertex *to_free_v = (vertex *)to_free;
  vertex_free(to_free_v);
  free(to_free_v);
}

pipeline pipeline_create(renderer *rn, effect ef) {
  return (pipeline){.rn = rn,
                    .st = st_create(WINDOW_WIDTH, WINDOW_HEIGHT),
                    .effect = ef,
                    .zb = z_buffer_create(WINDOW_WIDTH, WINDOW_HEIGHT)};
}

void pipeline_begin_frame(pipeline *p) { z_buffer_clear(&p->zb); }

void pipeline_draw(pipeline *p, indexed_triangle_list *tri_list) {
  process_vertices(p, tri_list->vertices, tri_list->indices);
}

static void process_vertices(pipeline *p, const vector *verts,
                             const vector *inds) {
  size_t verts_size = vector_get_size(verts);
  vector *verts_out = vector_create_cf(verts_size, verts_cf);
  vertex **data = (vertex **)vector_get_data(verts);

  assert(verts_size != 0);

  for (size_t i = 0; i < verts_size; i++) {
    vertex *new_vertex = malloc(sizeof(vertex));
    *new_vertex =
        (vertex){.pos = data[i]->pos, .sd = data[i]->sd, .fn = data[i]->fn};

    *new_vertex = p->effect.vs.transform(p->effect.vs.shader_data, new_vertex);
    vector_push_back(verts_out, new_vertex);
  }

  assemble_triangles(p, verts_out, inds);
  vector_destroy(verts_out);
}

static void assemble_triangles(pipeline *p, const vector *verts,
                               const vector *inds) {
  vertex **data = (vertex **)vector_get_data(verts);
  int **ind_d = (int **)vector_get_data(inds);

  for (size_t i = 0; i < vector_get_size(inds); i += 3) {
    vertex *v1 = data[*ind_d[i]];
    vertex *v2 = data[*ind_d[i + 1]];
    vertex *v3 = data[*ind_d[i + 2]];

    vec3 v2_pos = v2->pos;
    vec3 v3_pos = v3->pos;

    vec3_subs(&v2_pos, &v1->pos);
    vec3_subs(&v3_pos, &v1->pos);
    vec3 cross = vec3_cross_prod(&v2_pos, &v3_pos);

    bool should_cull = vec3_dot_prod(&cross, &v1->pos) > 0.0f;

    if (!should_cull) {
      process_triangles(p, v1, v2, v3, i / 3);
    }
  }
}

static void process_triangles(pipeline *p, const vertex *v0, const vertex *v1,
                              const vertex *v2, unsigned int index) {
  triangle trian =
      p->effect.gs.process(p->effect.gs.shader_data, v0, v1, v2, index);

  post_process_tverts(p, trian);

  vertex_free(&trian.v0);
  vertex_free(&trian.v1);
  vertex_free(&trian.v2);
}

static void post_process_tverts(pipeline *p, triangle tr) {
  st_transform(&p->st, &tr.v0);
  st_transform(&p->st, &tr.v1);
  st_transform(&p->st, &tr.v2);

  triangle_rasterizer_draw(p->rn, tr, p);
}
