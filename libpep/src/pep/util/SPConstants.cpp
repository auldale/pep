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
 * SPConstants.cpp
 * 
 * Shibboleth SP XML constants.
 */

#include "../internal.h"
#include "SPConstants.h"
#include <xercesc/util/XMLUniDefs.hpp>

using namespace pepconstants;

const XMLCh pepconstants::SHIB1_PROTOCOL_ENUM[] = // urn:mace:shibboleth:1.0
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_1, chPeriod, chDigit_0, chNull
};

const XMLCh pepconstants::SHIBMD_NS[] = // urn:mace:shibboleth:metadata:1.0
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chLatin_m, chLatin_e, chLatin_t, chLatin_a, chLatin_d, chLatin_a, chLatin_t, chLatin_a, chColon,
  chDigit_1, chPeriod, chDigit_0, chNull
};

const XMLCh pepconstants::SHIBMD_PREFIX[] = UNICODE_LITERAL_6(s,h,i,b,m,d);

const XMLCh pepconstants::SHIB2SPCONFIG_NS[] = // urn:mace:shibboleth:2.0:native:sp:config
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon, chLatin_n, chLatin_a, chLatin_t, chLatin_i, chLatin_v, chLatin_e, chColon,
  chLatin_s, chLatin_p, chColon, chLatin_c, chLatin_o, chLatin_n, chLatin_f, chLatin_i, chLatin_g, chNull
};

const XMLCh pepconstants::SHIB2SPPROTOCOLS_NS[] = // urn:mace:shibboleth:2.0:native:sp:protocols
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon, chLatin_n, chLatin_a, chLatin_t, chLatin_i, chLatin_v, chLatin_e, chColon,
  chLatin_s, chLatin_p, chColon, chLatin_p, chLatin_r, chLatin_o, chLatin_t, chLatin_o, chLatin_c, chLatin_o, chLatin_l, chLatin_s, chNull
};

const XMLCh pepconstants::SHIB2ATTRIBUTEMAP_NS[] = // urn:mace:shibboleth:2.0:attribute-map
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon,
  chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u, chLatin_t, chLatin_e, chDash,
  chLatin_m, chLatin_a, chLatin_p, chNull
};

const XMLCh pepconstants::SHIB2SPNOTIFY_NS[] = // urn:mace:shibboleth:2.0:sp:notify
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon, chLatin_s, chLatin_p, chColon,
  chLatin_n, chLatin_o, chLatin_t, chLatin_i, chLatin_f, chLatin_y, chNull
};

const XMLCh pepconstants::SHIB2ATTRIBUTEFILTER_NS[] = // urn:mace:shibboleth:2.0:afp
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon, chLatin_a, chLatin_f, chLatin_p, chNull
};

const XMLCh pepconstants::SHIB2ATTRIBUTEFILTER_MF_BASIC_NS[] = // urn:mace:shibboleth:2.0:afp:mf:basic
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon, chLatin_a, chLatin_f, chLatin_p, chColon, chLatin_m, chLatin_f, chColon,
  chLatin_b, chLatin_a, chLatin_s, chLatin_i, chLatin_c, chNull
};

const XMLCh pepconstants::SHIB2ATTRIBUTEFILTER_MF_SAML_NS[] = // urn:mace:shibboleth:2.0:afp:mf:saml
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon, chLatin_a, chLatin_f, chLatin_p, chColon, chLatin_m, chLatin_f, chColon,
  chLatin_s, chLatin_a, chLatin_m, chLatin_l, chNull
};

const XMLCh pepconstants::SHIB1_ATTRIBUTE_NAMESPACE_URI[] = // urn:mace:shibboleth:1.0:attributeNamespace:uri
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_1, chPeriod, chDigit_0, chColon,
  chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u, chLatin_t, chLatin_e,
    chLatin_N, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_u, chLatin_r, chLatin_i, chNull
};

const XMLCh pepconstants::SHIB1_NAMEID_FORMAT_URI[] = // urn:mace:shibboleth:1.0:nameIdentifier
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_1, chPeriod, chDigit_0, chColon,
  chLatin_n, chLatin_a, chLatin_m, chLatin_e,
    chLatin_I, chLatin_d, chLatin_e, chLatin_n, chLatin_t, chLatin_i, chLatin_f, chLatin_i, chLatin_e, chLatin_r, chNull
};

const XMLCh pepconstants::SHIB1_AUTHNREQUEST_PROFILE_URI[] = // urn:mace:shibboleth:1.0:profiles:AuthnRequest
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_1, chPeriod, chDigit_0, chColon,
  chLatin_p, chLatin_r, chLatin_o, chLatin_f, chLatin_i, chLatin_l, chLatin_e, chLatin_s, chColon,
  chLatin_A, chLatin_u, chLatin_t, chLatin_h, chLatin_n,
  chLatin_R, chLatin_e, chLatin_q, chLatin_u, chLatin_e, chLatin_s, chLatin_t, chNull
};

const XMLCh pepconstants::SHIB2_BINDING_FILE[] = // urn:mace:shibboleth:2.0:bindings:File
{ chLatin_u, chLatin_r, chLatin_n, chColon, chLatin_m, chLatin_a, chLatin_c, chLatin_e, chColon,
  chLatin_s, chLatin_h, chLatin_i, chLatin_b, chLatin_b, chLatin_o, chLatin_l, chLatin_e, chLatin_t, chLatin_h, chColon,
  chDigit_2, chPeriod, chDigit_0, chColon,
  chLatin_b, chLatin_i, chLatin_n, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chLatin_s, chColon,
  chLatin_F, chLatin_i, chLatin_l, chLatin_e, chNull
};

const char pepconstants::SHIB1_SESSIONINIT_PROFILE_URI[] = "urn:mace:shibboleth:sp:1.3:SessionInit";

const char pepconstants::SHIB1_LOGOUT_PROFILE_URI[] = "urn:mace:shibboleth:sp:1.3:Logout";

const char pepconstants::ASCII_SHIB2SPCONFIG_NS[] = "urn:mace:shibboleth:2.0:native:sp:config";
