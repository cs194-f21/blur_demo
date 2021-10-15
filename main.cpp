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
  constexpr float peak_gflops = 229.899; // Hard-coded for i9-7900X @ 4.3 GHz

  constexpr long muls = I_VEC * J_VEC + (I_VEC + 2) * J_VEC;
  constexpr long adds = 2 * muls;
  constexpr long flops = VEC_W * (adds + muls);

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
  } while (duration < 3.0);

  double gflops = (double)flops / duration * (double)nIter * 1e-9;

  printf("I=%-2d J=%-2d Ran %9ld iterations in %.2lf seconds. Speed was "
         "%.2lf GFLOPs (%.2lf%% of peak)\n",
         I_VEC, J_VEC, nIter, duration, gflops, gflops / peak_gflops * 100);
}
