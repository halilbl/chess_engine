# CMake generated Testfile for 
# Source directory: C:/Users/blebl/Desktop/repos/chess_engine
# Build directory: C:/Users/blebl/Desktop/repos/chess_engine/out/build/x64-Debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ChessTests "C:/Users/blebl/Desktop/repos/chess_engine/out/build/x64-Debug/chess_tests.exe")
set_tests_properties(ChessTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/blebl/Desktop/repos/chess_engine/CMakeLists.txt;22;add_test;C:/Users/blebl/Desktop/repos/chess_engine/CMakeLists.txt;0;")
subdirs("_deps/catch2-build")
