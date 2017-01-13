add_library(timecurve MODULE "")

target_sources(timecurve
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/timecurve.cpp"
)

target_include_directories(timecurve
    SYSTEM BEFORE PUBLIC
        ${LIB2GEOM_INCLUDE_DIRS}
)
target_link_libraries(timecurve ${LIB2GEOM_LIBRARIES})

install (
    TARGETS timecurve
    DESTINATION lib/synfig/modules
)
