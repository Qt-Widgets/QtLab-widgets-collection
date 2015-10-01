set(actionlabel_RES_FILES
  res.qrc
  )

set(actionlabel_UI_FILES
  mainwindow.ui
)

qt5_add_resources(actionlabel_RES ${actionlabel_RES_FILES})
qt5_wrap_ui(actionlabel_UI ${actionlabel_UI_FILES})

set(actionlabel_SRCS main.cpp mainwindow.cpp)

add_executable(actionlabel ${actionlabel_SRCS} ${actionlabel_UI} ${actionlabel_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(actionlabel QSCore Qt5::Widgets)
