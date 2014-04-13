/**
 * Licensed to the University Corporation for Advanced Internet
 * Development, Inc. (UCAID) under one or more contributor license
 * agreements. See the NOTICE file distributed with this work for
 * additional information regarding copyright ownership.
 *
 * UCAID licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the
 * License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

/**
 * ListenerService.cpp
 *
 * Interprocess remoting engine.
 */

#include "../../internal.h"
#include "../../exceptions.h"
#include "../ListenerService.h"
#include "../../config/PEPConfig.h"

#include <xercesc/dom/DOM.hpp>
#ifndef PEP_LITE
#include <xmltooling/security/SecurityHelper.h>
#endif

using namespace xmltooling;
using namespace xercesc;
using namespace std;
using namespace pep;


// TODO this is nasty bad thing exposing implementation in interface:
#include "UnixListener.h"
//#include "TCPListener.h"



/*namespace shibsp {
 SHIBSP_DLLLOCAL PluginManager<ListenerService,string,const DOMElement*>::Factory TCPListenerServiceFactory;
 #ifndef WIN32
 SHIBSP_DLLLOCAL PluginManager<ListenerService,string,const DOMElement*>::Factory UnixListenerServiceFactory;
 #endif
 };*/

void pep::registerListenerServices() {

	PEPConfig& conf = PEPConfig::getConfig();
	//conf.registerListenerFactory(TCP_LISTENER_SERVICE, &TCPListenerServiceFactory);
	conf.registerListenerFactory(UNIX_LISTENER_SERVICE, &UnixListener::UnixListenerServiceFactory);
}

Remoted::Remoted() {
}

Remoted::~Remoted() {
}

ListenerService::ListenerService() {
}

ListenerService::~ListenerService() {
}

Remoted* ListenerService::regListener(const char* address, Remoted* listener) {
	Remoted* ret = nullptr;
	map<string, Remoted*>::const_iterator i = m_listenerMap.find(address);
	if (i != m_listenerMap.end())
		ret = i->second;
	m_listenerMap[address] = listener;
	Category::getInstance(SHIBSP_LOGCAT".Listener").info("registered remoted message endpoint (%s)",
			address);
	return ret;
}

bool ListenerService::unregListener(const char* address, Remoted* current, Remoted* restore) {
	map<string, Remoted*>::const_iterator i = m_listenerMap.find(address);
	if (i != m_listenerMap.end() && i->second == current) {
		if (restore)
			m_listenerMap[address] = restore;
		else
			m_listenerMap.erase(address);
		Category::getInstance(SHIBSP_LOGCAT".Listener").info(
				"unregistered remoted message endpoint (%s)", address);
		return true;
	}
	return false;
}

Remoted* ListenerService::lookup(const char *address) const {
	map<string, Remoted*>::const_iterator i = m_listenerMap.find(address);
	return (i == m_listenerMap.end()) ? nullptr : i->second;
}

void ListenerService::receive(DDF &in, ostream& out) {
	if (!in.name())
		throw ListenerException("Incoming message with no destination address rejected.");
	else if (!strcmp("ping", in.name())) {
		DDF outmsg = DDF(nullptr).integer(in.integer() + 1);
		DDFJanitor jan(outmsg);
		out << outmsg;
		return;
	} else if (!strcmp("hash", in.name())) {
#ifndef PEP_LITE
		const char* hashAlg = in["alg"].string();
		const char* data = in["data"].string();
		if (!hashAlg || !*hashAlg || !data || !*data)
			throw ListenerException("Hash request missing algorithm or data parameters.");
		DDF outmsg(nullptr);
		DDFJanitor jan(outmsg);
		outmsg.string(SecurityHelper::doHash(hashAlg, data, strlen(data)).c_str());
		out << outmsg;
		return;
#else
		throw ListenerException("Hash algorithms unavailable in lite build of library.");
#endif
	}

	// Two stage lookup, on the listener itself, and the SP interface.
	    PEP* pep = PEPConfig::getConfig().getPEP();
	// Locker locker(pep);
	Remoted* dest = lookup(in.name());
	if (!dest) {
	 dest = pep->lookupListener(in.name());
	 if (!dest)
	 throw ListenerException("No destination registered for incoming message addressed to ($1).", params(1,in.name()));
	 }

	dest->receive(in, out);
}

bool ListenerService::init(bool force) {
	return true;
}

void ListenerService::term() {
}
