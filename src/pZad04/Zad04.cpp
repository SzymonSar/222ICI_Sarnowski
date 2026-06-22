/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad04.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Zad04.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include <cmath>
#include <limits>
using namespace std;

//---------------------------------------------------------
// Constructor()

Zad04::Zad04()
{
    m_nav_x       = 0.0;
    m_nav_y       = 0.0;
    m_nav_hdg     = 0.0;
    m_sonar_angle = 90.0;
    m_sonar_range = 20.0;
}

//---------------------------------------------------------
// Destructor

Zad04::~Zad04()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool Zad04::OnNewMail(MOOSMSG_LIST &NewMail)
{
    AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p = NewMail.begin(); p != NewMail.end(); p++) {
        CMOOSMsg &msg = *p;
        string key   = msg.GetKey();

        if (key == "NAV_X") m_nav_x = msg.GetDouble();
        else if (key == "NAV_Y") m_nav_y = msg.GetDouble();
        else if (key == "NAV_HEADING") m_nav_hdg = msg.GetDouble();
        
        else if (key == "DEF_POINTS" && msg.IsString()) {
            string content = msg.GetString();
            
            // Czyszczenie wszystkich punktów
            if (tolower(content) == "clear") {
                m_points.clear();
                reportEvent("Usunięto wszystkie punkty.");
                continue;
            }

            // Dekodowanie nowego punktu z walidacją wejścia
            string id_str = tokStringParse(content, "id", ',', '=');
            if (id_str.empty()) {
                reportRunWarning("Błędny komunikat DEF_POINTS (brak ID): " + content);
                continue;
            }

            string remove_str = tokStringParse(content, "remove", ',', '=');
            if (tolower(remove_str) == "true") {
                m_points.erase(id_str);
                reportEvent("Usunięto punkt: " + id_str);
                continue;
            }

            try {
                double x = stod(tokStringParse(content, "x", ',', '='));
                double y = stod(tokStringParse(content, "y", ',', '='));
                
                TargetPoint tp;
                tp.id = id_str;
                tp.x = x;
                tp.y = y;
                m_points[id_str] = tp;
                reportEvent("Dodano/Zaktualizowano punkt: " + id_str);
            } catch (...) {
                reportRunWarning("Błąd parsowania współrzędnych punktu: " + content);
            }
        }
        else if (key == "SONAR_CONFIG_UPDATE" && msg.IsString()) {
            string content = msg.GetString();
            try {
                double new_angle = stod(tokStringParse(content, "sonar_angle", ',', '='));
                double new_range = stod(tokStringParse(content, "sonar_range", ',', '='));
                
                if (new_angle > 0 && new_range > 0) {
                    m_sonar_angle = new_angle;
                    m_sonar_range = new_range;
                    reportEvent("Aktualizacja sonaru -> Kąt: " + to_string(m_sonar_angle) + 
                                ", Zasięg: " + to_string(m_sonar_range));
                } else {
                    reportRunWarning("Niepoprawne (ujemne) parametry sonaru!");
                }
            } catch (...) {
                reportRunWarning("Błąd parsowania SONAR_CONFIG_UPDATE: " + content);
            }
        }
    }
    return true;
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool Zad04::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second


