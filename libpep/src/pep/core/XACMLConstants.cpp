/*
 * XACMLConstants.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: watanabe
 */

#include "XACMLConstants.h"

#include <xercesc/util/XMLUniDefs.hpp>
#include <xmltooling/base.h>

using namespace xacml;
using namespace xercesc;

const XMLCh xacml::XACML_CORE_NS[] = // urn:oasis:names:tc:xacml:3.0:core:schema:wd-17
		{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o, chLatin_a, chLatin_s, chLatin_i,
				chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chColon,
				chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a, chLatin_c, chLatin_m,
				chLatin_l, chColon, chDigit_3, chPeriod, chDigit_0, chColon, chLatin_c, chLatin_o,
				chLatin_r, chLatin_e, chColon, chLatin_s, chLatin_c, chLatin_h, chLatin_e,
				chLatin_m, chLatin_a, chColon, chLatin_w, chLatin_d, chDash, chDigit_1, chDigit_7,
				chNull };

/**  XACML Request Element ("Request") */
const XMLCh xacml::REQUEST_ELEM[] = UNICODE_LITERAL_7(R,e,q,u,e,s,t);

/**  XACML Request Element ("Attributes") */
const XMLCh xacml::ATTRIBUTES_ELEM[] = UNICODE_LITERAL_10(A,t,t,r,i,b,u,t,e,s);

/**  XACML Request Element ("Attribute") */
const XMLCh xacml::ATTRIBUTE_ELEM[] = UNICODE_LITERAL_9(A,t,t,r,i,b,u,t,e);

/**  XACML Request Element ("AttributeValue") */
const XMLCh xacml::ATTRIBUTE_VALUE_ELEM[] = UNICODE_LITERAL_14(A,t,t,r,i,b,u,t,e,V,a,l,u,e);

/**  XACML Request Element ("Category") */
const XMLCh xacml::CATEGORY_ATTR[] = UNICODE_LITERAL_8(C,a,t,e,g,o,r,y);

/**  XACML Request Element ("ReturnPolicyIdList") */
const XMLCh xacml::RETURN_POLICY_ID_LIST_ATTR[] =
		UNICODE_LITERAL_18(R,e,t,u,r,n,P,o,l,i,c,y,I,d,L,i,s,t);

/**  XACML Request Element ("CombinedDecision") */
const XMLCh xacml::COMBINED_DECISION_ATTR[] = UNICODE_LITERAL_16(C,o,m,b,i,n,e,d,D,e,c,i,s,i,o,n);

/**  XACML Request Element ("AttributeId") */
const XMLCh xacml::ATTRIBUTE_ID_ATTR[] = UNICODE_LITERAL_11(A,t,t,r,i,b,u,t,e,I,d);

/**  XACML Request Element ("IncludeInResult") */
const XMLCh xacml::INCLUDE_IN_RESULT_ATTR[] = UNICODE_LITERAL_15(I,n,c,l,u,d,e,I,n,R,e,s,u,l,t);

/**  XACML Request Element ("DataType") */
const XMLCh xacml::DATA_TYPE_ATTR[] = UNICODE_LITERAL_8(D,a,t,a,T,y,p,e);

/**  XACML String data type ("http://www.w3.org/2001/XMLSchema#string") */
const XMLCh xacml::STRING_DATA_TYPE[] = { chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon,
		chForwardSlash, chForwardSlash, chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w,
		chDigit_3, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chDigit_2, chDigit_0,
		chDigit_0, chDigit_1, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chLatin_S, chLatin_c,
		chLatin_h, chLatin_e, chLatin_m, chLatin_a, chPound, chLatin_s, chLatin_t, chLatin_r,
		chLatin_i, chLatin_n, chLatin_g, chNull };

/**  XACML String data type ("http://www.w3.org/2001/XMLSchema#anyURI") */
const XMLCh xacml::ANY_URI_DATA_TYPE[] = { chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon,
		chForwardSlash, chForwardSlash, chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w,
		chDigit_3, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chDigit_2, chDigit_0,
		chDigit_0, chDigit_1, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chLatin_S, chLatin_c,
		chLatin_h, chLatin_e, chLatin_m, chLatin_a, chPound, chLatin_a, chLatin_n, chLatin_y,
		chLatin_U, chLatin_R, chLatin_I, chNull };

