set(relocatable_gridlayout_SRCS main.cc relocatable_gridlayout.cc layout_engine.cc)
add_executable(relocatable_gridlayout ${relocatable_gridlayout_SRCS})
# Use the Widgets module from Qt 5.
target_link_libraries(relocatable_gridlayout Qt5::Widgets)

add_test(relocatable_gridlayout relocatable_gridlayout)
