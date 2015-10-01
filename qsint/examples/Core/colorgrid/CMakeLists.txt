set(colorgrid_RES_FILES
  )

set(colorgrid_UI_FILES
  mainwindow.ui
)

qt5_add_resources(colorgrid_RES ${colorgrid_RES_FILES})
qt5_wrap_ui(colorgrid_UI ${colorgrid_UI_FILES})

set(colorgrid_SRCS main.cpp mainwindow.cpp)

add_executable(colorgrid ${colorgrid_SRCS} ${colorgrid_UI} ${colorgrid_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(colorgrid QSCore Qt5::Widgets)
