#--------------------------------------------------------
# The CMakeLists.txt for:                       pTargetMonitor
# Author(s):                              SzS
#--------------------------------------------------------

SET(SRC
  TargetMonitor.cpp
  TargetMonitor_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTargetMonitor ${SRC})

TARGET_LINK_LIBRARIES(pTargetMonitor
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)

