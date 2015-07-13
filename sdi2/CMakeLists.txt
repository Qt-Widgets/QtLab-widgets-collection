find_package(Qt5LinguistTools)

qt5_add_resources(sdi2_RES ${CMAKE_CURRENT_SOURCE_DIR}/images.qrc)

set(sdi2_SRCS main.cpp sdiwindow.cpp)

# Translations
  if(${Qt5LinguistTools_FOUND})
      message(STATUS "Qt Linguist found - will set up translations")
#      qt5_create_translation(sdi2_QMS
#          ${sdi2_SRCS}
	qt5_add_translation(sdi2_QMS
          "${CMAKE_CURRENT_SOURCE_DIR}/qt_zh_CN.ts"
          "${CMAKE_CURRENT_SOURCE_DIR}/sdi_zh_CN.ts"
      )
  endif(${Qt5LinguistTools_FOUND})

message(STATUS "sdi2_RES is ${sdi2_RES} ${sdi2_QMS}")

add_executable(sdi2 ${sdi2_SRCS} ${sdi2_RES} ${sdi2_QMS})
# Use the Widgets module from Qt 5.
target_link_libraries(sdi2 Qt5::Widgets)

add_test(sdi2 sdi2)
