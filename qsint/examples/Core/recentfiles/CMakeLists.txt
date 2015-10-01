set(recentfiles_RES_FILES
  )

set(recentfiles_UI_FILES
  mainwindow.ui
)

qt5_add_resources(recentfiles_RES ${recentfiles_RES_FILES})
qt5_wrap_ui(recentfiles_UI ${recentfiles_UI_FILES})

set(recentfiles_SRCS main.cpp mainwindow.cpp)

add_executable(recentfiles ${recentfiles_SRCS} ${recentfiles_UI} ${recentfiles_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(recentfiles QSCore Qt5::Widgets)