/**  XACML String data type ("http://www.w3.org/2001/XMLSchema#base64Binary") */
const XMLCh xacml::BASE64_BINARY_DATA_TYPE[] = { chLatin_h, chLatin_t, chLatin_t, chLatin_p,
		chColon, chForwardSlash, chForwardSlash, chLatin_w, chLatin_w, chLatin_w, chPeriod,
		chLatin_w, chDigit_3, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chDigit_2,
		chDigit_0, chDigit_0, chDigit_1, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chLatin_S,
		chLatin_c, chLatin_h, chLatin_e, chLatin_m, chLatin_a, chPound, chLatin_b, chLatin_a,
		chLatin_s, chLatin_e, chDigit_6, chDigit_4, chLatin_B, chLatin_i, chLatin_n, chLatin_a,
		chLatin_r, chLatin_y, chNull };

/**  XACML Request Element ("urn:oasis:names:tc:xacml:1.0:subject-category:access-subject") */
const XMLCh xacml::SUBJECT_ATTR_CAT[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o,
		chLatin_a, chLatin_s, chLatin_i, chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m,
		chLatin_e, chLatin_s, chColon, chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a,
		chLatin_c, chLatin_m, chLatin_l, chColon, chDigit_1, chPeriod, chDigit_0, chColon,
		chLatin_s, chLatin_u, chLatin_b, chLatin_j, chLatin_e, chLatin_c, chLatin_t, chDash,
		chLatin_c, chLatin_a, chLatin_t, chLatin_e, chLatin_g, chLatin_o, chLatin_r, chLatin_y,
		chColon, chLatin_a, chLatin_c, chLatin_c, chLatin_e, chLatin_s, chLatin_s, chDash,
		chLatin_s, chLatin_u, chLatin_b, chLatin_j, chLatin_e, chLatin_c, chLatin_t, chNull };

/**  XACML Request Element ("urn:oasis:names:tc:xacml:3.0:attribute-category:resource") */
const XMLCh xacml::RESOURCE_ATTR_CAT[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o,
		chLatin_a, chLatin_s, chLatin_i, chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m,
		chLatin_e, chLatin_s, chColon, chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a,
		chLatin_c, chLatin_m, chLatin_l, chColon, chDigit_3, chPeriod, chDigit_0, chColon,
		chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u, chLatin_t,
		chLatin_e, chDash, chLatin_c, chLatin_a, chLatin_t, chLatin_e, chLatin_g, chLatin_o,
		chLatin_r, chLatin_y, chColon, chLatin_r, chLatin_e, chLatin_s, chLatin_o, chLatin_u,
		chLatin_r, chLatin_c, chLatin_e, chNull };

/**  XACML Request Element ("urn:oasis:names:tc:xacml:3.0:attribute-category:action") */
const XMLCh xacml::ACTION_ATTR_CAT[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o,
		chLatin_a, chLatin_s, chLatin_i, chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m,
		chLatin_e, chLatin_s, chColon, chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a,
		chLatin_c, chLatin_m, chLatin_l, chColon, chDigit_3, chPeriod, chDigit_0, chColon,
		chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u, chLatin_t,
		chLatin_e, chDash, chLatin_c, chLatin_a, chLatin_t, chLatin_e, chLatin_g, chLatin_o,
		chLatin_r, chLatin_y, chColon, chLatin_a, chLatin_c, chLatin_t, chLatin_i, chLatin_o,
		chLatin_n, chNull };

//// Attribute Ids ////

/**  XACML Attribute Id ("urn:oasis:names:tc:xacml:1.0:subject:subject-id") */
const XMLCh xacml::SUBJECT_ID_ATTR_ID[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o,
		chLatin_a, chLatin_s, chLatin_i, chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m,
		chLatin_e, chLatin_s, chColon, chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a,
		chLatin_c, chLatin_m, chLatin_l, chColon, chDigit_1, chPeriod, chDigit_0, chColon,
		chLatin_s, chLatin_u, chLatin_b, chLatin_j, chLatin_e, chLatin_c, chLatin_t, chColon,
		chLatin_s, chLatin_u, chLatin_b, chLatin_j, chLatin_e, chLatin_c, chLatin_t, chDash,
		chLatin_i, chLatin_d, chNull };

