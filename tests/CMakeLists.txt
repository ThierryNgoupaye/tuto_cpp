add_executable(mes_tests
    test_calculatrice.cpp
    test_camera.cpp
)


target_link_libraries(mes_tests PRIVATE
    mon_code
    GTest::gtest_main
)


include(GoogleTest)

gtest_discover_tests(mes_tests)