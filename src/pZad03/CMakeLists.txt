#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad03
# Author(s):                              Szs
#--------------------------------------------------------

SET(SRC
  Zad03.cpp
  Zad03_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad03 ${SRC})

TARGET_LINK_LIBRARIES(pZad03
   ${MOOS_LIBRARIES}
   apputil
   geometry
   mbutil
   m
   pthread)