/**  XACML Attribute Id ("urn:oasis:names:tc:xacml:1.0:resource:resource-id") */
const XMLCh xacml::RESOURCE_ID_ATTR_ID[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o,
		chLatin_a, chLatin_s, chLatin_i, chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m,
		chLatin_e, chLatin_s, chColon, chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a,
		chLatin_c, chLatin_m, chLatin_l, chColon, chDigit_1, chPeriod, chDigit_0, chColon,
		chLatin_r, chLatin_e, chLatin_s, chLatin_o, chLatin_u, chLatin_r, chLatin_c, chLatin_e,
		chColon, chLatin_r, chLatin_e, chLatin_s, chLatin_o, chLatin_u, chLatin_r, chLatin_c,
		chLatin_e, chDash, chLatin_i, chLatin_d, chNull };

/**  XACML Attribute Id ("urn:cvut:fit:swinpro:attribute:action:http:method") */
const XMLCh xacml::HTTP_METHOD_ATTR_ID[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_c,
		chLatin_v, chLatin_u, chLatin_t, chColon, chLatin_f, chLatin_i, chLatin_t, chColon,
		chLatin_s, chLatin_w, chLatin_i, chLatin_n, chLatin_p, chLatin_r, chLatin_o, chColon,
		chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u, chLatin_t,
		chLatin_e, chColon, chLatin_a, chLatin_c, chLatin_t, chLatin_i, chLatin_o, chLatin_n,
		chColon, chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chLatin_m, chLatin_e,
		chLatin_t, chLatin_h, chLatin_o, chLatin_d, chNull };

/**  XACML Attribute Id ("urn:cvut:fit:swinpro:attribute:subject:clientCertificate") */
const XMLCh xacml::CLIENT_CERTIFICATE_ATTR_ID[] = { chLatin_u, chLatin_r, chLatin_n, chColon,
		chLatin_c, chLatin_v, chLatin_u, chLatin_t, chColon, chLatin_f, chLatin_i, chLatin_t,
		chColon, chLatin_s, chLatin_w, chLatin_i, chLatin_n, chLatin_p, chLatin_r, chLatin_o,
		chColon, chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u,
		chLatin_t, chLatin_e, chColon, chLatin_s, chLatin_u, chLatin_b, chLatin_j, chLatin_e,
		chLatin_c, chLatin_t, chColon, chLatin_c, chLatin_l, chLatin_i, chLatin_e, chLatin_n,
		chLatin_t, chLatin_C, chLatin_e, chLatin_r, chLatin_t, chLatin_i, chLatin_f, chLatin_i,
		chLatin_c, chLatin_a, chLatin_t, chLatin_e, chNull };

/**  XACML Response Element ("Response") */
const XMLCh xacml::RESPONSE_ELEM[] = UNICODE_LITERAL_8(R,e,s,p,o,n,s,e);

/**  XACML Response Element ("Result") */
const XMLCh xacml::RESULT_ELEM[] = UNICODE_LITERAL_6(R,e,s,u,l,t);

/**  XACML Response Element ("Decision") */
const XMLCh xacml::DECISION_ELEM[] = UNICODE_LITERAL_8(D,e,c,i,s,i,o,n);

/**  XACML Response Element ("Status") */
const XMLCh xacml::STATUS_ELEM[] = UNICODE_LITERAL_6(S,t,a,t,u,s);

/**  XACML Response Element ("StatusCode") */
const XMLCh xacml::STATUS_CODE_ELEM[] = UNICODE_LITERAL_10(S,t,a,t,u,s,C,o,d,e);

/**  XACML Response Element ("Obligations") */
const XMLCh xacml::OBLIGATIONS_ELEM[] = UNICODE_LITERAL_11(O,b,l,i,g,a,t,i,o,n,s);

/**  XACML Response Element ("Obligation") */
const XMLCh xacml::OBLIGATION_ELEM[] = UNICODE_LITERAL_10(O,b,l,i,g,a,t,i,o,n);

