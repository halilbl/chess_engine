cmake_minimum_required(VERSION 3.16)
project(ChessEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.4
)
FetchContent_MakeAvailable(Catch2)

# engine code shared by the executable and the tests
add_library(chess_core STATIC
    src/board.cpp
    src/move_generator.cpp
    src/bitboard_utils.cpp
  )
target_include_directories(chess_core PUBLIC src)

add_executable(chess_engine src/main.cpp)
target_link_libraries(chess_engine PRIVATE chess_core)

add_executable(chess_tests tests/test_main.cpp)
target_link_libraries(chess_tests PRIVATE chess_core Catch2::Catch2WithMain)

enable_testing()
add_test(NAME ChessTests COMMAND chess_tests)