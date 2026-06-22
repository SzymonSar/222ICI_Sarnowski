/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad08S.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/
#ifndef ZAD08S_HEADER
#define ZAD08S_HEADER

#include <string>
#include "IvPBehavior.h"

class Zad08S : public IvPBehavior {
public:
  Zad08S(IvPDomain);
  ~Zad08S() {}

  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmetChg();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected:
  std::string m_contact_name;
  double      m_chase_speed;
  double      m_detect_range;
  double      m_capture_range;

  double      m_osx;
  double      m_osy;
  double      m_cnx;
  double      m_cny;
};

#endif