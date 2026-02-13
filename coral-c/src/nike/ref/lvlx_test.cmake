# Testing {{{{{
# NIKE
add_executable(test_nike_${SVARIANT_LOWER} ${LVLX_DIR}/test/test_nike.c)
target_link_libraries(test_nike_${SVARIANT_LOWER} ${LIB_SIGNATURE_${SVARIANT_UPPER}} sqisign_common_test)
target_include_directories(test_nike_${SVARIANT_LOWER} PRIVATE ${INC_PUBLIC} ${INC_COMMON} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_EC} ${INC_HD}  ${INC_NIKE})

add_test(test_nike_${SVARIANT_LOWER} test_nike_${SVARIANT_LOWER} 3)

add_custom_command(
  TARGET test_nike_${SVARIANT_LOWER}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy $<TARGET_FILE:test_nike_${SVARIANT_LOWER}> ${CMAKE_BINARY_DIR}/test/nike_${SVARIANT_LOWER}
)

# NIKE Tools
add_executable(test_nike_tools_${SVARIANT_LOWER} ${LVLX_DIR}/test/test_nike_tools.c)
target_link_libraries(test_nike_tools_${SVARIANT_LOWER} ${LIB_NIKE_${SVARIANT_UPPER}} ${LIB_QUATERNION} sqisign_common_test)
target_include_directories(test_nike_tools_${SVARIANT_LOWER} PRIVATE ${INC_PUBLIC} ${INC_COMMON} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_EC} ${INC_HD}  ${INC_NIKE})

add_test(test_nike_tools_${SVARIANT_LOWER} test_nike_tools_${SVARIANT_LOWER} 3)

add_custom_command(
  TARGET test_nike_tools_${SVARIANT_LOWER}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy $<TARGET_FILE:test_nike_tools_${SVARIANT_LOWER}> ${CMAKE_BINARY_DIR}/test/nike_${SVARIANT_LOWER}
)

# Test action
add_executable(test_nike_action_${SVARIANT_LOWER} ${LVLX_DIR}/test/test_nike_action.c)
target_link_libraries(test_nike_action_${SVARIANT_LOWER} ${LIB_NIKE_${SVARIANT_UPPER}} ${LIB_QUATERNION} sqisign_common_test)
target_include_directories(test_nike_action_${SVARIANT_LOWER} PRIVATE ${INC_PUBLIC} ${INC_COMMON} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_EC} ${INC_HD} ${INC_NIKE})

add_test(test_nike_action_${SVARIANT_LOWER} test_nike_action_${SVARIANT_LOWER} 3)

add_custom_command(
  TARGET test_nike_action_${SVARIANT_LOWER}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy $<TARGET_FILE:test_nike_action_${SVARIANT_LOWER}> ${CMAKE_BINARY_DIR}/test/nike_${SVARIANT_LOWER}
)
# }}}}}
# Benchmarking {{{{{
# NIKE
add_executable(bench_nike_${SVARIANT_LOWER} ${LVLX_DIR}/test/bench_nike.c)
target_link_libraries(bench_nike_${SVARIANT_LOWER} ${LIB_NIKE_${SVARIANT_UPPER}} sqisign_common_sys)
target_include_directories(bench_nike_${SVARIANT_LOWER} PRIVATE ${INC_PUBLIC} ${INC_COMMON} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_EC} ${INC_HD}  ${INC_NIKE})

set(BM_BINS ${BM_BINS} bench_nike_${SVARIANT_LOWER} CACHE INTERNAL "List of benchmark executables")

# Norm equation
add_executable(bench_normeq_${SVARIANT_LOWER} ${LVLX_DIR}/test/bench_normeq.c)
target_link_libraries(bench_normeq_${SVARIANT_LOWER} ${LIB_NIKE_${SVARIANT_UPPER}} sqisign_common_sys)
target_include_directories(bench_normeq_${SVARIANT_LOWER} PRIVATE ${INC_PUBLIC} ${INC_COMMON} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_EC} ${INC_HD}  ${INC_NIKE})

set(BM_BINS ${BM_BINS} bench_normeq_${SVARIANT_LOWER} CACHE INTERNAL "List of benchmark executables")
# }}}}}
