list(APPEND SUBTASKS
  CAST
  WRAP
  LITERAL
  CSV
  XPATH
  CONCAT
  BINARY
)

foreach(SUBTASK_NAME ${SUBTASKS})
  if(SUBTASK_${SUBTASK_NAME})
    message(STATUS "Subtask ${SUBTASK_NAME} enabled")
    string(TOLOWER ${SUBTASK_NAME} SUBTASK_LOWER)
    add_executable(subtask_${SUBTASK_LOWER}
      ${SUBTASK_LOWER}_test.cpp
    )
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUBTASK_LOWER}.h)
      target_sources(subtask_${SUBTASK_LOWER} PRIVATE ${SUBTASK_LOWER}.h)
    endif()
    target_link_libraries(subtask_${SUBTASK_LOWER}
      jsoncpp_static
      GTest::gtest_main
      GTest::gmock
    )
  endif()
endforeach()
