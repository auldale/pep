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
 * StatusHandler.cpp
 *
 * Handler for exposing information about the internals of the SP.
 */

#include "internal.h"
#include "core/Application.h"
#include "exceptions.h"
#include "handler/RemotedHandler.h"

#include "util/CGIParser.h"

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/scoped_ptr.hpp>
#include <xmltooling/version.h>
#include <xmltooling/util/DateTime.h>
#include "util/SPConstants.h"

#include "core/XACMLConstants.h"
#include "core/AuthorizationConstants.h"


#include "config/PEPConfig.h"
#include "handler/AuthorizationHandler.h"
#ifndef PEP_LITE
#include <xmltooling/security/Credential.h>
#include <xmltooling/security/CredentialCriteria.h>
#include <xsec/enc/OpenSSL/OpenSSLCryptoX509.hpp>
#include <xsec/enc/XSECCryptoException.hpp>
#include <xsec/framework/XSECException.hpp>
#include <xsec/framework/XSECProvider.hpp>
#include "core/XACMLRequestBuilder.h"
#include <xmltooling/impl/AnyElement.h>
#include <xmltooling/soap/SOAP.h>
#include <xmltooling/soap/SOAPClient.h>
#include <xmltooling/soap/HTTPSOAPTransport.h>


using namespace xmlsignature;
#endif
using namespace xmltooling;
using namespace boost;
using namespace std;
using namespace pep;
#ifndef PEP_LITE
using namespace soap11;
#endif

namespace pep {

#ifndef PEP_LITE
class XACMLClient: public soap11::SOAPClient {
public:
	XACMLClient() {
	}
	virtual ~XACMLClient() {
	}
private:
	void prepareTransport(SOAPTransport& transport) {
		transport.setVerifyHost(false);
		HTTPSOAPTransport* http = dynamic_cast<HTTPSOAPTransport*>(&transport);
		if (http) {
			http->useChunkedEncoding(false);
			//http->setRequestHeader(PACKAGE_NAME, PACKAGE_VERSION);
		}
	}
};
#endif

Handler* AuthorizationHandlerFactory(const pair<const DOMElement*, const char*>& p) {
	Category& log = Category::getInstance("AuthorizationHandler");
	log.debug("initialization of Authorization Handler");
	return new AuthorizationHandler(p.first, p.second);
}

#ifndef PEP_LITE
vector<XSECCryptoX509*> g_NoCerts;
#else
vector<string> g_NoCerts;
#endif

static char _x2c(const char *what) {
	register char digit;

	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));
	return (digit);
}

AuthorizationHandler::AuthorizationHandler(const DOMElement* e, const char* appId) :
		AbstractHandler(e, Category::getInstance("AuthorizationHandler"), nullptr, nullptr) {

	string address(appId);
	address += getString("Location").second;
	setAddress(address.c_str());

	m_pdpURL = getString("pdpURL").second;

	// location urcuje selekci handleru mezi apache-inprocess
	// address urcuje selekci handleru mezi inprocess-outprocess

	// register handler by its address in PEP
	PEPConfig& conf = PEPConfig::getConfig();
	conf.getPEP()->regListener(m_address.c_str(), this);

}

pair<bool, long> AuthorizationHandler::run(PEPRequest& request, bool isHandler) const {

	try {
		if (PEPConfig::getConfig().isEnabled(PEPConfig::OutOfProcess)) {
			// When out of process, we run natively and directly process the message.
			return processAuthzRequest(request.getApplication(), request, request);
		} else {
			// When not out of process, we remote all the message processing.
			DDF out, in = wrap(request, nullptr, true);
			DDFJanitor jin(in), jout(out);
			out = request.getPEP().getListenerService()->send(in);
			return unwrap(request, out);
		}
	} catch (XMLToolingException& ex) {
		m_log.error("error while processing request: %s", ex.what());

		return make_pair(true, 0L);
	} catch (std::exception& ex) {
		m_log.error("error while processing request: %s", ex.what());

		return make_pair(false, 0L);
	}
}

void AuthorizationHandler::receive(DDF& in, ostream& out) {
	// Find application.
	const char* aid = in["application_id"].string();
	const Application* app = aid ? PEPConfig::getConfig().getPEP()->getApplication(aid) : nullptr;
	if (!app) {
		// Something's horribly wrong.
		m_log.error("couldn't find application (%s) for status request", aid ? aid : "(missing)");
		throw ConfigurationException("Unable to locate application for status request, deleted?");
	}

	// Wrap a response shim.
	DDF ret(nullptr);
	DDFJanitor jout(ret);
	scoped_ptr<HTTPRequest> req(getRequest(in));
	scoped_ptr<HTTPResponse> resp(getResponse(ret));

	// Since we're remoted, the result should either be a throw, a false/0 return,
	// which we just return as an empty structure, or a response/redirect,
	// which we capture in the facade and send back.
	processAuthzRequest(*app, *req, *resp);
	out << ret;
}

