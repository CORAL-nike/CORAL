add_executable(curve-arith.test_${SVARIANT_LOWER} ${LVLX_DIR}/test/curve-arith-test.c ${LVLX_DIR}/test/test_extras.c)
target_include_directories(curve-arith.test_${SVARIANT_LOWER} PUBLIC ${INC_COMMON} ${INC_MP} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_PUBLIC} ../include ${INC_EC} .)
target_link_libraries(curve-arith.test_${SVARIANT_LOWER} ${LIB_EC_${SVARIANT_UPPER}} sqisign_common_test)

add_executable(curve-arith.bench_${SVARIANT_LOWER} ${LVLX_DIR}/test/curve-arith-bench.c ${LVLX_DIR}/test/test_extras.c)
target_include_directories(curve-arith.bench_${SVARIANT_LOWER} PUBLIC ${INC_COMMON} ${INC_MP} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_PUBLIC} ../include ${INC_EC} .)
target_link_libraries(curve-arith.bench_${SVARIANT_LOWER} ${LIB_EC_${SVARIANT_UPPER}} sqisign_common_sys)

set(BM_BINS ${BM_BINS}
    curve-arith.bench_${SVARIANT_LOWER}
    CACHE INTERNAL "List of benchmark executables")
