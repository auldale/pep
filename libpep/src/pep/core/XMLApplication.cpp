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
 * XMLServiceProvider.cpp
 *
 * XML-based SP configuration and mgmt.
 */

#include "internal.h"
#include "exceptions.h"
#include "config/PEPConfig.h"
#include "core/PEP.h"
#include "remoting/ListenerService.h"
#include "util/DOMPropertySet.h"
#include "util/SPConstants.h"

#if defined(XMLTOOLING_LOG4SHIB)
# include <log4shib/PropertyConfigurator.hh>
#elif defined(XMLTOOLING_LOG4CPP)
# include <log4cpp/PropertyConfigurator.hh>
#else
# error "Supported logging library not available."
#endif
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>

#include <xmltooling/XMLToolingConfig.h>
#include <xmltooling/version.h>
#include <xmltooling/util/NDC.h>
#include <xmltooling/util/ReloadableXMLFile.h>
#include <xmltooling/util/TemplateEngine.h>
#include <xmltooling/util/Threads.h>
#include <xmltooling/util/XMLHelper.h>
#ifndef PEP_LITE
# include <xmltooling/security/ChainingTrustEngine.h>
# include <xmltooling/security/CredentialResolver.h>
# include <xmltooling/security/SecurityHelper.h>
# include <xmltooling/util/ReplayCache.h>
# include <xmltooling/util/StorageService.h>

# include <xsec/utils/XSECPlatformUtils.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>
#endif
#include "XMLApplication.h"
#include "config/ConfigConstants.h"

#include "handler/Handler.h"
#include "handler/AuthorizationHandler.h"

using namespace xmltooling;
using namespace boost;
using namespace std;
using namespace pep;

//Handler* AuthorizationHandlerFactory(const pair<const DOMElement*, const char*>& p);

XMLApplication::XMLApplication(const pep::PEP* pep,
//    const ProtocolProvider* pp,
		DOMElement* e, const XMLApplication* base) :
		Application(pep), m_base(base) {
#ifdef _DEBUG
	xmltooling::NDC ndc("XMLApplication");
#endif
	Category& log = Category::getInstance("Application");

	// First load any property sets.
	//map<string, string> remapper;
	load(e, nullptr, this, nullptr);

	//if (conf.isEnabled(SPConfig::Handlers))
	doHandlers(e, log);

	// Assign parent.
	if (base)
		setParent(base);

}

XMLApplication::~XMLApplication() {

}


DOMNodeFilter::FilterAction XMLApplication::acceptNode(const DOMNode* node) const {
	const XMLCh* name = node->getNodeName();
	if (XMLString::equals(name, APPLICATION_OVERRIDE_ELEM) ||
			XMLString::equals(name, AUTHORIZATION_ELEM)

	)
		return FILTER_REJECT;

	return FILTER_ACCEPT;
}

void XMLApplication::clearHeader(PEPRequest& request, const char* rawname,
		const char* cginame) const {
	if (!m_attributePrefix.first.empty()) {
		string temp = m_attributePrefix.first + rawname;
		string temp2 = m_attributePrefix.second + (cginame + 5);
		request.clearHeader(temp.c_str(), temp2.c_str());
	} else if (m_base) {
		m_base->clearHeader(request, rawname, cginame);
	} else {
		request.clearHeader(rawname, cginame);
	}
}

void XMLApplication::setHeader(PEPRequest& request, const char* name, const char* value) const {
	if (!m_attributePrefix.first.empty()) {
		string temp = m_attributePrefix.first + name;
		request.setHeader(temp.c_str(), value);
	} else if (m_base) {
		m_base->setHeader(request, name, value);
	} else {
		request.setHeader(name, value);
	}
}

string XMLApplication::getSecureHeader(const PEPRequest& request, const char* name) const {
	if (!m_attributePrefix.first.empty()) {
		string temp = m_attributePrefix.first + name;
		return request.getSecureHeader(temp.c_str());
	} else if (m_base) {
		return m_base->getSecureHeader(request, name);
	} else {
		return request.getSecureHeader(name);
	}
}

void XMLApplication::doHandlers(const DOMElement* e, Category& log) {

	pair<bool, const char*> location;


	// Found node which is an Element. Re-cast node as element
	DOMNode * child = e->getFirstChild();

	while (child) {

		if (!child->getNodeType() || child->getNodeType() != DOMNode::ELEMENT_NODE) { // is element
			child = XMLHelper::getNextSiblingElement(child);
			continue;
		}

		DOMElement* childElem = dynamic_cast<xercesc::DOMElement*>(child);
		if (!childElem->hasAttributeNS(nullptr, LOCATION_ATTR)) {
			auto_ptr_char hclass(child->getLocalName());
			log.error("%s handler with no Location property cannot be processed", hclass.get());
			child = XMLHelper::getNextSiblingElement(child);
			continue;
		}
		try {
			boost::shared_ptr<Handler> handler;

			if (XMLString::equals(childElem->getNodeName(), AUTHORIZATION_ELEM)) {
				log.debug("Found Authorization Handler");
				pair<const DOMElement*, const char*> handlerCfg(childElem, getId());
				Handler * newHandler = AuthorizationHandlerFactory(handlerCfg);
				handler.reset(newHandler);
			}else {
				log.error("Unrecognized Handler type, skipping it...");
						child = XMLHelper::getNextSiblingElement(child);
				                    continue;
			}

			m_handlers.push_back(handler);

			// Insert into location map.
			location = handler->getString("Location");
			if (location.first && *location.second == '/')
				m_handlerMap[location.second] = handler.get();
			else if (location.first)
				m_handlerMap[string("/") + location.second] = handler.get();

		} catch (std::exception& ex) {
			log.error("caught exception processing handler element: %s", ex.what());
		}

		child = XMLHelper::getNextSiblingElement(child);
	}
}


const Handler* XMLApplication::getHandler(const char* path) const {
string wrap(path);
wrap = wrap.substr(0, wrap.find(';'));
map<string, const Handler*>::const_iterator i = m_handlerMap.find(wrap.substr(0, wrap.find('?')));
if (i != m_handlerMap.end())
	return i->second;
return m_base ? m_base->getHandler(path) : nullptr;
}

void XMLApplication::getHandlers(vector<const Handler*>& handlers) const {
static void (vector<const Handler*>::*pb)(
		const Handler* const &) = &vector<const Handler*>::push_back;
for_each(m_handlers.begin(), m_handlers.end(),
		boost::bind(pb, boost::ref(handlers), boost::bind(&boost::shared_ptr<Handler>::get, _1)));
if (m_base) {
	for (map<string, const Handler*>::const_iterator h = m_base->m_handlerMap.begin();
			h != m_base->m_handlerMap.end(); ++h) {
		if (m_handlerMap.count(h->first) == 0)
			handlers.push_back(h->second);
	}
}
}
