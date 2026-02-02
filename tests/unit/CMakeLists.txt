cmake_minimum_required(VERSION 3.10)
project(ledger-app-tron-unit-tests C)

find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(CMOCKA cmocka)
endif()

if(NOT CMOCKA_FOUND)
    find_library(CMOCKA_LIBRARIES cmocka)
    find_path(CMOCKA_INCLUDE_DIRS cmocka.h)
    if(CMOCKA_LIBRARIES AND CMOCKA_INCLUDE_DIRS)
        set(CMOCKA_FOUND TRUE)
        set(CMOCKA_CFLAGS_OTHER "")
        set(CMOCKA_LIBRARY_DIRS "")
    endif()
endif()

if(NOT CMOCKA_FOUND)
    message(FATAL_ERROR "CMocka not found. Install libcmocka-dev or cmocka")
endif()

enable_testing()
include(CTest)

include_directories(../../src)
include_directories(../../proto)
include_directories(../../nanopb)
include_directories(./mock)
include_directories(.)
add_definitions(-DNO_BOLOS_SDK=1)

add_library(uint256_lib SHARED ../../src/uint256.c)
target_compile_definitions(uint256_lib PRIVATE NO_BOLOS_SDK=1)

add_executable(test_uint256 test_uint256.c)
target_link_libraries(test_uint256 ${CMOCKA_LIBRARIES} uint256_lib)
target_include_directories(test_uint256 PUBLIC ${CMOCKA_INCLUDE_DIRS})
target_compile_options(test_uint256 PUBLIC ${CMOCKA_CFLAGS_OTHER})
target_link_directories(test_uint256 PUBLIC ${CMOCKA_LIBRARY_DIRS})
add_test(test_uint256 ${CMAKE_CURRENT_BINARY_DIR}/test_uint256)

add_executable(test_parse_utils
    test_parse_utils.c
    ../../src/helpers.c
    ../../src/parse.c
    mock/ledger_stubs.c
    ../../nanopb/pb_common.c
    ../../nanopb/pb_decode.c
    ../../proto/misc/TronApp.pb.c
    ../../proto/core/Tron.pb.c
    ../../proto/core/Contract.pb.c
    ../../proto/google/protobuf/any.pb.c
)
target_link_libraries(test_parse_utils ${CMOCKA_LIBRARIES})
target_include_directories(test_parse_utils PUBLIC ${CMOCKA_INCLUDE_DIRS})
target_compile_options(test_parse_utils PUBLIC ${CMOCKA_CFLAGS_OTHER})
target_link_directories(test_parse_utils PUBLIC ${CMOCKA_LIBRARY_DIRS})
add_test(test_parse_utils ${CMAKE_CURRENT_BINARY_DIR}/test_parse_utils)