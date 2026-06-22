/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad07.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Zad07.h"
#include <cmath>
using namespace std;

//---------------------------------------------------------
// Constructor()

Zad07::Zad07()
{
  m_roles_assigned = false;
    m_alpha_arrived = false;
    m_bravo_arrived = false;

    m_last_target_x = 0;
    m_last_target_y = 0;
    m_target_x = 0;
    m_target_y = 0;

    m_alpha_nav_x = 0;
    m_alpha_nav_y = 0;
    m_bravo_nav_x = 0;
    m_bravo_nav_y = 0;

    m_first_arrival = "";
}

//---------------------------------------------------------
// Destructor

Zad07::~Zad07()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool Zad07::OnNewMail(MOOSMSG_LIST &NewMail)
{
    AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) {
        CMOOSMsg &msg = *p;
        string key    = msg.GetKey();

        // Odczytywanie zmiennych z MOOSDB
        if(key == "NAV_X_ALPHA") m_alpha_nav_x = msg.GetDouble();
        else if(key == "NAV_Y_ALPHA") m_alpha_nav_y = msg.GetDouble();
        else if(key == "NAV_X_BRAVO") m_bravo_nav_x = msg.GetDouble();
        else if(key == "NAV_Y_BRAVO") m_bravo_nav_y = msg.GetDouble();
        else if(key == "TARGET_X") m_target_x = msg.GetDouble();
        else if(key == "TARGET_Y") m_target_y = msg.GetDouble();
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool Zad07::OnConnectToServer()
{
   registerVariables();
   return(true);
}

///---------------------------------------------------------
// Procedure: Iterate()
bool Zad07::Iterate() {
    AppCastingMOOSApp::Iterate();

    // 1. WYKRYCIE ZMIANY CELU (LOGIKA NA 5.0)
    if (m_target_x != m_last_target_x || m_target_y != m_last_target_y) {
        // Resetowanie logiki misji
        m_alpha_arrived = false;
        m_bravo_arrived = false;
        m_roles_assigned = false;
        m_first_arrival = "";
        
        m_last_target_x = m_target_x;
        m_last_target_y = m_target_y;

        // Resetowanie flag w MOOSDB
        Notify("ROLES_ASSIGNED", "false");
        Notify("A_ARRIVED", "false");
        Notify("B_ARRIVED", "false");
        
        reportEvent("Wykryto nowy cel! Zresetowano logike.");
    }

    // Jeśli role są już przypisane, nie liczymy nic więcej
    if (m_roles_assigned) {
        AppCastingMOOSApp::PostReport();
        return true; 
    }

    // 2. OBLICZANIE ODLEGŁOŚCI
    double dist_alpha = hypot(m_alpha_nav_x - m_target_x, m_alpha_nav_y - m_target_y);
    double dist_bravo = hypot(m_bravo_nav_x - m_target_x, m_bravo_nav_y - m_target_y);

    // 3. SPRAWDZANIE KTO DOTARŁ
    if (dist_alpha <= 5.0 && !m_alpha_arrived) {
        m_alpha_arrived = true;
        Notify("A_ARRIVED", "true");
        if (m_first_arrival == "") m_first_arrival = "alpha"; // Zapamiętaj pierwszego
    }

    if (dist_bravo <= 5.0 && !m_bravo_arrived) {
        m_bravo_arrived = true;
        Notify("B_ARRIVED", "true");
        if (m_first_arrival == "") m_first_arrival = "bravo"; // Zapamiętaj pierwszego
    }

    // 4. PRZYDZIELANIE RÓL (Tylko gdy oba dotrą i decyzja nie zapadła)
    if (m_alpha_arrived && m_bravo_arrived && !m_roles_assigned) {
        
        Notify("ROLES_ASSIGNED", "true");
        Notify("FIRST_ARRIVAL", m_first_arrival);
        m_roles_assigned = true;

        // Formowanie aktualizacji dla behaviora Loiter
        string center = "x=" + doubleToString(m_target_x) + ",y=" + doubleToString(m_target_y);
        string inner_loiter = "polygon=radial::" + center + ",radius=15,pts=8,snap=1,clockwise=true";
        string outer_loiter = "polygon=radial::" + center + ",radius=30,pts=8,snap=1,clockwise=false";

        // Rozdzielenie na podstawie pierwszego
        if (m_first_arrival == "alpha") {
            Notify("LOITER_UPDATES_ALPHA", inner_loiter);
            Notify("LOITER_UPDATES_BRAVO", outer_loiter);
            reportEvent("Alpha dotarla pierwsza (INNER). Bravo druga (OUTER).");
        } else {
            Notify("LOITER_UPDATES_BRAVO", inner_loiter);
            Notify("LOITER_UPDATES_ALPHA", outer_loiter);
            reportEvent("Bravo dotarla pierwsza (INNER). Alpha druga (OUTER).");
        }
    }

    AppCastingMOOSApp::PostReport();
    return true;
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Zad07::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  registerVariables();  
  return(true);
}


//---------------------------------------------------------
// Procedure: registerVariables()

void Zad07::registerVariables() {
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X_ALPHA", 0);
  Register("NAV_Y_ALPHA", 0);
  Register("NAV_X_BRAVO", 0);
  Register("NAV_Y_BRAVO", 0);
  Register("TARGET_X", 0);
  Register("TARGET_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Zad07::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Status Misji Koordynowanej:                 " << endl;
  m_msgs << "============================================" << endl;

  m_msgs << "CEL (X,Y): " << m_target_x << " , " << m_target_y << endl;
  m_msgs << "Pierwszy na miejscu: " << (m_first_arrival.empty() ? "BRAK" : m_first_arrival) << endl;
  m_msgs << "Czy role przydzielone?: " << (m_roles_assigned ? "TAK" : "NIE") << endl;
  m_msgs << "--------------------------------------------" << endl;
  m_msgs << "Alpha  - Na miejscu: " << (m_alpha_arrived ? "TAK" : "NIE") << endl;
  m_msgs << "Bravo  - Na miejscu: " << (m_bravo_arrived ? "TAK" : "NIE") << endl;

  return(true);
}



