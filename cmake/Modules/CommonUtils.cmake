MACRO(ADD_CPPLINT_SUPPORT root)
add_custom_target(cpplint
  run_cpplint.sh ${CMAKE_CURRENT_SOURCE_DIR} ${root}
  VERBATIM)
ENDMACRO(ADD_CPPLINT_SUPPORT)

MACRO(AUTO_ADD_SIMPLE_TARGETS)
file (GLOB SRC_FILES *.cc)
#message("===============source files: ${SRC_FILES}")
foreach(SRC_FILE ${SRC_FILES})
  string(REGEX REPLACE ".*/\(.*\)[.]cc$" "\\1" TGT_NAME ${SRC_FILE})
  add_executable(${TGT_NAME} ${SRC_FILE})
  target_link_libraries(${TGT_NAME} Qt5::Widgets)
  add_test(${TGT_NAME} ${TGT_NAME})
endforeach(SRC_FILE)
ENDMACRO(AUTO_ADD_SIMPLE_TARGETS)
