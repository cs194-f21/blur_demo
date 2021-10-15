#ifndef BLUR_H
#define BLUR_H

// How wide are our vectors in bits?
static constexpr long VEC_BITS = 512;

// We're going to use floats here, but it's worth trying this
// experiment for other data types (like double), too.
typedef float data_t;

// How many elements can our vector store?
static constexpr long VEC_W = VEC_BITS / (8 * sizeof(data_t));

// Create a GCC-specific vector type. It takes a size in bytes
// and an alignment, which is 1 here (i.e. unaligned).
typedef data_t vec_t
    __attribute__((vector_size(VEC_W * sizeof(data_t)), aligned(1)));

// Try a variety of vector block sizes
#if !defined(I_VEC) && !defined(J_VEC)
static constexpr long I_VEC = 7;
static constexpr long J_VEC = 2;
#endif

void blur_tile(data_t *a, long lda, data_t *b, long ldb);

#endif
