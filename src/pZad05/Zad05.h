/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad05.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad05_HEADER
#define Zad05_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPolygon.h"
#include "XYPoint.h"
#include <string>
#include <map>
#include "XYSegList.h"

struct CommPoint {
    std::string name;
    double x, y;
};

class Zad05 : public AppCastingMOOSApp
{
 public:
   Zad05();
   ~Zad05();

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

private:
    double m_max_range;
    std::map<std::string, CommPoint> m_points; 
    double m_nav_x, m_nav_y;
    bool   m_pos_received;
};

#endif 
