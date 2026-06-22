#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad08S
# Author(s):                              SzS
#--------------------------------------------------------

SET(SRC
  Zad08S.cpp
  Zad08S_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad08S ${SRC})

TARGET_LINK_LIBRARIES(pZad08S
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

