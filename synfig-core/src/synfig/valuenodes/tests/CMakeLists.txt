add_executable(valuenode_add valuenode_add.cpp)
target_link_libraries(valuenode_add Catch)
target_link_libraries(valuenode_add synfig)
add_test(test_valuenode_add valuenode_add)

add_executable(timecurve timecurve.cpp)
target_link_libraries(timecurve Catch)
target_link_libraries(timecurve synfig)
add_test(test_timecurve timecurve)
