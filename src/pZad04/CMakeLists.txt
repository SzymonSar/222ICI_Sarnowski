#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad04
# Author(s):                              Szs
#--------------------------------------------------------

SET(SRC
  Zad04.cpp
  Zad04_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad04 ${SRC})

TARGET_LINK_LIBRARIES(pZad04
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)

