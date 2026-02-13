add_subdirectory(common)

add_subdirectory(quaternion)


add_subdirectory(mp)
add_subdirectory(gf)
add_subdirectory(precomp)
add_subdirectory(ec)
add_subdirectory(hd)
add_subdirectory(nike)

FOREACH(SVARIANT ${SVARIANT_S})
    string(TOLOWER ${SVARIANT} SVARIANT_LOWER)
    string(TOUPPER ${SVARIANT} SVARIANT_UPPER)
    set(SOURCE_FILES_VARIANT nike.c)

    # Library for SQIsign variant
    add_library(nike_${SVARIANT_LOWER} ${SOURCE_FILES_VARIANT})
    target_link_libraries(nike_${SVARIANT_LOWER} PUBLIC
        ${LIB_NIKE_${SVARIANT_UPPER}}
        ${LIB_QUATERNION}
        ${LIB_MP}
        ${LIB_GF_${SVARIANT_UPPER}}
        ${LIB_EC_${SVARIANT_UPPER}}
        ${LIB_HD_${SVARIANT_UPPER}}
        ${LIB_PRECOMP_${SVARIANT_UPPER}}
        GMP
        sqisign_common_sys
    )

    target_include_directories(nike_${SVARIANT_LOWER} PUBLIC ${INC_NIKE} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_MP} ${INC_EC} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_COMMON} ${INC_HD} ../include PRIVATE common/generic internal)
    target_compile_definitions(nike_${SVARIANT_LOWER} PUBLIC SQISIGN_VARIANT=${SVARIANT})

    # Library for SQIsign variant (test)
    add_library(nike_${SVARIANT_LOWER}_test ${SOURCE_FILES_VARIANT})
    target_link_libraries(nike_${SVARIANT_LOWER}_test PUBLIC
        ${LIB_NIKE_${SVARIANT_UPPER}}
        ${LIB_QUATERNION}
        ${LIB_MP}
        ${LIB_GF_${SVARIANT_UPPER}}
        ${LIB_EC_${SVARIANT_UPPER}}
        ${LIB_HD_${SVARIANT_UPPER}}
        ${LIB_PRECOMP_${SVARIANT_UPPER}}
        GMP
        sqisign_common_test
    )

    target_include_directories(nike_${SVARIANT_LOWER}_test PUBLIC ${INC_NIKE} ${INC_QUATERNION} ${INC_PRECOMP_${SVARIANT_UPPER}} ${INC_MP} ${INC_EC} ${INC_GF} ${INC_GF_${SVARIANT_UPPER}} ${INC_COMMON} ${INC_HD} ../include PRIVATE common/generic internal)
    target_compile_definitions(nike_${SVARIANT_LOWER}_test PUBLIC SQISIGN_VARIANT=${SVARIANT})

ENDFOREACH()
