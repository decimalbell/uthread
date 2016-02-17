add_executable(resume_test resume_test.c)
target_link_libraries(resume_test uthread)

add_executable(yield_test yield_test.c)
target_link_libraries(yield_test uthread)
