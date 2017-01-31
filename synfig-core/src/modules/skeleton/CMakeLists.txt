add_library(skeleton MODULE "")

target_sources(skeleton
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/skeleton.cpp"
)

target_include_directories(skeleton
    SYSTEM BEFORE PUBLIC
        ${LIB2GEOM_INCLUDE_DIRS}
)
target_link_libraries(skeleton ${LIB2GEOM_LIBRARIES})

install (
    TARGETS skeleton
    DESTINATION lib/synfig/modules
)
