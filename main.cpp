#include "blur.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  constexpr long vdivps = I_VEC * J_VEC + (I_VEC + 2) * J_VEC;
  constexpr long vaddps = 2 * vdivps;
  constexpr long flops = VEC_W * (vdivps + vaddps);

  constexpr long width = J_VEC * VEC_W;
  constexpr long lda = width + 2;
  constexpr long ldb = width;

  std::vector<data_t> a((I_VEC + 2) * (J_VEC * VEC_W + 2));
  std::vector<data_t> b(I_VEC * (J_VEC * VEC_W));

  std::random_device rd;
  std::mt19937 rng{rd()};
  std::uniform_real_distribution<data_t> dist{0.0, 1.0};

  std::generate(std::begin(a), std::end(a), [&]() { return dist(rng); });

  long nIter = 0;

  const auto start = std::chrono::steady_clock::now();
  auto end = std::chrono::steady_clock::now();
  double duration;

  do {
    for (int i = 0; i < 1000; i++) {
      blur_tile(a.data(), lda, b.data(), ldb);
    }
    nIter += 1000;
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration<double>(end - start).count();
  } while (duration < 0.5);

  printf("%8s I=%-2d J=%-2d Ran %8ld iterations in %.2lf seconds. Speed was "
         "%.2lf GFLOPs\n",
         UNROLLING ? "UNROLL" : "AUTO", I_VEC, J_VEC, nIter, duration,
         (double)flops / duration * (double)nIter * 1e-9);
}
