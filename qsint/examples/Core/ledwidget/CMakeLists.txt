set(ledwidget_RES_FILES
  )

set(ledwidget_UI_FILES
  ledwidgetdemo.ui
)

qt5_add_resources(ledwidget_RES ${ledwidget_RES_FILES})
qt5_wrap_ui(ledwidget_UI ${ledwidget_UI_FILES})

set(ledwidget_SRCS main.cpp ledwidgetdemo.cpp)

add_executable(ledwidget ${ledwidget_SRCS} ${ledwidget_UI} ${ledwidget_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(ledwidget QSCore Qt5::Widgets)
