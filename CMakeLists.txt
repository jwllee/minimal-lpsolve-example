cmake_minimum_required(VERSION 3.8...3.15)

project(minimal-lpsolve-example)

set(LPSOLVE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/lpsolve")
set(LPSOLVE_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/extern/lpsolve/liblpsolve55.so")

# example crop problem
add_executable(crop_example src/crop_example.cpp)
target_include_directories(crop_example PUBLIC "${LPSOLVE_DIR}")
# -L${LPSOLVE_DIR} tells the compiler where the library is at
# rpath allows usage of lpsolve55 instead of having to add the library path in LD_LIBRARY_PATH
target_link_libraries(crop_example PUBLIC -L"${LPSOLVE_DIR}" -Wl,-rpath="${LPSOLVE_DIR}" lpsolve55)
