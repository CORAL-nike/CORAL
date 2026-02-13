set(SOURCE_FILES_QUATERNION_GENERIC_REF
    intbig.c
    integers.c
    finit.c
    mini-gmp.c
    randomized.c
)

if (NOT GMP_LIBRARY STREQUAL "MINI")
    list(APPEND SOURCE_FILES_QUATERNION_GENERIC_REF ${PROJECT_SOURCE_DIR}/src/mini-gmp/mini-gmp-extra.c)
endif()

set(SOURCE_FILES_QUATERNION_GENERIC_REF_TESTS
    ${SOURCE_FILES_QUATERNION_GENERIC_REF}
)
add_executable(sqisign_test_quaternion ${SOURCE_FILES_QUATERNION_GENERIC_REF_TESTS} test_quaternions.c)
target_link_libraries(sqisign_test_quaternion ${LIB_QUATERNION} ${GMP} sqisign_common_sys)
target_include_directories(sqisign_test_quaternion PRIVATE ../internal_quaternion_headers  ${INC_COMMON} ${INC_QUATERNION} ${INC_PUBLIC} ${PROJECT_SOURCE_DIR}/src/mini-gmp)

# MSAN and GMP lead to false positives, see
# https://gmplib.org/list-archives/gmp-bugs/2019-March/004529.html
if(NOT CMAKE_BUILD_TYPE STREQUAL "MSAN")
    add_test(sqisign_test_quaternion sqisign_test_quaternion)
endif()

