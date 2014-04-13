/*
 * ConfigConstants.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: watanabe
 */

#include "config/ConfigConstants.h"

#include <xercesc/util/XMLUniDefs.hpp>
#include <xmltooling/base.h>

using namespace pep;
using namespace xercesc;

/** PEP Config Element ("PEPConfig") */
const XMLCh pep::PEP_CONFIG_ELEM[] = UNICODE_LITERAL_9(P,E,P,C,o,n,f,i,g);

/** PEP Config Element ("ApplicationDefaults") */
const XMLCh pep::APPLICATION_DEFAULTS_ELEM[] =
		UNICODE_LITERAL_19(A,p,p,l,i,c,a,t,i,o,n,D,e,f,a,u,l,t,s);

/** PEP Config Element ("ApplicationOverride") */
const XMLCh pep::APPLICATION_OVERRIDE_ELEM[] =
		UNICODE_LITERAL_19(A,p,p,l,i,c,a,t,i,o,n,O,v,e,r,r,i,d,e);

/** PEP Config Element ("Authorization") */
const XMLCh pep::AUTHORIZATION_ELEM[] = UNICODE_LITERAL_13(A,u,t,h,o,r,i,z,a,t,i,o,n);

/** PEP Config Attribute ("Location") */
const XMLCh pep::LOCATION_ATTR[] = UNICODE_LITERAL_8(L,o,c,a,t,i,o,n);

/** PEP Config Element ("Listener") */
const XMLCh pep::LISTENER_ELEM[] = UNICODE_LITERAL_8(L,i,s,t,e,n,e,r);

/** PEP Config Element ("UnixListener") */
const XMLCh pep::UNIX_LISTENER_ELEM[] = UNICODE_LITERAL_12(U,n,i,x,L,i,s,t,e,n,e,r);

/** PEP Config Element ("TCPListener") */
const XMLCh pep::TCP_LISTENER_ELEM[] = UNICODE_LITERAL_11(T,C,P,L,i,s,t,e,n,e,r);

/** PEP Config Attribute ("address") */
const XMLCh pep::ADDRESS_ATTR[] = UNICODE_LITERAL_7(a,d,d,r,e,s,s);

/** PEP Config Attribute ("stackSize") */
const XMLCh pep::STACK_SIZE_ATTR[] = UNICODE_LITERAL_9(s,t,a,c,k,S,i,z,e);