pair<bool, long> AuthorizationHandler::processAuthzRequest(const Application& application,
		const HTTPRequest& httpRequest, HTTPResponse& httpResponse) const {


#ifndef PEP_LITE
	m_log.debug("processing status request");

	XACMLRequestBuilder builder;

	/////////// subject ////////////
	//// subjectId
	boost::shared_ptr<XACMLRequestBuilder::Attribute> subjIdAttr = builder.addAttribute(
			xacml::SUBJECT_ATTR_CAT, xacml::SUBJECT_ID_ATTR_ID, true);

	builder.addAttributeValue(subjIdAttr, httpRequest.getRemoteUser().c_str(),
			xacml::STRING_DATA_TYPE);

	//// clientCert
	vector<XSECCryptoX509*> certs = httpRequest.getClientCertificates();
	if (!certs.empty()) {

		XSECCryptoX509* cert = certs[0];
		const char* raw_cert = cert->getDEREncodingSB().rawCharBuffer();
		boost::shared_ptr<XACMLRequestBuilder::Attribute> clientCertAttr = builder.addAttribute(
				xacml::SUBJECT_ATTR_CAT, xacml::CLIENT_CERTIFICATE_ATTR_ID, false);
		builder.addAttributeValue(clientCertAttr, raw_cert, xacml::BASE64_BINARY_DATA_TYPE);
	}

	/////////// resource ////////////
	boost::shared_ptr<XACMLRequestBuilder::Attribute> resIdAttr = builder.addAttribute(
			xacml::RESOURCE_ATTR_CAT, xacml::RESOURCE_ID_ATTR_ID, true);
	builder.addAttributeValue(resIdAttr, httpRequest.getRequestURL(), xacml::ANY_URI_DATA_TYPE);

	/////////// action ////////////
	boost::shared_ptr<XACMLRequestBuilder::Attribute> httpMethodAttr = builder.addAttribute(
			xacml::ACTION_ATTR_CAT, xacml::HTTP_METHOD_ATTR_ID, true);
	builder.addAttributeValue(httpMethodAttr, httpRequest.getMethod(), xacml::STRING_DATA_TYPE);

	ElementProxy* xacmlReq = builder.getDocument();

	scoped_ptr<Envelope> env(EnvelopeBuilder::buildEnvelope());
	Body* body = BodyBuilder::buildBody();
	env->setBody(body);
	// ElementProxy* msg = new AnyElementImpl(shibspconstants::SHIB2SPNOTIFY_NS, LogoutNotification);
	body->getUnknownXMLObjects().push_back(xacmlReq);

	bool result = true;
	XACMLClient soapClient;
	Envelope* response;

	try {
		soapClient.send(*env,
				SOAPTransport::Address(application.getId(), application.getId(), m_pdpURL.c_str()));

		response = soapClient.receive();

		processAuthzResponse(response->getBody(), httpResponse);
	} catch (std::exception& ex) {
		Category::getInstance("AuthorizationHandler").error(
				"Error occured while invoking soap client: %s", ex.what());
		result = false;
	}
	soapClient.reset();

	httpResponse.setContentType("text/xml");
#endif
	return make_pair(false, 0L);
}

void AuthorizationHandler::processUpdateRemoteUserObligation(xercesc::DOMElement* obligElem,
		HTTPResponse& httpResponse) const {

	DOMNodeList* assignList = obligElem->getElementsByTagName(xacml::ATTRIBUTE_ASSIGNMENT_ELEM);
	const XMLSize_t nodeCount = assignList->getLength();

	for (XMLSize_t xx = 0; xx < nodeCount; ++xx) {

		DOMNode* currentNode = assignList->item(xx);

		if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element
			DOMElement* currentElem = dynamic_cast<xercesc::DOMElement*>(currentNode);
			const char* textContent = XMLString::transcode(currentElem->getTextContent());

			m_log.debug("Obligation remote-user value: %s", textContent);
			httpResponse.setResponseHeader(PEP_HEADER_OBLIGATION_REMOTE_USER, textContent);
			break;
		}

	}
}

void AuthorizationHandler::processRedirectObligation(xercesc::DOMElement* obligElem,
		HTTPResponse& httpResponse) const {

	DOMNodeList* assignList = obligElem->getElementsByTagName(xacml::ATTRIBUTE_ASSIGNMENT_ELEM);
	const XMLSize_t nodeCount = assignList->getLength();

	for (XMLSize_t xx = 0; xx < nodeCount; ++xx) {

		DOMNode* currentNode = assignList->item(xx);

		if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element
			DOMElement* currentElem = dynamic_cast<xercesc::DOMElement*>(currentNode);
			const char* textContent = XMLString::transcode(currentElem->getTextContent());

			m_log.debug("Obligation redirect value: %s", textContent);
			httpResponse.setResponseHeader(PEP_HEADER_OBLIGATION_REDIRECT, textContent);
			break;
		}

	}
}

