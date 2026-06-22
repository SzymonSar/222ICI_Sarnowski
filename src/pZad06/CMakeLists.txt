#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad06
# Author(s):                              SzS'


#--------------------------------------------------------

SET(SRC
  Zad06.cpp
  Zad06_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad06 ${SRC})

TARGET_LINK_LIBRARIES(pZad06
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

