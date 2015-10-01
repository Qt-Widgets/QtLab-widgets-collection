set(optionslider_RES_FILES
  )

set(optionslider_UI_FILES
  mainwidget.ui
)

qt5_add_resources(optionslider_RES ${optionslider_RES_FILES})
qt5_wrap_ui(optionslider_UI ${optionslider_UI_FILES})

set(optionslider_SRCS main.cpp mainwidget.cpp)

add_executable(optionslider ${optionslider_SRCS} ${optionslider_UI} ${optionslider_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(optionslider QSCore Qt5::Widgets)
