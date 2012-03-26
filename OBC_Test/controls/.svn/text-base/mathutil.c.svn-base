/**
 *@file  mathutil.c
 *@brief Matrix/Vector manipulations
*/
#include "common.h"
#include "mathutil.h"

void copy_vector(vector v_src, vector v_dest)
{
  uint8_t i;
  for(i = 0; i < 3; i++)
    v_dest[i] = v_src[i];
}

void copy_quaternion(quaternion q_src, quaternion q_dest)
{
  uint8_t i;
  for(i = 0; i < 4; i++)
    q_dest[i] = q_src[i];
}


float vector_norm(vector v)
{
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float quaternion_norm(quaternion q)
{
  return sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
}

float vector_dot_product(vector v_a, vector v_b)
{
  return v_a[0] * v_b[0] + v_a[1] * v_b[1] + v_a[2] * v_b[2];
}

void add_vectors(vector v_a, vector v_b, vector v_res)
{
  uint8_t i;
  for(i = 0; i < 3; i++)
    v_res[i] = v_a[i] + v_b[i];
}

void vector_into_matrix(vector v, matrix m, vector v_res)
{
  uint8_t i, j;
  
  for(i = 0; i < 3; i++)
  {
    v_res[i] = 0;
    for(j = 0; j < 3; j++)
      v_res[i] += v[j] * m[j][i];
  }
}

void vector_cross_product(vector v_a, vector v_b, vector v_res)
{
  v_res[0] = v_a[1] * v_b[2] - v_a[2] * v_b[1];
	v_res[1] = v_a[2] * v_b[0] - v_a[0] * v_b[2];
	v_res[2] = v_a[0] * v_b[1] - v_a[1] * v_b[0];
}

void scalar_into_vector(vector v, float s)
{
  uint8_t i;
  for(i = 0; i < 3; i++)
    v[i] = v[i] * s;
}

void scalar_into_quaternion(quaternion q, float s)
{
  uint8_t i;
  for(i = 0; i < 4; i++)
    q[i] = q[i] * s;
}

void convert_unit_vector(vector v)
{
  uint8_t i;
  float norm;
  
  norm = vector_norm(v);
  
  for(i = 0; i < 3; i++)
    v[i] = v[i] / norm;
}

void convert_unit_quaternion(quaternion q)
{
  uint8_t i;
  float norm;
  
  norm = quaternion_norm(q);
  
  for(i = 0; i < 4; i++)
    q[i] = q[i] / norm;
}
