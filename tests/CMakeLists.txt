include(GoogleTest)

# Add tests executable
add_executable(tests
        CopyOnWritePtrTest.cpp
)

target_link_libraries(tests
        PRIVATE
        CopyOnWritePtr
        GTest::GTest
        GTest::Main
)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(tests PRIVATE -fno-omit-frame-pointer -fsanitize=address,undefined)
    target_link_options(tests PRIVATE -fno-omit-frame-pointer -fsanitize=address,undefined)
endif()
add_test(NAME base-tests COMMAND tests)
set_tests_properties(base-tests PROPERTIES TIMEOUT 10)
