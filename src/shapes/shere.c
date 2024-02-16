#include "../indexed_triangle_list.h"
#include "mat3.h"
#include "vec3.h"
#include "vector.h"
#include <stdlib.h>

static int calc_dist(int iLat, int iLong, int long_div) {
  return iLat * long_div + iLong;
}

indexed_triangle_list sphere_get_plain(float radius, int lat_div, int long_div,
                                       vertex_create_func vcf) {

  const vec3 base = {0.0f, 0.0f, radius};
  const float lat_angle = PI / (float)lat_div;
  const float long_ang = 2.0f * PI / (float)long_div;

  vector *vertices = vector_create(10);

  for (int iLat = 1; iLat < lat_div; iLat++) {
    mat3 lat_mat = mat3_rotationX(lat_angle * iLat);
    const vec3 lat_base = mat3_mult_vec3(&lat_mat, &base);

    for (int iLong = 0; iLong < long_div; iLong++) {

      mat3 long_mat = mat3_rotationZ(long_ang * iLong);

      vertex *new_vertex = malloc(sizeof(vertex));
      vec3 pos = mat3_mult_vec3(&long_mat, &lat_base);

      *new_vertex = vcf(pos, NULL);
      vector_push_back(vertices, new_vertex);
    }
  }

  const size_t i_north_pole = vector_get_size(vertices);
  vertex *new_vertex_n = malloc(sizeof(vertex));
  *new_vertex_n = vcf(base, NULL);
  vector_push_back(vertices, new_vertex_n);

  const size_t i_south_pole = vector_get_size(vertices);
  vertex *new_vertex_s = malloc(sizeof(vertex));

  vec3 base_neg = vec3_copy(&base);
  vec3_mult_s(&base_neg, -1.0f);
  *new_vertex_s = vcf(base_neg, NULL);
  vector_push_back(vertices, new_vertex_s);

  vector *indices = vector_create(10);

  for (int iLat = 0; iLat < lat_div - 2; iLat++) {
    for (int iLong = 0; iLong < long_div - 1; iLong++) {
      int *i1 = malloc(sizeof(int));
      int *i2 = malloc(sizeof(int));
      int *i3 = malloc(sizeof(int));
      int *i4 = malloc(sizeof(int));
      int *i5 = malloc(sizeof(int));
      int *i6 = malloc(sizeof(int));

      *i1 = calc_dist(iLat, iLong, long_div);
      *i2 = calc_dist(iLat + 1, iLong, long_div);
      *i3 = calc_dist(iLat, iLong + 1, long_div);
      *i4 = calc_dist(iLat, iLong + 1, long_div);
      *i5 = calc_dist(iLat + 1, iLong, long_div);
      *i6 = calc_dist(iLat + 1, iLong + 1, long_div);

      vector_push_back(indices, i1);
      vector_push_back(indices, i2);
      vector_push_back(indices, i3);
      vector_push_back(indices, i4);
      vector_push_back(indices, i5);
      vector_push_back(indices, i6);
    }
    int *i1 = malloc(sizeof(int));
    int *i2 = malloc(sizeof(int));
    int *i3 = malloc(sizeof(int));
    int *i4 = malloc(sizeof(int));
    int *i5 = malloc(sizeof(int));
    int *i6 = malloc(sizeof(int));

    *i1 = calc_dist(iLat, long_div - 1, long_div);
    *i2 = calc_dist(iLat + 1, long_div - 1, long_div);
    *i3 = calc_dist(iLat, 0, long_div);
    *i4 = calc_dist(iLat, 0, long_div);
    *i5 = calc_dist(iLat + 1, long_div - 1, long_div);
    *i6 = calc_dist(iLat + 1, 0, long_div);

    vector_push_back(indices, i1);
    vector_push_back(indices, i2);
    vector_push_back(indices, i3);
    vector_push_back(indices, i4);
    vector_push_back(indices, i5);
    vector_push_back(indices, i6);
  }

  for (int iLong = 0; iLong < long_div - 1; iLong++) {
    int *i1 = malloc(sizeof(int));
    int *i2 = malloc(sizeof(int));
    int *i3 = malloc(sizeof(int));
    int *i4 = malloc(sizeof(int));
    int *i5 = malloc(sizeof(int));
    int *i6 = malloc(sizeof(int));

    *i1 = i_north_pole;
    *i2 = calc_dist(0, iLong, long_div);
    *i3 = calc_dist(0, iLong + 1, long_div);

    *i4 = calc_dist(lat_div - 2, iLong + 1, long_div);
    *i5 = calc_dist(lat_div - 2, iLong, long_div);
    *i6 = i_south_pole;

    vector_push_back(indices, i1);
    vector_push_back(indices, i2);
    vector_push_back(indices, i3);
    vector_push_back(indices, i4);
    vector_push_back(indices, i5);
    vector_push_back(indices, i6);
  }

  int *i1 = malloc(sizeof(int));
  int *i2 = malloc(sizeof(int));
  int *i3 = malloc(sizeof(int));
  int *i4 = malloc(sizeof(int));
  int *i5 = malloc(sizeof(int));
  int *i6 = malloc(sizeof(int));

  *i1 = i_north_pole;
  *i2 = calc_dist(0, long_div - 1, long_div);
  *i3 = calc_dist(0, 0, long_div);

  *i4 = calc_dist(lat_div - 2, 0, long_div);
  *i5 = calc_dist(lat_div - 2, long_div - 1, long_div);
  *i6 = i_south_pole;

  vector_push_back(indices, i1);
  vector_push_back(indices, i2);
  vector_push_back(indices, i3);
  vector_push_back(indices, i4);
  vector_push_back(indices, i5);
  vector_push_back(indices, i6);

  return (indexed_triangle_list){.vertices = vertices, .indices = indices};
}
