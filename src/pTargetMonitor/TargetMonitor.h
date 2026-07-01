/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: TargetMonitor.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef TargetMonitor_HEADER
#define TargetMonitor_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class TargetMonitor : public AppCastingMOOSApp
{
 public:
   TargetMonitor();
   ~TargetMonitor();

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
 int nav_x;
 int nav_y;
 int p1x;
 int p1y;
 double distance;
};

#endif 