bool Zad04::Iterate()
{
    AppCastingMOOSApp::Iterate();

    // 1. Aktualizacja i rysowanie wizualizacji sonaru
    updateSonarPolygon();

    // 2. Analiza punktów (Detekcja)
    int detected_count = 0;
    double min_distance = std::numeric_limits<double>::max();
    string nearest_id = "";
    string detected_list = "";

    // Przechodzimy przez wszystkie zapisane punkty
    for (auto const& [id, pt] : m_points) {
        bool is_detected = m_sonar_poly.contains(pt.x, pt.y);
        
        XYPoint point_vis(pt.x, pt.y);
        point_vis.set_label(pt.id);

        if (is_detected) {
            detected_count++;
            detected_list += pt.id + ",";
            
            // Obliczanie odległości
            double dist = hypot(pt.x - m_nav_x, pt.y - m_nav_y);
            if (dist < min_distance) {
                min_distance = dist;
                nearest_id = pt.id;
            }
            
            // Wykryty punkt kolorujemy na żółto
            point_vis.set_color("vertex", "yellow"); 
        } else {
            // Niewykryty punkt jest szary
            point_vis.set_color("vertex", "gray");
        }
        
        // Zapisujemy specyfikację do publikacji
        Notify("VIEW_POINT", point_vis.get_spec());
    }

    // 3. Oznaczanie najbliższego punktu na czerwono (Ocena 5)
    if (detected_count > 0 && !nearest_id.empty()) {
        TargetPoint nearest = m_points[nearest_id];
        XYPoint highlight_pt(nearest.x, nearest.y);
        highlight_pt.set_label(nearest.id + "_NEAREST");
        highlight_pt.set_color("vertex", "red");
        highlight_pt.set_param("vertex_size", "8"); // Powiększamy
        Notify("VIEW_POINT", highlight_pt.get_spec());
    }

    // 4. Publikowanie raportów do bazy
    if (detected_count > 0) {
        string sonar_info = "count=" + to_string(detected_count) + 
                            ",nearest=" + to_string(min_distance);
        Notify("SONAR_INFO", sonar_info);
        
        // Publikacja dodatkowej zmiennej (Ocena 5)
        if (!detected_list.empty()) {
            detected_list.pop_back(); // Usuwa ostatni przecinek
            Notify("SONAR_DETECTED_IDS", detected_list);
        }
    } else {
        // Konsekwentny zapis przy braku obiektów
        Notify("SONAR_INFO", "count=0,nearest=-1");
        Notify("SONAR_DETECTED_IDS", "none");
    }

    AppCastingMOOSApp::PostReport();
    return true;
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Zad04::OnStartUp()
{
    AppCastingMOOSApp::OnStartUp();

    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(!m_MissionReader.GetConfiguration(m_sAppName,sParams))
        reportConfigWarning("Brak pliku konfiguracyjnego.");

    STRING_LIST::iterator p;
    for(p = sParams.begin(); p != sParams.end(); p++) {
        string orig  = *p;
        string line  = *p;
        string param = tolower(biteStringX(line, '='));
        string value = line;

        if(param == "sonar_angle") {
            m_sonar_angle = stod(value);
        }
        else if(param == "sonar_range") {
            m_sonar_range = stod(value);
        }
    }

    registerVariables();
    return true;
}


//---------------------------------------------------------
// Procedure: registerVariables()

void Zad04::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
    Register("NAV_X", 0);
    Register("NAV_Y", 0);
    Register("NAV_HEADING", 0);
    Register("DEF_POINTS", 0);
    Register("SONAR_CONFIG_UPDATE", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Zad04::buildReport() 
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

bool Zad04::updateSonarPolygon()
{
    m_sonar_poly.clear();
    m_sonar_poly.add_vertex(m_nav_x, m_nav_y); // Środek sonaru to pojazd

    int arc_steps = 15; // Gładkość łuku
    double start_hdg = m_nav_hdg - (m_sonar_angle / 2.0);
    double step = m_sonar_angle / arc_steps;

    for (int i = 0; i <= arc_steps; i++) {
        double current_hdg = start_hdg + (i * step);
        // Konwersja heading (MOOS) na kąt matematyczny w radianach
        double math_rad = (90.0 - current_hdg) * M_PI / 180.0;
        
        double px = m_nav_x + m_sonar_range * cos(math_rad);
        double py = m_nav_y + m_sonar_range * sin(math_rad);
        m_sonar_poly.add_vertex(px, py);
    }

    m_sonar_poly.set_label("sonar_fov");
    m_sonar_poly.set_color("edge", "cyan");
    m_sonar_poly.set_color("fill", "dodger_blue");
    m_sonar_poly.set_param("vertex_size", "0");
    m_sonar_poly.set_param("edge_size", "1");
    
    Notify("VIEW_POLYGON", m_sonar_poly.get_spec());
    return true;
}


