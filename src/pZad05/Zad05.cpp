/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad05.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Zad05.h"
#include <cmath>
#include <limits>
#include "XYSegList.h"

using namespace std;

//---------------------------------------------------------
// Constructor()

Zad05::Zad05()
{
    m_nav_x       = 0.0;
    m_nav_y       = 0.0;

}

//---------------------------------------------------------
// Destructor

Zad05::~Zad05()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool Zad05::OnNewMail(MOOSMSG_LIST &NewMail)
{
    AppCastingMOOSApp::OnNewMail(NewMail);
    for(auto const& msg : NewMail) {
        if(msg.GetKey() == "NAV_X") {
            m_nav_x = msg.GetDouble();
            m_pos_received = true;
        }
        if(msg.GetKey() == "NAV_Y") {
            m_nav_y = msg.GetDouble();
            m_pos_received = true;
        }
    }
    return true;
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool Zad05::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Zad05::Iterate()
{
    AppCastingMOOSApp::Iterate();
    if (!m_pos_received) return true;

    int active_nodes = 0;
    double max_signal = 0.0;

    for (auto const& [name, pt] : m_points) {
        double dist = hypot(pt.x - m_nav_x, pt.y - m_nav_y);

        if (dist <= m_max_range || true) {
            active_nodes++;
            double signal_percent = 100.0 * (1.0 - (dist / m_max_range));
            if (signal_percent > max_signal) max_signal = signal_percent;

 
            XYSegList seglist;
            seglist.add_vertex(pt.x, pt.y);
            seglist.add_vertex(m_nav_x, m_nav_y);
            std::string color = "";
            if (signal_percent > 50) {seglist.set_color("edge", "green");color = "green";}
            else if (signal_percent > 25) {seglist.set_color("edge", "yellow");color = "yellow";}
            else {seglist.set_color("edge", "red");color = "red";}
            
            std::string spec = "pts={" + std::to_string(pt.x) + "," + std::to_string(pt.y) + 
                   ":" + std::to_string(m_nav_x) + "," + std::to_string(m_nav_y) + 
                   "},label=z5pkt" + name + ",active=true,edge_color=" + color + ",edge_size=10";
            Notify("VIEW_SEGLIST", spec);
            std::string pt_spec = "x=" + std::to_string(pt.x) + ",y=" + std::to_string(pt.y) + 
                      ",label=" + name + ",color=white,vertex_size=12";
            Notify("VIEW_POINT", pt_spec);
        } else {

            //Notify("VIEW_SEGLIST", "label=" + name + ",active=false");
        }
    }   
    std::string comm_info = "signal_str=" + std::to_string((int)max_signal) + 
                            "%,comm_nodes=" + std::to_string(active_nodes);
    Notify("COMM_INFO", comm_info);


    if (active_nodes == 0) {
        //Notify("RETURN", "true");
        reportEvent("Utracono łączność! Zakończenie misji.");
    }

    return true;
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Zad05::OnStartUp()
{
    AppCastingMOOSApp::OnStartUp();
    STRING_LIST sParams;
    m_MissionReader.GetConfiguration(m_sAppName, sParams);

    for(auto const& sLine : sParams) {
    std::string tempLine = sLine; 
    std::string param = biteStringX(tempLine, '=');
    std::string value = tempLine; 
    
    if(param == "max_range") m_max_range = std::stod(value);
        if(param == "point") {
    std::string name = tokStringParse(sLine, "name", ',', '=');
    std::string x_str = tokStringParse(sLine, "x", ',', '=');
    reportEvent("Parsowanie: " + name + " | X: " + x_str); 
    
    if(!name.empty() && !x_str.empty()) {
        m_points[name] = {name, std::stod(x_str), std::stod(tokStringParse(sLine, "y", ',', '='))};
    } else {
        reportRunWarning("Błąd parsowania punktu: " + sLine);
    }
}
    }
    return true;
}



//------------------------------------------------------------
// Procedure: buildReport()

bool Zad05::buildReport() 
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

void Zad05::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}





