/************************************************************/
/*    NAME: SzS'

                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad06.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad06_HEADER
#define Zad06_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYSegList.h"

struct CommPoint {
    std::string name;
    double x, y;
};

class Zad06 : public AppCastingMOOSApp
{
 public:
   Zad06();
   ~Zad06();

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
