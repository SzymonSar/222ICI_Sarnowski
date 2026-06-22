#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad01
# Author(s):                              SzS
#--------------------------------------------------------

SET(SRC
  Zad01.cpp
  Zad01_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad01 ${SRC})

TARGET_LINK_LIBRARIES(pZad01
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

