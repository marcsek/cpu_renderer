#include "indexed_triangle_list.h"
#include "../vendors/tinyobj/tinyobj_loader_c.h"
#include "essentials.h"
#include "helpers/debug_info.h"
#include "vector.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1KB
#define MAX_LINE_LENGTH 1000
// 10MB
#define MAX_BUFFER_LENGTH 10000000

static void load_file(void *ctx, const char *filename, int is_mtl,
                      const char *obj_filename, char **buf, size_t *len) {
  UNUSED(is_mtl);
  UNUSED(obj_filename);

  FILE *fptr;

  if ((fptr = fopen(filename, "r")) == NULL) {
    printf("Couldn\'t open obj file. (%s)\n", filename);
  }

  char buff[MAX_BUFFER_LENGTH];
  char first_line[MAX_LINE_LENGTH];

  fscanf(fptr, "%[^\n]", first_line);

  size_t new_len = fread(buff, sizeof(char), MAX_BUFFER_LENGTH, fptr);

  for (int i = 0; first_line[i]; i++) {
    first_line[i] = tolower(first_line[i]);
  }

  if (strnstr(first_line, "ccw", MAX_LINE_LENGTH) != NULL) {
    *(bool *)ctx = true;
  }

  debug_printf("loaded file \"%s\"; size: %lu bytes", filename, new_len);

  *buf = buff;
  *len = new_len;

  fclose(fptr);
}

indexed_triangle_list itd_load(const char *file_name, vertex_create_func vcf) {
  indexed_triangle_list tl;

  bool isCCW = false;

  tinyobj_attrib_t attrib;
  tinyobj_shape_t *shapes = NULL;
  size_t num_shapes;
  tinyobj_material_t *materials = NULL;
  size_t num_materials;

  unsigned int flags = 0;
  const int ret =
      tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                        &num_materials, file_name, load_file, &isCCW, flags);

  if (ret != TINYOBJ_SUCCESS) {
    debug_printf("Couldn\'t parse obj file. (%s)", file_name);
  }

  tl.vertices = vector_create(attrib.num_vertices);

  for (int i = 0; (unsigned int)i < attrib.num_vertices * 3; i += 3) {
    vertex *new_vertex = malloc(sizeof(vertex));

    *new_vertex = vcf((vec3){attrib.vertices[i], attrib.vertices[i + 1],
                             attrib.vertices[i + 2]},
                      NULL);
    vector_push_back(tl.vertices, new_vertex);
  }

  tl.indices = vector_create(attrib.num_faces);

  for (int i = 0; (unsigned int)i < attrib.num_faces; i++) {
    int *v = malloc(sizeof(int));
    *v = attrib.faces[i].v_idx;
    vector_push_back(tl.indices, v);
  }

  tinyobj_attrib_free(&attrib);
  tinyobj_shapes_free(shapes, num_shapes);
  tinyobj_materials_free(materials, num_materials);

  return tl;
}
