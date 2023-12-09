# The new Python3 module is much more robust than the previous PythonInterp
find_package(Python3 COMPONENTS Interpreter)
# Set variables for backwards compatibility with cmake < 3.12.0
set(PYTHONINTERP_FOUND ${Python3_Interpreter_FOUND})
set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})

add_executable(json_intg_test
    main.cpp
)

    target_link_libraries(json_intg_test jsoncpp_static)

set_target_properties(json_intg_test PROPERTIES OUTPUT_NAME json_intg_test)

if(PYTHONINTERP_FOUND)
    # Run end to end parser/writer tests
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../test)
    set(RUNJSONTESTS_PATH ${TEST_DIR}/run_intg_tests.py)

    # Run unit tests in post-build
    # (default cmake workflow hides away the test result into a file, resulting in poor dev workflow?!?)
    add_custom_target(run_intg_test
        "${PYTHON_EXECUTABLE}" "${RUNJSONTESTS_PATH}" $<TARGET_FILE:json_intg_test>
        DEPENDS json_intg_test
    )
endif()
