set(SOURCE_FILES_QUATERNION_GENERIC_REF
    intbig.c
    integers.c
    finit.c
    printer.c
)

add_library(${LIB_QUATERNION} STATIC ${SOURCE_FILES_QUATERNION_GENERIC_REF})
target_link_libraries(${LIB_QUATERNION} GMP m sqisign_common_sys)
target_include_directories(${LIB_QUATERNION} PRIVATE common ${INC_PUBLIC} ${INC_COMMON} ${INC_QUATERNION} internal_quaternion_headers)
target_compile_options(${LIB_QUATERNION} PRIVATE ${C_OPT_FLAGS})

add_subdirectory(test)
