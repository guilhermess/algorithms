#add_library(trees SHARED
#        bst_test.cpp)

#target_link_libraries(ps1 PUBLIC ${OpenCV_LIBS})

add_executable(trees_test bst_test.cpp avl/avl_tree_test.cpp)
target_link_libraries(trees_test PUBLIC gtest_main)

add_test(NAME trees_test COMMAND trees_test)
