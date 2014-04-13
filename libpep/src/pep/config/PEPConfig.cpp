/*
 * PEPConfig.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: watanabe
 */

#include "PEPConfig.h"

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <xmltooling/util/XMLHelper.h>
#include <xmltooling/util/Threads.h>
#include "core/XMLApplication.h"
#include "config/ConfigConstants.h"

using namespace boost;
using namespace xercesc;
using namespace std;
using namespace xmltooling;

namespace pep {

PEPConfig globalConfigInstance;

PEPConfig& PEPConfig::getConfig() {
	//static PEPConfig config_instance;
	return globalConfigInstance;
}

PEPConfig::PEPConfig() :
		m_PEPInstance(nullptr), m_Features(0), m_initCount(0), m_lock(Mutex::create()) {
	try {
		XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
	} catch (XMLException& e) {
		char* message = XMLString::transcode(e.getMessage());
		cerr << "XML toolkit initialization error: " << message << endl;
		XMLString::release(&message);
		// throw exception here to return ERROR_XERCES_INIT
	}

	m_ConfigFileParser = new XercesDOMParser;
}

PEPConfig::~PEPConfig() {

	delete m_ConfigFileParser;

	// Terminate Xerces
	try {
		XMLPlatformUtils::Terminate();  // Terminate after release of memory
	} catch (xercesc::XMLException& e) {
		char* message = xercesc::XMLString::transcode(e.getMessage());

		cerr << "XML toolkit teardown error: " << message << endl;
		XMLString::release(&message);
	}
}

void PEPConfig::statConfigFile(string& configFile) const throw (std::runtime_error) {

	struct stat fileStatus;

	int iretStat = stat(configFile.c_str(), &fileStatus);
	if (iretStat == ENOENT)
		throw(std::runtime_error("Path file_name does not exist, or path is an empty string."));
	else if (iretStat == ENOTDIR)
		throw(std::runtime_error("A component of the path is not a directory."));
	else if (iretStat == ELOOP)
		throw(std::runtime_error("Too many symbolic links encountered while traversing the path."));
	else if (iretStat == EACCES)
		throw(std::runtime_error("Permission denied."));
	else if (iretStat == ENAMETOOLONG)
		throw(std::runtime_error("File can not be read\n"));
}

void PEPConfig::registerListenerFactory(string factoryName, ListenerServiceFactory factory) {
	m_ListenerFactoryMap[factoryName] = factory;
}

void PEPConfig::setFeatures(unsigned long features) {
	m_Features = features;
}

bool PEPConfig::isEnabled(PEPConfig::component_t feature) {
	return m_Features & feature;
}

void PEPConfig::initialize(string& configFile) throw (std::runtime_error) {

	Lock initLock(m_lock);

	if (m_initCount == INT_MAX) {
		Category::getInstance("PEPConfig").crit("library initialized too many times");
		return;
	}

	if (m_initCount >= 1) {
		++m_initCount;
		return;
	}

	const char* logconf = getenv("SHIBSP_LOGGING");

	string logpath = configFile.substr(0, configFile.find_last_of('/'));
	if (!logconf || !*logconf) {
		if (isEnabled(PEPConfig::OutOfProcess) && !isEnabled(PEPConfig::InProcess))
			logconf = logpath.append("/pepd.logger").c_str();
		else if (isEnabled(PEPConfig::InProcess) && !isEnabled(PEPConfig::OutOfProcess))
			logconf = logpath.append("/mod_pep.logger").c_str();
		else
			logconf = logpath.append("/pepd.logger").c_str();
	}

	XMLToolingConfig::getConfig().log_config(logconf);

	Category& log = Category::getInstance("Config");
	log.debug("library initialization started");

	XMLToolingConfig::getConfig().user_agent = string("PEP");

	if (!XMLToolingConfig::getConfig().init()) {
		log.fatal("failed to initialize XMLTooling library");
		throw(std::runtime_error("failed to initialize XMLTooling library"));
	}

	if (m_PEPInstance) {
		delete m_PEPInstance;
	}
	m_PEPInstance = new PEP();

	statConfigFile(configFile);

	// register listener service factories
	registerListenerServices();

	// Configure DOM parser.
	m_ConfigFileParser->setValidationScheme(XercesDOMParser::Val_Never);
	m_ConfigFileParser->setDoNamespaces(false);
	m_ConfigFileParser->setDoSchema(false);
	m_ConfigFileParser->setLoadExternalDTD(false);

	try {
		m_ConfigFileParser->parse(configFile.c_str());

		// no need to free this pointer - owned by the parent parser object
		DOMDocument* xmlDoc = m_ConfigFileParser->getDocument();

		// Get the top-level element: NAme is "root". No attributes for "root"

		DOMElement* elementRoot = xmlDoc->getDocumentElement();
		if (!elementRoot)
			throw(std::runtime_error("empty XML document"));

		// Parse XML file for tags of interest: "ApplicationSettings"
		// Look one level nested within "root". (child of root)

		DOMNodeList* children = elementRoot->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();

		// For all nodes, children of "root" in the XML tree.

		for (XMLSize_t xx = 0; xx < nodeCount; ++xx) {

			DOMNode* currentNode = children->item(xx);

			if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element

			// Found node which is an Element. Re-cast node as element
				DOMElement* currentElement = dynamic_cast<xercesc::DOMElement*>(currentNode);

				if (XMLString::equals(currentElement->getTagName(), APPLICATION_DEFAULTS_ELEM)) {
					parseApplicationConfig(currentElement);
				} else if (XMLString::equals(currentElement->getTagName(), LISTENER_ELEM)) {
					parseListenerServiceConfig(currentElement);
				}
			}
		}
	} catch (xercesc::XMLException& e) {
		char* message = xercesc::XMLString::transcode(e.getMessage());
		ostringstream errBuf;
		errBuf << "Error parsing file: " << message << flush;
		XMLString::release(&message);
	}
	++m_initCount;
}

