/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad07.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad07_HEADER
#define Zad07_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
class Zad07 : public AppCastingMOOSApp
{
 public:
   Zad07();
   ~Zad07();

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

 protected:
    double m_last_target_x;
    double m_last_target_y;
    double m_alpha_nav_x, m_alpha_nav_y;
    double m_bravo_nav_x, m_bravo_nav_y;
    double m_target_x, m_target_y;
    bool m_alpha_arrived;
    bool m_bravo_arrived;
    bool m_roles_assigned;
    std::string m_first_arrival;
};

#endif 
