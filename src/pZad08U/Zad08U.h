/************************************************************/
/*    NAME: SzS                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Zad08U.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/
#ifndef ZAD08U_HEADER
#define ZAD08U_HEADER

#include <string>
#include "IvPBehavior.h"

class Zad08U : public IvPBehavior {
public:
  Zad08U(IvPDomain);
  ~Zad08U() {}

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
  double      m_evade_range;
  double      m_evade_speed;

  double      m_osx;
  double      m_osy;
  double      m_cnx;
  double      m_cny;
};

#endif