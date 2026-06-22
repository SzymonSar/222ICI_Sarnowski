/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad08S.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/
#include <iterator>
#include <cstdlib>
#include <cmath>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "Zad08S.h"
#include "ZAIC_PE.h"
#include "OF_Couple.h"

using namespace std;

//---------------------------------------------------------------
// Constructor
Zad08S::Zad08S(IvPDomain domain) : IvPBehavior(domain)
{
  this->setParam("descriptor", "zad08s");

  m_domain = subDomain(m_domain, "course,speed");

  m_contact_name  = "";
  m_chase_speed   = 2.0;
  m_detect_range  = 50.0;
  m_capture_range = 5.0;

  m_osx = 0; m_osy = 0;
  m_cnx = 0; m_cny = 0;
}

//---------------------------------------------------------------
// Procedure: setParam
bool Zad08S::setParam(string param, string val)
{
  if(param == "contact_name") {
    m_contact_name = val;
    return true;
  }
  else if(param == "chase_speed") {
    m_chase_speed = atof(val.c_str());
    return true;
  }
  else if(param == "detect_range") {
    m_detect_range = atof(val.c_str());
    return true;
  }
  else if(param == "capture_range") {
    m_capture_range = atof(val.c_str());
    return true;
  }
  return false;
}

//---------------------------------------------------------------
// Procedures: onSetParamComplete, onCompleteState, etc.
void Zad08S::onSetParamComplete() {}
void Zad08S::onCompleteState() {}
void Zad08S::onIdleState() {}
void Zad08S::onHelmetChg() {}
void Zad08S::postConfigStatus() {}
void Zad08S::onRunToIdleState() {}
void Zad08S::onIdleToRunState() {}

//---------------------------------------------------------------
// Procedure: onRunState()
IvPFunction* Zad08S::onRunState()
{
  bool ok_osx, ok_osy;
  m_osx = m_info_buffer->getValue("NAV_X", ok_osx);
  m_osy = m_info_buffer->getValue("NAV_Y", ok_osy);

  if(!ok_osx || !ok_osy) return 0; 

  string con_x_var = "NAV_X_" + toupper(m_contact_name);
  string con_y_var = "NAV_Y_" + toupper(m_contact_name);

  bool ok_cnx, ok_cny;
  m_cnx = m_info_buffer->getValue(con_x_var, ok_cnx);
  m_cny = m_info_buffer->getValue(con_y_var, ok_cny);

  if(!ok_cnx || !ok_cny) return 0;

  double dist = hypot(m_cnx - m_osx, m_cny - m_osy);

  if(dist < m_capture_range) {
    postMessage("A_CAUGHT", "true");
  }

 
  if(dist > m_detect_range) {
    return 0; 
  }


  double angle_rad = atan2(m_cny - m_osy, m_cnx - m_osx);
  double angle_deg = angle_rad * 180.0 / M_PI;
  double bearing = 90.0 - angle_deg;
  if(bearing < 0) bearing += 360.0;


  double chase_heading = bearing;

  ZAIC_PE crs_zaic(m_domain, "course");
  crs_zaic.setSummary("crs_chase");
  crs_zaic.setPeak(chase_heading, 100);
  crs_zaic.setBase(0);
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();

  ZAIC_PE spd_zaic(m_domain, "speed");
  spd_zaic.setSummary("spd_chase");
  spd_zaic.setPeak(m_chase_speed, 100);
  spd_zaic.setBase(0);
  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();

  OF_Couple couple(m_domain);
  IvPFunction *coupled_ipf = couple.couple(crs_ipf, spd_ipf, 50, 50);

  return coupled_ipf;
}