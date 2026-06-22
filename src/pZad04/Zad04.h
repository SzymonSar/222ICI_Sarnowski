/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad04.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Zad04_HEADER
#define Zad04_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPolygon.h"
#include "XYPoint.h"
#include <string>
#include <map>
#include <limits>

// Struktura przechowująca informacje o punkcie
struct TargetPoint {
    std::string id;
    double x;
    double y;
};
class Zad04 : public AppCastingMOOSApp
{
 public:
   Zad04();
   ~Zad04();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   
 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   bool updateSonarPolygon();

 private: // Configuration variables

 private: // Zmienne stanu (State variables)
   double m_nav_x;
   double m_nav_y;
   double m_nav_hdg;
   
   // Parametry sonaru
   double m_sonar_angle;
   double m_sonar_range;

   // Poligon sonaru do wizualizacji i detekcji
   XYPolygon m_sonar_poly;

   // Mapa punktów (ID -> TargetPoint) dla łatwego zarządzania
   std::map<std::string, TargetPoint> m_points;
};

#endif 
