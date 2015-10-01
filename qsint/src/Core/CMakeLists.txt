qt5_add_resources(qsint_Core_RES
  ${CMAKE_CURRENT_SOURCE_DIR}/actionpanel/schemes.qrc
  ${CMAKE_CURRENT_SOURCE_DIR}/res/CommonIcons.qrc)

set(qsint_Core_SRCS
  recentfiles.cpp
  colorgrid.cpp
  colorbutton.cpp
  actionlabel.cpp
  actionbox.cpp
  scrollpanner.cpp
  widgetpopup.cpp
  tablistmenu.cpp
  actionpanel/actionpanel.cpp
  actionpanel/actionpanelscheme.cpp
  actionpanel/taskheader_p.cpp
  actionpanel/taskgroup_p.cpp
  actionpanel/actiongroup.cpp
  actionpanel/winxppanelscheme.cpp
  actionpanel/winvistapanelscheme.cpp
  actionpanel/macpanelscheme.cpp
  pathpicker.cpp
  actionpanel/androidpanelscheme.cpp
  optionslider.cpp
  spinslider.cpp
  ledwidget.cpp
  )

include_directories(actionpanel)

add_library(QSCore ${qsint_Core_SRCS} ${qsint_Core_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(QSCore Qt5::Widgets)
