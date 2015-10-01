set(tablistmenu_RES_FILES
  res.qrc
  )

set(tablistmenu_UI_FILES
  mainwindow.ui
)

qt5_add_resources(tablistmenu_RES ${tablistmenu_RES_FILES})
qt5_wrap_ui(tablistmenu_UI ${tablistmenu_UI_FILES})

set(tablistmenu_SRCS main.cpp mainwindow.cpp)

add_executable(tablistmenu ${tablistmenu_SRCS} ${tablistmenu_UI} ${tablistmenu_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(tablistmenu QSCore Qt5::Widgets)
