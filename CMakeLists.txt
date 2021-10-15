cmake_minimum_required(VERSION 3.21)
project(blur_demo CXX)

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(FATAL_ERROR "This program uses GCC-specific extensions")
endif ()

foreach (I_VEC RANGE 1 32)
  foreach (J_VEC RANGE 1 32)
    math(EXPR vecs "${I_VEC} * ${J_VEC}")
    if (vecs GREATER 32)
      continue()
    endif()

    set(target "blur_${I_VEC}_${J_VEC}")

    add_executable(${target} blur.cpp main.cpp)
    target_compile_options(${target} PRIVATE -Wall -pedantic -march=native)
    target_compile_features(${target} PRIVATE cxx_std_17)
    target_compile_definitions(
      ${target}
      PRIVATE
        I_VEC=${I_VEC}
        J_VEC=${J_VEC}
    )
  endforeach ()
endforeach ()