void PEPConfig::terminate() {
	Lock initLock(m_lock);
	if (m_initCount == 0) {
		Category::getInstance("PEPConfig").crit("term without corresponding init");
		return;
	} else if (--m_initCount > 0) {
		return;
	}

}

void PEPConfig::parseApplicationConfig(DOMElement* defAppElem) {
	Category& log = Category::getInstance("Application");

	////////////////////////
	boost::shared_ptr<XMLApplication> defapp(new XMLApplication(m_PEPInstance, defAppElem));

	m_PEPInstance->regApplication(defapp);

	DOMElement* child;

	// Load any overrides.
	child = XMLHelper::getFirstChildElement(defAppElem, APPLICATION_OVERRIDE_ELEM);
	while (child) {
		boost::shared_ptr<XMLApplication> iapp(
				new XMLApplication(m_PEPInstance, child, defapp.get()));
		if (m_PEPInstance->getApplication(iapp->getId()))
			log.crit(
					"found conf:ApplicationOverride element with duplicate id attribute (%s), skipping it",
					iapp->getId());
		else
			m_PEPInstance->regApplication(defapp);

		child = XMLHelper::getNextSiblingElement(child, APPLICATION_OVERRIDE_ELEM);
	}
}

void PEPConfig::parseListenerServiceConfig(DOMElement* elemListener) {
	const DOMNodeList * children = elemListener->getChildNodes();

	for (XMLSize_t idx = 0; idx < children->getLength(); ++idx) {
		DOMNode* currentNode = children->item(idx);
		if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element
			DOMElement* currentElement = dynamic_cast<xercesc::DOMElement*>(currentNode);
			const XMLCh * xmlch_elementName = currentElement->getTagName();
			char * elementName = XMLString::transcode(xmlch_elementName);
			map<string, ListenerServiceFactory>::iterator it_res = m_ListenerFactoryMap.find(
					string(elementName));
			if (it_res != m_ListenerFactoryMap.end()) {
				ListenerServiceFactory factory = (*it_res).second;
				m_PEPInstance->setListenerServiceFactory(factory, currentElement);
			}

		}
	}

}

} /* namespace pep */

