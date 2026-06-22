#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad02
# Author(s):                              Szs
#--------------------------------------------------------

SET(SRC
  Zad02.cpp
  Zad02_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad02 ${SRC})

TARGET_LINK_LIBRARIES(pZad02
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   pthread)

