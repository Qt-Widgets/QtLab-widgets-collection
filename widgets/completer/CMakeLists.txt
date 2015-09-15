#qt5_add_resources(completer_demo_RES ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)
#qt5_add_resources(completer_demo_RES ${PROJECT_SOURCE_DIR}/images.qrc)

set(completer_demo_SRCS
  main.cc
  candidate_item_model.cc
  completer_line_edit.cc
  custom_update_line_edit.cc
  recent_all_completer.cc
  recent_all_line_edit.cc
  treemodelcompleter.cc
  tree_combobox.cc
  widget_wrapper.cc
  )

add_executable(completer_demo ${completer_demo_SRCS} ${completer_demo_RES})
target_link_libraries(completer_demo Qt5::Widgets)

add_test(completer_demo completer_demo)
