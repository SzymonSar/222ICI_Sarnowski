/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad01.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad01_HEADER
#define Zad01_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class Zad01 : public AppCastingMOOSApp
{
 public:
   Zad01();
   ~Zad01();

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
 double curr_x;
 double curr_y;
 double prev_x;
 double prev_y;
 double distTotal;
 double ifRun;
};

#endif 
