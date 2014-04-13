/*
 * PEPConfig.h
 *
 *  Created on: Mar 30, 2013
 *      Author: watanabe
 */

#ifndef PEPCONFIG_H_
#define PEPCONFIG_H_

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xmltooling/util/Threads.h>
#include <boost/scoped_ptr.hpp>

#include <string>
#include <stdexcept>

#include "remoting/ListenerService.h"
#include "core/PEP.h"

namespace pep {

// Error codes
enum {
	ERROR_ARGS = 1, ERROR_XERCES_INIT, ERROR_PARSE, ERROR_EMPTY_DOCUMENT
};

class PEPConfig {
public:

	enum component_t {
		InProcess = 1, OutOfProcess = 2
	};

	static PEPConfig& getConfig();

	PEPConfig();
	~PEPConfig();

	void initialize(std::string& configPath) throw (std::runtime_error);

	void terminate();

	void registerListenerFactory(std::string factoryName, ListenerServiceFactory factoryFn);

	void setFeatures(unsigned long features);

	bool isEnabled(component_t feature);

	PEP* getPEP() {
		return m_PEPInstance;
	}

private:
	xercesc::XercesDOMParser *m_ConfigFileParser;

	void statConfigFile(std::string& configFile) const throw (std::runtime_error);
	void parseApplicationConfig(xercesc::DOMElement * elemAppSettings);
	void parseListenerServiceConfig(xercesc::DOMElement* elemListener);

	std::map<std::string, ListenerServiceFactory> m_ListenerFactoryMap;

	PEP* m_PEPInstance;
	unsigned long m_Features;

	int m_initCount;
	boost::scoped_ptr<xmltooling::Mutex> m_lock;

};

} /* namespace pep */
#endif /* PEPCONFIG_H_ */
