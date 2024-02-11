#include "pipeline.h"
#include "app.h"
#include "mat3.h"
#include "screen_transformer.h"
#include "shaders/vertex.h"
#include "triangle_rasterizer.h"
#include "vec3.h"
#include "vector.h"
#include "z_buffer.h"
#include <assert.h>
#include <stdlib.h>

static void process_vertices(pipeline *p, const vector *verts, const int *inds,
                             size_t i_len);
static void assemble_triangles(pipeline *p, const vector *verts,
                               const int *inds, size_t i_len);
static void process_triangles(pipeline *p, const vertex *v0, const vertex *v1,
                              const vertex *v2);
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
  // TODO: indexed_triangle_list_tex is not it
  process_vertices(p, tri_list->vertices, tri_list->indices, 36);
}

void pipeline_bind_rotation(pipeline *p, const mat3 *rotation_in) {
  p->rotation = *rotation_in;
}

void pipeline_bind_translation(pipeline *p, const vec3 *translation_in) {
  p->translation = *translation_in;
}

static void process_vertices(pipeline *p, const vector *verts, const int *inds,
                             size_t i_len) {
  size_t verts_size = vector_get_size(verts);
  vector *verts_out = vector_create_cf(verts_size, verts_cf);
  vertex **data = (vertex **)vector_get_data(verts);

  assert(verts_size != 0);

  for (size_t i = 0; i < verts_size; i++) {
    vec3 pos_rot = mat3_mult_vec3(&p->rotation, &data[i]->pos);
    vec3_add(&pos_rot, &p->translation);

    vertex *new_vertex = malloc(sizeof(vertex));
    *new_vertex =
        (vertex){.pos = pos_rot, .sd = data[i]->sd, .fn = data[i]->fn};
    *new_vertex = vertex_copy(new_vertex);

    vector_push_back(verts_out, new_vertex);
  }

  assemble_triangles(p, verts_out, inds, i_len);
  vector_destroy(verts_out);
}

static void assemble_triangles(pipeline *p, const vector *verts,
                               const int *inds, size_t i_len) {
  vertex **data = (vertex **)vector_get_data(verts);
  for (size_t i = 0; i < i_len; i += 3) {
    vertex *v1 = data[inds[i]];
    vertex *v2 = data[inds[i + 1]];
    vertex *v3 = data[inds[i + 2]];

    vec3 v2_pos = v2->pos;
    vec3 v3_pos = v3->pos;

    vec3_subs(&v2_pos, &v1->pos);
    vec3_subs(&v3_pos, &v1->pos);
    vec3 cross = vec3_cross_prod(&v2_pos, &v3_pos);

    bool should_cull = vec3_dot_prod(&cross, &v1->pos) > 0.0f;

    if (!should_cull) {
      process_triangles(p, v1, v2, v3);
    }
  }
}

static void process_triangles(pipeline *p, const vertex *v0, const vertex *v1,
                              const vertex *v2) {
  vertex v0_c = vertex_copy(v0);
  vertex v1_c = vertex_copy(v1);
  vertex v2_c = vertex_copy(v2);
  post_process_tverts(p, (triangle){v0_c, v1_c, v2_c});
  vertex_free(&v0_c);
  vertex_free(&v1_c);
  vertex_free(&v2_c);
}

static void post_process_tverts(pipeline *p, triangle tr) {
  st_transform(&p->st, &tr.v0);
  st_transform(&p->st, &tr.v1);
  st_transform(&p->st, &tr.v2);

  triangle_rasterizer_draw(p->rn, tr, &p->tex, p);
}