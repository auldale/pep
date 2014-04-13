/*
 * PEP.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: watanabe
 */

#include "PEP.h"
#include <xercesc/dom/DOMElement.hpp>
#include <string>
#include <boost/shared_ptr.hpp>
#include "exceptions.h"
#include "internal.h"
#include "base.h"
#include <exception>
#include "core/Application.h"
#include "core/PEPRequest.h"
#include "handler/Handler.h"

using namespace boost;
using namespace xercesc;
using namespace std;

namespace pep {

PEP::PEP() :
		m_ListenerService(nullptr) {
}

PEP::~PEP() {
	if (m_ListenerService) {
		delete m_ListenerService;
	}
}

bool PEP::validate() const {
	if (!m_ListenerService) {
		return false;
	}

	if (!getApplication("default")) {
		return false;
	}

	return true;
}

const Application* PEP::getApplication(const char* applicationId) const {
	map<string, boost::shared_ptr<Application> >::const_iterator i = m_AppMap.find(
			applicationId ? applicationId : "default");
	return (i != m_AppMap.end()) ? i->second.get() : nullptr;
}

void PEP::regApplication(boost::shared_ptr<Application> application) {

	string appId;
	if (application.get()->getId()) {
		appId = application.get()->getId();
	} else {
		appId = "default";
	}

	m_AppMap[appId] = application;

}

void PEP::setListenerServiceFactory(ListenerServiceFactory factoryFn,
		const DOMElement* const & config) {
	m_ListenerService = factoryFn(config);
}

pair<bool, long> PEP::doAuthorization(PEPRequest& request) const {
#ifdef _DEBUG
	xmltooling::NDC ndc("doAuthentication");
#endif


    Category& log = Category::getInstance("PEP");

    log.debug("Request URL: %s",request.getRequestURL());

    log.debug("Request REMOTE_USER: %s",request.getRemoteUser().c_str());

//    const Application* app = nullptr;
//    string targetURL = request.getRequestURL();

	try {
		return doHandler(request);

	} catch (...) {
		return make_pair(true, 500);
	}
}

pair<bool, long> PEP::doHandler(PEPRequest& request) const {
#ifdef _DEBUG
	xmltooling::NDC ndc("doHandler");
#endif
	Category& log = Category::getInstance("PEP");

	const Application* app = nullptr;
	string targetURL = request.getRequestURL();

	try {
		//RequestMapper::Settings settings = request.getRequestSettings();
		app = &(request.getApplication());

		const char* handlerURL = request.getHandlerURL(targetURL.c_str());
		if (!handlerURL)
			throw ConfigurationException(
					"Cannot determine handler from resource URL, check configuration.");

		// We dispatch based on our path info. We know the request URL begins with or equals the handler URL,
		// so the path info is the next character (or null).
		//const Handler* handler = app->getHandler(targetURL.c_str() + strlen(handlerURL));
		const Handler* handler = app->getHandler(handlerURL);
		if (!handler)
			throw ConfigurationException("Shibboleth handler invoked at an unconfigured location.");

		pair<bool, long> hret = handler->run(request);

		// Did the handler run successfully?
		if (hret.first)
			return hret;
		throw ConfigurationException(
				"Configured PEP handler failed to process the request.");
	} catch (...) {
		return make_pair(true, 0);
	}
}

Remoted* PEP::regListener(const char* address, Remoted* listener) {
	Remoted* ret = nullptr;
	map<string, Remoted*>::const_iterator i = m_listenerMap.find(address);
	if (i != m_listenerMap.end())
		ret = i->second;
	m_listenerMap[address] = listener;
	Category::getInstance(SHIBSP_LOGCAT".ServiceProvider").info(
			"registered remoted message endpoint (%s)", address);
	return ret;
}

bool PEP::unregListener(const char* address, Remoted* current, Remoted* restore) {
	map<string, Remoted*>::const_iterator i = m_listenerMap.find(address);
	if (i != m_listenerMap.end() && i->second == current) {
		if (restore)
			m_listenerMap[address] = restore;
		else
			m_listenerMap.erase(address);
		Category::getInstance(SHIBSP_LOGCAT".ServiceProvider").info(
				"unregistered remoted message endpoint (%s)", address);
		return true;
	}
	return false;
}

Remoted* PEP::lookupListener(const char *address) const {
	map<string, Remoted*>::const_iterator i = m_listenerMap.find(address);
	return (i == m_listenerMap.end()) ? nullptr : i->second;
}

} /* namespace pep */
