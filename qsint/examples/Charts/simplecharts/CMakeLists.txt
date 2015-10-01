set(simplecharts_RES_FILES
  )

set(simplecharts_UI_FILES
  mainwidget.ui
)

qt5_add_resources(simplecharts_RES ${simplecharts_RES_FILES})
qt5_wrap_ui(simplecharts_UI ${simplecharts_UI_FILES})

set(simplecharts_SRCS main.cpp mainwidget.cpp)

add_executable(simplecharts ${simplecharts_SRCS} ${simplecharts_UI} ${simplecharts_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(simplecharts QSCore QSCharts Qt5::Widgets)
