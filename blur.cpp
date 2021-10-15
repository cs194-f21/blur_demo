#include "blur.h"

void blur_tile(data_t *a, long lda, data_t *b, long ldb) {
  // a : (I_VEC + 2) x (J_VEC * VEC_W + 2)
  // b : I_VEC x (J_VEC * VEC_W)

  // Now we have an intermediate array of _vectors_
  vec_t h_blur[I_VEC + 2][J_VEC];
#if UNROLLING
#pragma GCC unroll 32
#endif
  for (long i = 0; i < I_VEC + 2; i++) {
    float *a_base = &a[i * lda];
#if UNROLLING
#pragma GCC unroll 32
#endif
    for (long jo = 0; jo < J_VEC; jo++) {
      // Load three horizontally overlapping vectors
      vec_t a_left = *(vec_t *)(&a_base[jo * VEC_W]);
      vec_t a_mid = *(vec_t *)(&a_base[jo * VEC_W + 1]);
      vec_t a_right = *(vec_t *)(&a_base[jo * VEC_W + 2]);

      // Average them, store into intermediate vectors
      h_blur[i][jo] = (a_left + a_mid + a_right) / 3.0;
    }
  }
#if UNROLLING
#pragma GCC unroll 32
#endif
  for (long i = 0; i < I_VEC; i++) {
    float *b_base = &b[i * ldb];
#if UNROLLING
#pragma GCC unroll 32
#endif
    for (long jo = 0; jo < J_VEC; jo++) {
      // Average 3x1 window of vectors, write back out to memory
      *(vec_t *)(&b_base[jo * VEC_W]) =
          (h_blur[i][jo] + h_blur[i + 1][jo] + h_blur[i + 2][jo]) / 3.0;
    }
  }
}
