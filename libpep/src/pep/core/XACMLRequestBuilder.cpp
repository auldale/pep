/*
 * XACMLRequestBuilder.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: watanabe
 */

#include "XACMLRequestBuilder.h"
#include <xmltooling/impl/AnyElement.h>
#include "core/XACMLConstants.h"
#include <xercesc/util/XMLString.hpp>
#include <xmltooling/logging.h>

using namespace std;
using namespace boost;
using namespace xmltooling;
using namespace xmltooling::logging;
using namespace xercesc;

namespace pep {

XACMLRequestBuilder::XACMLRequestBuilder(bool returnPolicyIdList, bool combinedDescision) :
		m_ReturnPolicyIdList(returnPolicyIdList), m_CombinedDescision(combinedDescision) {

}

XACMLRequestBuilder::~XACMLRequestBuilder() {

}

boost::shared_ptr<XACMLRequestBuilder::Attribute> XACMLRequestBuilder::addAttribute(
		const XMLCh* category, const XMLCh* attributeId, bool includeInResult) {

	Category& log = Category::getInstance("XACMLRequestBuilder");

	log.debug("Adding attribute: attributeId={%s}, category={%s}" , category, attributeId);

	map<const XMLCh*, boost::shared_ptr<Attribute>, XACMLRequestBuilder::CompareXMLString> categoryMap =
			m_AttributeMap[category];

	map<const XMLCh*, boost::shared_ptr<Attribute> >::iterator it = categoryMap.find(attributeId);
	if (it != categoryMap.end()) {
		// todo attribute already exsists
		return it->second;
	}

	boost::shared_ptr<Attribute> attr = boost::shared_ptr<Attribute>(new Attribute);
	attr->m_Id = attributeId;
	attr->includeInResult = includeInResult;

	categoryMap[attributeId] = attr;

	m_AttributeMap[category] = categoryMap;
	return attr;
}

void XACMLRequestBuilder::addAttributeValue(boost::shared_ptr<Attribute> attribute, string value,
		const XMLCh* dataType) {

	AttributeValue attrVal;
	attrVal.m_Value = value;
	attrVal.m_DataType = dataType;

	attribute->m_Values.push_back(attrVal);

}

ElementProxy* XACMLRequestBuilder::getDocument() {

	ElementProxy* requestElem = new AnyElementImpl(xacml::XACML_CORE_NS, xacml::REQUEST_ELEM);

	//body->getUnknownXMLObjects().push_back(msg);
	// set combined descision
	requestElem->setAttribute(xmltooling::QName(nullptr, xacml::COMBINED_DECISION_ATTR),
			m_CombinedDescision ? xacml::TRUE_VAL : xacml::FALSE_VAL);
	requestElem->setAttribute(xmltooling::QName(nullptr, xacml::RETURN_POLICY_ID_LIST_ATTR),
			m_ReturnPolicyIdList ? xacml::TRUE_VAL : xacml::FALSE_VAL);

	std::map<const XMLCh*,
			std::map<const XMLCh*, boost::shared_ptr<Attribute>,
					XACMLRequestBuilder::CompareXMLString>, XACMLRequestBuilder::CompareXMLString>::iterator categoryIter;

	for (categoryIter = m_AttributeMap.begin(); categoryIter != m_AttributeMap.end();
			categoryIter++) {

		Category& log = Category::getInstance("XACMLRequestBuilder");

		ElementProxy* attributesElem = new AnyElementImpl(xacml::XACML_CORE_NS,
				xacml::ATTRIBUTES_ELEM);
		requestElem->getUnknownXMLObjects().push_back(attributesElem);

		// todo memory leak v XMLSTring::transcode
		const XMLCh* categoryVal = categoryIter->first;
		attributesElem->setAttribute(xmltooling::QName(nullptr, xacml::CATEGORY_ATTR), categoryVal);

		std::map<const XMLCh*, boost::shared_ptr<Attribute>, XACMLRequestBuilder::CompareXMLString> categoryMap =
				categoryIter->second;
		std::map<const XMLCh*, boost::shared_ptr<Attribute>, XACMLRequestBuilder::CompareXMLString>::iterator attributeIter;
		for (attributeIter = categoryMap.begin(); attributeIter != categoryMap.end();
				attributeIter++) {

			ElementProxy* attributeElem = new AnyElementImpl(xacml::XACML_CORE_NS,
					xacml::ATTRIBUTE_ELEM);
			attributesElem->getUnknownXMLObjects().push_back(attributeElem);

			//XMLCh* attributeIdVal = XMLString::transcode(attributeIter->second->m_Id.c_str());
			attributeElem->setAttribute(xmltooling::QName(nullptr, xacml::ATTRIBUTE_ID_ATTR),
					attributeIter->second->m_Id);

			attributeElem->setAttribute(xmltooling::QName(nullptr, xacml::INCLUDE_IN_RESULT_ATTR),
					attributeIter->second->includeInResult ? xacml::TRUE_VAL : xacml::FALSE_VAL);

			vector<AttributeValue>::iterator attrValsIter;
			for (attrValsIter = attributeIter->second->m_Values.begin();
					attrValsIter != attributeIter->second->m_Values.end(); attrValsIter++) {

				ElementProxy* attributeValElem = new AnyElementImpl(xacml::XACML_CORE_NS,
						xacml::ATTRIBUTE_VALUE_ELEM);
				attributeElem->getUnknownXMLObjects().push_back(attributeValElem);

				//XMLCh* dataType = XMLString::transcode(attrValsIter->m_DataType.c_str());
				attributeValElem->setAttribute(xmltooling::QName(nullptr, xacml::DATA_TYPE_ATTR),
						attrValsIter->m_DataType);

				XMLCh* attrValue = XMLString::transcode(attrValsIter->m_Value.c_str());
				attributeValElem->setTextContent(attrValue);
			}

		}

	}

	return requestElem;

}

} /* namespace pep */
