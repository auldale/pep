/*
 * XACMLConstants.h
 *
 *  Created on: Apr 20, 2013
 *      Author: watanabe
 */

#ifndef XACMLCONSTANTS_H_
#define XACMLCONSTANTS_H_

#include <xercesc/util/XercesDefs.hpp>


namespace xacml {

/** ("urn:oasis:names:tc:xacml:3.0:core:schema:wd-17") */
extern const XMLCh XACML_CORE_NS[];

/**  XACML Request Element ("Request") */
extern const XMLCh REQUEST_ELEM[];

/**  XACML Request Element ("Attributes") */
extern const XMLCh ATTRIBUTES_ELEM[];

/**  XACML Request Element ("Attribute") */
extern const XMLCh ATTRIBUTE_ELEM[];

/**  XACML Request Element ("AttributeValue") */
extern const XMLCh ATTRIBUTE_VALUE_ELEM[];

/**  XACML Request Element ("Category") */
extern const XMLCh CATEGORY_ATTR[];

/**  XACML Request Element ("ReturnPolicyIdList") */
extern const XMLCh RETURN_POLICY_ID_LIST_ATTR[];

/**  XACML Request Element ("CombinedDecision") */
extern const XMLCh COMBINED_DECISION_ATTR[];

/**  XACML Request Element ("AttributeId") */
extern const XMLCh ATTRIBUTE_ID_ATTR[];

/**  XACML Request Element ("IncludeInResult") */
extern const XMLCh INCLUDE_IN_RESULT_ATTR[];

/**  XACML Request Element ("DataType") */
extern const XMLCh DATA_TYPE_ATTR[];

/**  XACML String data type ("http://www.w3.org/2001/XMLSchema#string") */
extern const XMLCh STRING_DATA_TYPE[];

/**  XACML String data type ("http://www.w3.org/2001/XMLSchema#anyURI") */
extern const XMLCh ANY_URI_DATA_TYPE[];

/**  XACML String data type ("http://www.w3.org/2001/XMLSchema#base64Binary") */
extern const XMLCh BASE64_BINARY_DATA_TYPE[];

/**  XACML Request Element ("urn:oasis:names:tc:xacml:1.0:subject-category:access-subject") */
extern const XMLCh SUBJECT_ATTR_CAT[];

/**  XACML Request Element ("urn:oasis:names:tc:xacml:3.0:attribute-category:resource") */
extern const XMLCh RESOURCE_ATTR_CAT[];

/**  XACML Request Element ("urn:oasis:names:tc:xacml:3.0:attribute-category:action") */
extern const XMLCh ACTION_ATTR_CAT[];

//// Attribute Ids ////

/**  XACML Attribute Id ("urn:oasis:names:tc:xacml:1.0:subject:subject-id") */
extern const XMLCh SUBJECT_ID_ATTR_ID[];

/**  XACML Attribute Id ("urn:oasis:names:tc:xacml:1.0:resource:resource-id") */
extern const XMLCh RESOURCE_ID_ATTR_ID[];

/**  XACML Attribute Id ("urn:cvut:fit:swinpro:attribute:action:http:method") */
extern const XMLCh HTTP_METHOD_ATTR_ID[];

/**  XACML Attribute Id ("urn:cvut:fit:swinpro:attribute:subject:clientCertificate") */
extern const XMLCh CLIENT_CERTIFICATE_ATTR_ID[];


//// response ////
/**  XACML Response Element ("Response") */
extern const XMLCh RESPONSE_ELEM[];

/**  XACML Response Element ("Result") */
extern const XMLCh RESULT_ELEM[];

/**  XACML Response Element ("Decision") */
extern const XMLCh DECISION_ELEM[];

/**  XACML Response Element ("Status") */
extern const XMLCh STATUS_ELEM[];

/**  XACML Response Element ("StatusCode") */
extern const XMLCh STATUS_CODE_ELEM[];

/**  XACML Response Element ("Obligations") */
extern const XMLCh OBLIGATIONS_ELEM[];

/**  XACML Response Element ("Obligations") */
extern const XMLCh OBLIGATION_ELEM[];

/**  XACML Response Element ("ObligationId") */
extern const XMLCh OBLIGATION_ID_ATTR[];


/**  XACML Response Element ("AttributeAssignment") */
extern const XMLCh ATTRIBUTE_ASSIGNMENT_ELEM[];


/**  XACML Response Element ("Permit") */
extern const XMLCh PERMIT_VAL[];

/**  XACML Response Element ("Deny") */
extern const XMLCh DENY_VAL[];

/**  XACML Response Element ("NotApplicable") */
extern const XMLCh NOT_APPLICABLE_VAL[];

/**  XACML Response Element ("Indeterminate") */
extern const XMLCh INDETERMINATE_VAL[];

/**  XACML Response Element ("urn:oasis:names:tc:xacml:1.0:status:ok") */
extern const XMLCh STATUS_OK_VAL[];


//// misc ////

/**  XACML Request Element ("true") */
extern const XMLCh TRUE_VAL[];

/**  XACML Request Element ("false") */
extern const XMLCh FALSE_VAL[];


//// swinpro namespace ////
/**  XACML Obligation Id ("urn:cvut:fit:swinpro:obligation:update-remote-user") */
extern const XMLCh UPDATE_REMOTE_USER_OBLIG_ID[];

/**  XACML Obligation Id ("urn:cvut:fit:swinpro:obligation:redirect") */
extern const XMLCh REDIRECT_OBLIG_ID[];


}
/* namespace xacml */
#endif /* XACMLCONSTANTS_H_ */
