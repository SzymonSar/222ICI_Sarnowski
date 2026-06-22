#--------------------------------------------------------
# The CMakeLists.txt for:                       pZad05
# Author(s):                              Szs
#--------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(SRC
  Zad05.cpp
  Zad05_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pZad05 ${SRC})

TARGET_LINK_LIBRARIES(pZad05
   geometry
   mbutil
   apputil
   m
   pthread
   ${MOOS_LIBRARIES}
   )

