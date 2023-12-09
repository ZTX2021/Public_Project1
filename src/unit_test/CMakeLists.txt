# vim: et ts=4 sts=4 sw=4 tw=0

add_executable(json_unit_test
    fuzz.cpp
    fuzz.h
    main.cpp
)

target_link_libraries(json_unit_test
    jsoncpp_static
    GTest::gtest_main
    GTest::gmock
)

# another way to solve issue #90
#set_target_properties(json_unit_test PROPERTIES COMPILE_FLAGS -ffloat-store)
