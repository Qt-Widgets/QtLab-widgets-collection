set(pathpicker_RES_FILES
  )

set(pathpicker_UI_FILES
  mainwindow.ui
)

qt5_add_resources(pathpicker_RES ${pathpicker_RES_FILES})
qt5_wrap_ui(pathpicker_UI ${pathpicker_UI_FILES})

set(pathpicker_SRCS main.cpp mainwindow.cpp)

add_executable(pathpicker ${pathpicker_SRCS} ${pathpicker_UI} ${pathpicker_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(pathpicker QSCore Qt5::Widgets)
