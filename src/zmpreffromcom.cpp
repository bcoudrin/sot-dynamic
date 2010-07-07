/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      ZmprefFromCom.h
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <sot-dynamic/zmpreffromcom.h>
#include <sot-core/debug.h>
#include <dynamic-graph/factory.h>

using namespace sot;
using namespace dynamicgraph;
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(ZmprefFromCom,"ZmprefFromCom");


const double ZmprefFromCom::DT_DEFAULT = 5e-3; 
const double ZmprefFromCom::FOOT_HEIGHT_DEFAULT = .105; 

ZmprefFromCom::
ZmprefFromCom( const std::string & name ) 
  :Entity(name)
   ,dt(DT_DEFAULT) 
   ,footHeight(FOOT_HEIGHT_DEFAULT)
   ,waistPositionSIN(NULL,"sotZmprefFromCom("+name+")::input(MatrixHomo)::waist")
   ,comPositionSIN(NULL,"sotZmprefFromCom("+name+")::input(Vector)::com")
  ,dcomSIN(NULL,"sotZmprefFromCom("+name+")::input(Vector)::dcom")
   ,zmprefSOUT( boost::bind(&ZmprefFromCom::computeZmpref,this,_1,_2),
		waistPositionSIN<<comPositionSIN<<dcomSIN,
		"sotZmprefFromCom("+name+")::output(RPY)::zmpref" ) 
{
  sotDEBUGIN(5);

  signalRegistration(waistPositionSIN<<comPositionSIN<<dcomSIN<<zmprefSOUT );

  sotDEBUGOUT(5);
}


ZmprefFromCom::
~ZmprefFromCom( void )
{
  sotDEBUGIN(5);

  sotDEBUGOUT(5);
  return;
}

/* --- SIGNALS -------------------------------------------------------------- */
/* --- SIGNALS -------------------------------------------------------------- */
/* --- SIGNALS -------------------------------------------------------------- */
ml::Vector& ZmprefFromCom::
computeZmpref( ml::Vector& res,
	       const int& time )
{
  sotDEBUGIN(15);
  
  const ml::Vector& com = comPositionSIN( time );
  const ml::Vector& dcom = dcomSIN( time );
  const MatrixHomogeneous& oTw = waistPositionSIN( time );

  MatrixHomogeneous wTo = oTw.inverse();
  ml::Vector nextComRef = dcom;  nextComRef*= dt;nextComRef+=com;
  
  
  nextComRef(2) = -footHeight; // projection on the ground.
  wTo.multiply(nextComRef,res);

  sotDEBUGOUT(15);
  return res;
}



/* --- PARAMS --------------------------------------------------------------- */
/* --- PARAMS --------------------------------------------------------------- */
/* --- PARAMS --------------------------------------------------------------- */
void ZmprefFromCom::
commandLine( const std::string& cmdLine,
	     std::istringstream& cmdArgs,
	     std::ostream& os )
{
  sotDEBUG(25) << "Cmd " << cmdLine <<std::endl;

  if( cmdLine == "help" )
    {
      os<<"zmprefFromCom:" << std::endl
	<<"  - dt [<value>]: get/set the dt value." << std::endl
	<<"  - footHeight [<value>]: "
	<<"get/set the height of the foot ("
	<< FOOT_HEIGHT_DEFAULT <<" by default)." <<std::endl;
      Entity::commandLine(cmdLine,cmdArgs,os);
    }
  if( cmdLine == "dt" )
    {
      cmdArgs>>std::ws;
      if( cmdArgs.good() ) cmdArgs>>dt; else os <<"dt = " << dt<<std::endl;
    }
  if( cmdLine == "footHeight" )
    {
      cmdArgs>>std::ws;
      if( cmdArgs.good() ) cmdArgs>>footHeight; 
      else os <<"footHeight = " << footHeight<<std::endl;
    }
  else { Entity::commandLine( cmdLine,cmdArgs,os); }
}

