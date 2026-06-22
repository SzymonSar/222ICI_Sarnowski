#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad08U
# Author(s):                              SzS
#--------------------------------------------------------

SET(SRC
  Zad08U.cpp
  Zad08U_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad08U ${SRC})

TARGET_LINK_LIBRARIES(pZad08U
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

