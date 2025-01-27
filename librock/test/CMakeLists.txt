


if(BUILD_TEST)
    if(BUILD_3T)
        add_subdirectory(3t)
    endif()
    if(BUILD_UT)
        add_subdirectory(ut)
    endif()
    if(BUILD_IT)
        add_subdirectory(it)
    endif()
endif()