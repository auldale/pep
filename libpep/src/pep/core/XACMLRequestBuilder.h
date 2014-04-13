/*
 * XACMLRequestBuilder.h
 *
 *  Created on: Apr 20, 2013
 *      Author: watanabe
 */

#ifndef XACMLREQUESTBUILDER_H_
#define XACMLREQUESTBUILDER_H_

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <xmltooling/ElementProxy.h>
#include <xercesc/util/XMLString.hpp>

namespace pep {

class XACMLRequestBuilder {

public:

	struct AttributeValue {
		const XMLCh* m_DataType;
		std::string m_Value;
	};

	struct Attribute {
		const XMLCh* m_Id;
		bool includeInResult;
		std::vector<AttributeValue> m_Values;
	};

	// TODO add into constructor  	 <Request CombinedDecision="false" ReturnPolicyIdList="true" xmlns="urn:oasis:names:tc:xacml:3.0:core:schema:wd-17">

	XACMLRequestBuilder(bool returnPolicyIdList = true, bool combinedDescision = false);
	virtual ~XACMLRequestBuilder();

	boost::shared_ptr<Attribute> addAttribute(const XMLCh* category, const XMLCh* attributeId,
			bool includeInResult = false);

	void addAttributeValue(boost::shared_ptr<Attribute> attribute, std::string value,
			const XMLCh* dataType);

	xmltooling::ElementProxy* getDocument();

private:

	struct CompareXMLString {
		bool operator()(const XMLCh* s1, const XMLCh* s2) const {
			return xercesc::XMLString::compareString(s1, s2) < 0;
		}
	};

	std::map<const XMLCh*, std::map<const XMLCh*, boost::shared_ptr<Attribute>, CompareXMLString>,
			CompareXMLString> m_AttributeMap;

	bool m_ReturnPolicyIdList;
	bool m_CombinedDescision;

};

} /* namespace pep */
#endif /* XACMLREQUESTBUILDER_H_ */
