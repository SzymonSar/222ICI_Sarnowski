/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad02.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad02_HEADER
#define Zad02_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include  <iostream>

class Zad02 : public AppCastingMOOSApp
{
 public:
   Zad02();
   ~Zad02();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
 std::string Def_points;
 int m_point_counter;
 
};

#endif 
