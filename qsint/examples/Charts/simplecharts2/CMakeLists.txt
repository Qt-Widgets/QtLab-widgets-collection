set(simplecharts2_RES_FILES
  )

set(simplecharts2_UI_FILES
  mainwidget.ui
)

qt5_add_resources(simplecharts2_RES ${simplecharts2_RES_FILES})
qt5_wrap_ui(simplecharts2_UI ${simplecharts2_UI_FILES})

set(simplecharts2_SRCS main.cpp mainwidget.cpp)

add_executable(simplecharts2 ${simplecharts2_SRCS} ${simplecharts2_UI} ${simplecharts2_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(simplecharts2 QSCore QSCharts Qt5::Widgets)
