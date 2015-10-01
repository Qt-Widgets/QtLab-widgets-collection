set(qsint_Charts_SRCS
  plotterbase.cpp
  barchartplotter.cpp
  axisbase.cpp
  piechart.cpp
  ringchart.cpp
  paintscheme.cpp
  )

add_library(QSCharts ${qsint_Charts_SRCS} ${qsint_Charts_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(QSCharts Qt5::Widgets)
