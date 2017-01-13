# TODO: move this somewhere else
function(add_synfig_catch_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} Catch synfig)
    add_test("test_${name}" ${name})
endfunction(add_synfig_catch_test)

add_synfig_catch_test(valuenode_add)
