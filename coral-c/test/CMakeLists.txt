# SPDX-License-Identifier: Apache-2.0

FOREACH(SVARIANT ${SVARIANT_S})

    string(TOLOWER ${SVARIANT} SVARIANT_LOWER)
    string(TOUPPER ${SVARIANT} SVARIANT_UPPER)

    add_executable(nike_test_scheme_${SVARIANT} test_nike.c)
    target_link_libraries(nike_test_scheme_${SVARIANT} nike_${SVARIANT_LOWER})
    target_include_directories(nike_test_scheme_${SVARIANT} PUBLIC ${PROJECT_SOURCE_DIR}/src/common ${INC_PUBLIC} ${PROJECT_SOURCE_DIR}/src/NIKE/${SVARIANT_LOWER})
    add_test(nike_${SVARIANT}_SELFTEST nike_test_scheme_${SVARIANT})


ENDFOREACH()