/**  XACML Response Element ("ObligationId") */
const XMLCh xacml::OBLIGATION_ID_ATTR[] = UNICODE_LITERAL_12(O,b,l,i,g,a,t,i,o,n,I,d);

/**  XACML Response Element ("AttributeAssignment") */
const XMLCh xacml::ATTRIBUTE_ASSIGNMENT_ELEM[] =
		UNICODE_LITERAL_19(A,t,t,r,i,b,u,t,e,A,s,s,i,g,n,m,e,n,t);

/**  XACML Response Element ("Permit") */
const XMLCh xacml::PERMIT_VAL[] = UNICODE_LITERAL_6(P,e,r,m,i,t);

/**  XACML Response Element ("Deny") */
const XMLCh xacml::DENY_VAL[] = UNICODE_LITERAL_4(D,e,n,y);

/**  XACML Response Element ("NotApplicable") */
const XMLCh xacml::NOT_APPLICABLE_VAL[] = UNICODE_LITERAL_13(N,o,t,A,p,p,l,i,c,a,b,l,e);

/**  XACML Response Element ("Indeterminate") */
const XMLCh xacml::INDETERMINATE_VAL[] = UNICODE_LITERAL_13(I,n,d,e,t,e,r,m,i,n,a,t,e);

/**  XACML Response Element ("urn:oasis:names:tc:xacml:1.0:status:ok") */
const XMLCh xacml::STATUS_OK_VAL[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_o,
		chLatin_a, chLatin_s, chLatin_i, chLatin_s, chColon, chLatin_n, chLatin_a, chLatin_m,
		chLatin_e, chLatin_s, chColon, chLatin_t, chLatin_c, chColon, chLatin_x, chLatin_a,
		chLatin_c, chLatin_m, chLatin_l, chColon, chDigit_1, chPeriod, chDigit_0, chColon,
		chLatin_s, chLatin_t, chLatin_a, chLatin_t, chLatin_u, chLatin_s, chColon, chLatin_o,
		chLatin_k, chNull };

/**  XACML Request Element ("true") */
const XMLCh xacml::TRUE_VAL[] = UNICODE_LITERAL_4(t,r,u,e);

/**  XACML Request Element ("false") */
const XMLCh xacml::FALSE_VAL[] = UNICODE_LITERAL_5(f,a,l,s,e);

/**  XACML Obligation Id ("urn:cvut:fit:swinpro:obligation:update-remote-user") */
const XMLCh xacml::UPDATE_REMOTE_USER_OBLIG_ID[] = { chLatin_u, chLatin_r, chLatin_n, chColon,
		chLatin_c, chLatin_v, chLatin_u, chLatin_t, chColon, chLatin_f, chLatin_i, chLatin_t,
		chColon, chLatin_s, chLatin_w, chLatin_i, chLatin_n, chLatin_p, chLatin_r, chLatin_o,
		chColon, chLatin_o, chLatin_b, chLatin_l, chLatin_i, chLatin_g, chLatin_a, chLatin_t,
		chLatin_i, chLatin_o, chLatin_n, chColon, chLatin_u, chLatin_p, chLatin_d, chLatin_a,
		chLatin_t, chLatin_e, chDash, chLatin_r, chLatin_e, chLatin_m, chLatin_o, chLatin_t,
		chLatin_e, chDash, chLatin_u, chLatin_s, chLatin_e, chLatin_r, chNull };

/**  XACML Obligation Id ("urn:cvut:fit:swinpro:obligation:redirect") */
const XMLCh xacml::REDIRECT_OBLIG_ID[] = { chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_c,
		chLatin_v, chLatin_u, chLatin_t, chColon, chLatin_f, chLatin_i, chLatin_t, chColon,
		chLatin_s, chLatin_w, chLatin_i, chLatin_n, chLatin_p, chLatin_r, chLatin_o, chColon,
		chLatin_o, chLatin_b, chLatin_l, chLatin_i, chLatin_g, chLatin_a, chLatin_t, chLatin_i,
		chLatin_o, chLatin_n, chColon, chLatin_r, chLatin_e, chLatin_d, chLatin_i, chLatin_r,
		chLatin_e, chLatin_c, chLatin_t, chNull };