void AuthorizationHandler::processAuthzResponse(ElementProxy* xacmlResponse,
		HTTPResponse& httpResponse) const {
	DOMElement* bodyElem = xacmlResponse->getDOM();
	DOMNodeList* children = bodyElem->getElementsByTagName(xacml::RESPONSE_ELEM);

	if (children->getLength() < 1) {
		m_log.error("Missing Response element");
		return;
	}

	DOMNode* responseNode = children->item(0);

	if (!responseNode->getNodeType() || responseNode->getNodeType() != DOMNode::ELEMENT_NODE) { // is element
		m_log.error("Invalid response node type");
		return;
	}

	DOMElement* responseElement = dynamic_cast<xercesc::DOMElement*>(responseNode);

	children = responseElement->getElementsByTagName(xacml::RESULT_ELEM);

	if (children->getLength() < 1) {
		m_log.error("Missing Result element");
		return;
	}

	if (children->getLength() > 1) {
		m_log.error("Multiple Result elements are not supported yet.");
		return;
	}

	DOMNode* resultNode = children->item(0);

	if (!resultNode->getNodeType() || resultNode->getNodeType() != DOMNode::ELEMENT_NODE) { // is element
		m_log.error("Invalid result node type");
		return;
	}

	DOMElement* resultElement = dynamic_cast<xercesc::DOMElement*>(resultNode);
	children = resultElement->getChildNodes();
	const XMLSize_t nodeCount = children->getLength();

	for (XMLSize_t xx = 0; xx < nodeCount; ++xx) {

		DOMNode* currentNode = children->item(xx);

		if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element

		// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast<xercesc::DOMElement*>(currentNode);

			if (XMLString::equals(currentElement->getTagName(), xacml::DECISION_ELEM)) {
				m_log.debug("Processing decision");
				const XMLCh* xmlch_Decision = currentElement->getTextContent();

				// Deny-biased PEP
				if (XMLString::equals(xmlch_Decision, xacml::PERMIT_VAL)) {
					httpResponse.setResponseHeader(PEP_HEADER_DECISION, PEP_HEADER_VAL_PERMIT);
				} else {
					httpResponse.setResponseHeader(PEP_HEADER_DECISION, PEP_HEADER_VAL_DENY);
				}

			} else if (XMLString::equals(currentElement->getTagName(), xacml::STATUS_ELEM)) {
				m_log.debug("Processing status");
				DOMNode * statusCodeNode = currentElement->getElementsByTagName(
						xacml::STATUS_CODE_ELEM)->item(0);

				if (!statusCodeNode->getNodeType()
						&& statusCodeNode->getNodeType() != DOMNode::ELEMENT_NODE) { // is element
					continue;
				}

				DOMElement * statusCodeElem = dynamic_cast<xercesc::DOMElement*>(currentNode);

				const XMLCh* xmlch_StatusCode = statusCodeElem->getTextContent();

				if (XMLString::equals(xmlch_StatusCode, xacml::STATUS_OK_VAL)) {
					httpResponse.setResponseHeader(PEP_HEADER_STATUS, PEP_HEADER_VAL_OK);
				} else {
					httpResponse.setResponseHeader(PEP_HEADER_STATUS, PEP_HEADER_VAL_ERROR);
				}
			} else if (XMLString::equals(currentElement->getTagName(), xacml::OBLIGATIONS_ELEM)) {
				m_log.debug("Processing obligations");

				DOMNodeList* obligationChildren = currentElement->getChildNodes();

				const XMLSize_t obligChildrenNC = obligationChildren->getLength();

				for (XMLSize_t yy = 0; yy < obligChildrenNC; ++yy) {

					DOMNode* obligNode = obligationChildren->item(yy);

					if (obligNode->getNodeType()
							&& obligNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element
						DOMElement* obligElem = dynamic_cast<xercesc::DOMElement*>(obligNode);
						const XMLCh* obligId = obligElem->getAttribute(xacml::OBLIGATION_ID_ATTR);
						m_log.error("Obligation Node, id: %s", XMLString::transcode(obligId));

						if (XMLString::equals(obligId, xacml::UPDATE_REMOTE_USER_OBLIG_ID)) {
							m_log.debug("Processing set-remote-user obligation");
							processUpdateRemoteUserObligation(obligElem, httpResponse);
						}

						if (XMLString::equals(obligId, xacml::REDIRECT_OBLIG_ID)) {
							m_log.debug("Processing redirect obligation");
							processRedirectObligation(obligElem, httpResponse);
						}
					}

				}
			}
		}

	}
}
}
