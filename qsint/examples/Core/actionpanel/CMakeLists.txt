set(actionpanel_RES_FILES
  res.qrc
  )

set(actionpanel_UI_FILES
  mainwindow.ui
)

qt5_add_resources(actionpanel_RES ${actionpanel_RES_FILES})
qt5_wrap_ui(actionpanel_UI ${actionpanel_UI_FILES})

set(actionpanel_SRCS main.cpp mainwindow.cpp)

add_executable(actionpanel ${actionpanel_SRCS} ${actionpanel_UI} ${actionpanel_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(actionpanel QSCore Qt5::Widgets)
