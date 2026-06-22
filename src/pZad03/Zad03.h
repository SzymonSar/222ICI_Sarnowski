/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad03.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad03_HEADER
#define Zad03_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPolygon.h"
#include <string>

class Zad03 : public AppCastingMOOSApp
{
 public:
   Zad03();
   ~Zad03();

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
    double    m_nav_x;
    double    m_nav_y;
    XYPolygon m_polygon;
    bool      m_poly_received;
    bool      handleDefPoly(std::string msg_str);
};

#endif 
