/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: TargetMonitor.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "TargetMonitor.h"
#include "XYPoint.h"

using namespace std;

//---------------------------------------------------------
// Constructor()

TargetMonitor::TargetMonitor()
{
  nav_x = 0;
  nav_y = -20;
  p1x = 0;
  p1y = 0;
  distance = 0;
}

//---------------------------------------------------------
// Destructor

TargetMonitor::~TargetMonitor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool TargetMonitor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    if(key == "FOO") 
       cout << "great!";
    else if (key == "NAV_X" && msg.IsDouble()) {
          nav_x = (int)msg.GetDouble();
      }
      else if (key == "NAV_Y" && msg.IsDouble()) {
          nav_y = (int)msg.GetDouble();
      }
    else if (key == "DEF_POINTS" && msg.IsString()) {
      string points_str = msg.GetString();
      if(points_str.empty()) continue; 
      std::vector<string> svector = parseString(points_str, ';');
      //svector.size()
      for(unsigned int i = 0; i < 1; i++) {
                string pt_str = svector[i];

                pt_str.erase(std::remove(pt_str.begin(), pt_str.end(), '('), pt_str.end());
                pt_str.erase(std::remove(pt_str.begin(), pt_str.end(), ')'), pt_str.end());

                std::vector<string> coords = parseString(pt_str, ',');
                
                if(coords.size() == 2) {
                    try {
                        double x = std::stod(coords[0]);
                        double y = std::stod(coords[1]);
                        p1x = x;
                        p1y = y;
                        XYPoint point(x, y);
                        string label = "Punkt " + to_string(i+1);
                        point.set_label(label);
                        point.set_color("vertex", "magenta"); 
                        point.set_param("vertex_size", "6"); 
                        Notify("VIEW_POINT", point.get_spec());
                        
                        
                    } catch (const std::exception& e) {
                        reportRunWarning("Błąd konwersji ze stringa na double!");
                    }
                }
            }
    }else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool TargetMonitor::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TargetMonitor::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  distance = sqrt(((nav_x-p1x)*(nav_x-p1x)) + ((nav_y-p1y)*(nav_y-p1y)));
  Notify("TARGET_DISTANCE", distance);
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TargetMonitor::OnStartUp()
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

void TargetMonitor::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("DEF_POINTS", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TargetMonitor::buildReport() 
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




