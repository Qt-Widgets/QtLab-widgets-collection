set(scrollpanner_RES_FILES
  )

set(scrollpanner_UI_FILES
  mainwindow.ui
)

qt5_add_resources(scrollpanner_RES ${scrollpanner_RES_FILES})
qt5_wrap_ui(scrollpanner_UI ${scrollpanner_UI_FILES})

set(scrollpanner_SRCS main.cpp mainwindow.cpp)

add_executable(scrollpanner ${scrollpanner_SRCS} ${scrollpanner_UI} ${scrollpanner_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(scrollpanner QSCore Qt5::Widgets)
