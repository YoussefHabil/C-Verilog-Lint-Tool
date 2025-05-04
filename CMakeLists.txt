set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)
project(VerilogLint)

set(CMAKE_CXX_STANDARD 17)
include_directories(check_violations)

# Main executable
add_executable(verilog_lint
    check_violations/VerilogLint.cpp
    check_violations/arithmetic_overflow_violation.h
    check_violations/uninitialized_register_violation.h
    check_violations/infer_latch_violation.h
    check_violations/case_violations.h
    check_violations/multidriven_register_violation.h
    check_violations/GlobalFunctions.h
    check_violations/Report.h
)

find_package(GTest REQUIRED)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})

# Tests
add_executable(OverflowTest check_violations/tests/ArithmeticOverflowTest.cpp)
target_link_libraries(OverflowTest GTest::GTest GTest::Main pthread)

add_executable(UninitTest check_violations/tests/UninitializedRegTest.cpp)
target_link_libraries(UninitTest GTest::GTest GTest::Main pthread)

add_executable(LatchTest check_violations/tests/InferLatchTest.cpp)
target_link_libraries(LatchTest GTest::GTest GTest::Main pthread)

add_executable(CaseTest check_violations/tests/CaseViolationsTest.cpp)
target_link_libraries(CaseTest GTest::GTest GTest::Main pthread)

add_executable(MultiTest check_violations/tests/MultiDrivenTest.cpp)
target_link_libraries(MultiTest GTest::GTest GTest::Main pthread)

add_test(NAME OverflowTest COMMAND OverflowTest)
add_test(NAME UninitTest   COMMAND UninitTest)
add_test(NAME LatchTest    COMMAND LatchTest)
add_test(NAME CaseTest     COMMAND CaseTest)
add_test(NAME MultiTest    COMMAND MultiTest)
