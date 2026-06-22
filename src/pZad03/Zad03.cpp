/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad03.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Zad03.h"
#include "XYPolygon.h"

using namespace std;

//---------------------------------------------------------
// Constructor()

Zad03::Zad03()
{
  m_nav_x = 0.0;
  m_nav_y = 0.0;
  m_poly_received = false;
}

//---------------------------------------------------------
// Destructor

Zad03::~Zad03()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool Zad03::OnNewMail(MOOSMSG_LIST &NewMail)
{
   AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p = NewMail.begin(); p != NewMail.end(); p++) {
        CMOOSMsg &msg = *p;
        std::string key = msg.GetKey();
        
    #if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
    #endif

        if (key == "NAV_X" && msg.IsDouble()) {
            m_nav_x = msg.GetDouble();
        }
        else if (key == "NAV_Y" && msg.IsDouble()) {
            m_nav_y = msg.GetDouble();
        }
        else if (key == "DEF_POLY" && msg.IsString()) {
            std::string poly_str = msg.GetString();
            if(!poly_str.empty()) {
                if(handleDefPoly(poly_str)) {
                    m_poly_received = true;
                    reportEvent("Pomyślnie sparsowano i utworzono nowy poligon.");
                } else {
                    reportRunWarning("Błąd parsowania komunikatu DEF_POLY: " + poly_str);
                }
            }
        }else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
    }
    return true;
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool Zad03::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Zad03::Iterate()
{
  AppCastingMOOSApp::Iterate();

  
    if(m_poly_received) {
        
        if(m_polygon.contains(m_nav_x, m_nav_y)) {
            m_polygon.set_color("edge", "green"); 
        } else {
            m_polygon.set_color("edge", "red");   
        }


        Notify("VIEW_POLYGON", m_polygon.get_spec());
    }

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Zad03::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables()

void Zad03::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("DEF_POLY", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Zad03::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}
bool Zad03::handleDefPoly(std::string msg_str)
{
    // 1. Usuwamy wszystkie spacje (zabezpieczenie przed literówkami typu "w=40, h=60")
    msg_str.erase(std::remove(msg_str.begin(), msg_str.end(), ' '), msg_str.end());

    // 2. Sprytny trik: ukrywamy przecinek wewnątrz nawiasów środka, 
    // żeby tokStringParse się nie pogubił!
    std::string safe_msg = msg_str;
    size_t c_start = safe_msg.find("center=(");
    if (c_start != std::string::npos) {
        size_t comma_pos = safe_msg.find(",", c_start);
        size_t c_end = safe_msg.find(")", c_start);
        // Jeśli przecinek jest wewnątrz nawiasów, zamieniamy go na '_'
        if (comma_pos != std::string::npos && comma_pos < c_end) {
            safe_msg[comma_pos] = '_'; 
        }
    }

    // 3. Teraz możemy bezpiecznie użyć wymaganej funkcji tokStringParse!
    std::string w_str = tokStringParse(safe_msg, "w", ',', '=');
    std::string h_str = tokStringParse(safe_msg, "h", ',', '=');
    std::string center_str = tokStringParse(safe_msg, "center", ',', '='); // Zwróci np. "(60_20)"

    // Sprawdzenie czy znaleziono wszystkie parametry
    if(w_str.empty() || h_str.empty() || center_str.empty()) {
        reportRunWarning("Brak pełnych danych! " + safe_msg);
        return false;
    }

    try {
        double w = std::stod(w_str);
        double h = std::stod(h_str);

        if(w <= 0 || h <= 0) return false;

        // center_str to teraz np. "(60_20)" - usuwamy nawiasy
        if(center_str.front() == '(') center_str.erase(0, 1);
        if(center_str.back() == ')')  center_str.pop_back();

        // Teraz center_str to "60_20" -> dzielimy za pomocą parseString po znaku '_'
        std::vector<std::string> coords = parseString(center_str, '_');
        if(coords.size() != 2) return false;

        double cx = std::stod(coords[0]);
        double cy = std::stod(coords[1]);

        // Obliczanie 4 wierzchołków prostokąta
        double left   = cx - (w / 2.0);
        double right  = cx + (w / 2.0);
        double bottom = cy - (h / 2.0);
        double top    = cy + (h / 2.0);

        // Tworzenie obiektu XYPolygon
        XYPolygon new_poly;
        new_poly.add_vertex(left, top);     
        new_poly.add_vertex(right, top);    
        new_poly.add_vertex(right, bottom); 
        new_poly.add_vertex(left, bottom);  
        
        // Konfiguracja wyglądu
        new_poly.set_label("strefa_zadania");
        new_poly.set_param("fill_color", "invisible"); 
        new_poly.set_param("edge_size", "2");

        m_polygon = new_poly;
        
        // Czyścimy ostrzeżenie z AppCastingu jeśli poprzednio wystąpił błąd
        retractRunWarning("Brak pełnych danych! " + safe_msg);
        return true;

    } catch (const std::exception& e) {
        reportRunWarning("Błąd konwersji tekstu na liczbę!");
        return false;
    }
}




