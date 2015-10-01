set(spinslider_RES_FILES
  )

set(spinslider_UI_FILES
  mainwidget.ui
)

qt5_add_resources(spinslider_RES ${spinslider_RES_FILES})
qt5_wrap_ui(spinslider_UI ${spinslider_UI_FILES})

set(spinslider_SRCS main.cpp mainwidget.cpp)

add_executable(spinslider ${spinslider_SRCS} ${spinslider_UI} ${spinslider_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(spinslider QSCore Qt5::Widgets)
