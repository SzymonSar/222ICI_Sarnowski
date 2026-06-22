/************************************************************/
/*    NAME: Szs                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad02.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Zad02.h"
#include "XYPoint.h"
#include <algorithm>

using namespace std;

//---------------------------------------------------------
// Constructor()

Zad02::Zad02()
{
  m_point_counter = 1;
  Def_points = "(10,20);(30,40);(50,60)";
}

//---------------------------------------------------------
// Destructor

Zad02::~Zad02()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool Zad02::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
    MOOSMSG_LIST::iterator p;
    for(p = NewMail.begin(); p != NewMail.end(); p++) {
        CMOOSMsg &msg = *p;
        string key    = msg.GetKey();

        if (key == "DEF_POINTS" && msg.IsString()) {
            string points_str = msg.GetString();

            if(points_str.empty()) continue; 
            std::vector<string> svector = parseString(points_str, ';');
            for(unsigned int i = 0; i < svector.size(); i++) {
                string pt_str = svector[i];

                pt_str.erase(std::remove(pt_str.begin(), pt_str.end(), '('), pt_str.end());
                pt_str.erase(std::remove(pt_str.begin(), pt_str.end(), ')'), pt_str.end());

                std::vector<string> coords = parseString(pt_str, ',');
                
                if(coords.size() == 2) {
                    try {
                        double x = std::stod(coords[0]);
                        double y = std::stod(coords[1]);
                        XYPoint point(x, y);
                        string label = "P" + to_string(m_point_counter++);
                        point.set_label(label);
                        point.set_color("vertex", "magenta"); 
                        point.set_param("vertex_size", "6"); 
                        Notify("VIEW_POINT", point.get_spec());
                        reportEvent("Dodano punkt: " + label + " (" + coords[0] + ", " + coords[1] + ")");
                        
                    } catch (const std::exception& e) {
                        reportRunWarning("Błąd konwersji ze stringa na double!");
                    }
                }
            }
        }
    }
    return true;
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool Zad02::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Zad02::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Zad02::OnStartUp()
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

void Zad02::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("DEF_POINTS", 0);

}


//------------------------------------------------------------
// Procedure: buildReport()

bool Zad02::buildReport() 
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




