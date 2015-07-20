qt5_add_resources(qtoolbutton_defaultaction_RES ${PROJECT_SOURCE_DIR}/images.qrc)

set(qtoolbutton_defaultaction_SRCS main.cc testdialog.cc)

add_executable(qtoolbutton_defaultaction ${qtoolbutton_defaultaction_SRCS} ${qtoolbutton_defaultaction_RES})
target_link_libraries(qtoolbutton_defaultaction Qt5::Widgets)

add_test(qtoolbutton_defaultaction qtoolbutton_defaultaction)
