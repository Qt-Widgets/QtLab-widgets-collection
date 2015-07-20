set(show_grid_border_SRCS main.cc show_grid_border.cc)
add_executable(show_grid_border ${show_grid_border_SRCS})
# Use the Widgets module from Qt 5.
target_link_libraries(show_grid_border Qt5::Widgets)

add_test(show_grid_border show_grid_border)
