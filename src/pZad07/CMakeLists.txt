#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad07
# Author(s):                              SzS
#--------------------------------------------------------

SET(SRC
  Zad07.cpp
  Zad07_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad07 ${SRC})

TARGET_LINK_LIBRARIES(pZad07
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

