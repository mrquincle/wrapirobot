#ifndef _IROBOT_H
#define _IROBOT_H

#include "RobotKIT.h"
#include "RobotScout.h"
#include "RobotAWheel.h"

/**
 * Sorry, for this construction. But if you compile multiple object files, you will get an error caused by
 * including the gSOAP headers multiple times for multiple compilation units. Probably they made some errors
 * in defining not everything "extern".
 * Specific error: multiple definition of `_namespaces'
 *
 * Now, you run the risk to have MAINFILE not defined
 */
#ifdef MAINFILE
#include "Udata.nsmap"
#endif

#include "IRobotWrapper.h"

#endif // _IROBOT_H
