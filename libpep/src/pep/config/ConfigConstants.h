/*
 * ConfigConstants.h
 *
 *  Created on: Apr 20, 2013
 *      Author: watanabe
 */

#ifndef CONFIGCONSTANTS_H_
#define CONFIGCONSTANTS_H_

#include <xercesc/util/XercesDefs.hpp>

namespace pep {

/** PEP Config Element ("PEPConfig") */
extern const XMLCh PEP_CONFIG_ELEM[];

/** PEP Config Element ("ApplicationDefaults") */
extern const XMLCh APPLICATION_DEFAULTS_ELEM[];

/** PEP Config Element ("ApplicationOverride") */
extern const XMLCh APPLICATION_OVERRIDE_ELEM[];

/** PEP Config Element ("Authorization") */
extern const XMLCh AUTHORIZATION_ELEM[];

/** PEP Config Attribute ("Location") */
extern const XMLCh LOCATION_ATTR[];

/** PEP Config Element ("Listener") */
extern const XMLCh LISTENER_ELEM[];

/** PEP Config Element ("UnixListener") */
extern const XMLCh UNIX_LISTENER_ELEM[];

/** PEP Config Element ("TCPListener") */
extern const XMLCh TCP_LISTENER_ELEM[];

/** PEP Config Attribute ("address") */
extern const XMLCh ADDRESS_ATTR[];

/** PEP Config Attribute ("stackSize") */
extern const XMLCh STACK_SIZE_ATTR[];

}
/* namespace pep */
#endif /* CONFIGCONSTANTS_H_ */
