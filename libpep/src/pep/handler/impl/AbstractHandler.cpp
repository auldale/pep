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
 * AbstractHandler.cpp
 *
 * Base class for handlers based on a DOMPropertySet.
 */

#include "internal.h"
#include "exceptions.h"
#include "core/Application.h"
#include "handler/AbstractHandler.h"
#include "remoting/ListenerService.h"


#include <vector>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <xmltooling/XMLToolingConfig.h>
#include <xmltooling/util/PathResolver.h>
#include <xmltooling/util/URLEncoder.h>


# include <xmltooling/security/Credential.h>
# include <xmltooling/security/CredentialResolver.h>
# include <xmltooling/util/StorageService.h>



#include <xmltooling/XMLToolingConfig.h>
#include <xmltooling/util/URLEncoder.h>


using namespace xmltooling;
using namespace xercesc;
using namespace boost;
using namespace std;
using namespace pep;

namespace pep {

    void SHIBSP_DLLLOCAL generateRandomHex(std::string& buf, unsigned int len) {
        static char DIGITS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        int r;
        unsigned char b1,b2;
        buf.erase();
        for (unsigned int i=0; i<len; i+=4) {
            r = rand();
            b1 = (0x00FF & r);
            b2 = (0xFF00 & r)  >> 8;
            buf += (DIGITS[(0xF0 & b1) >> 4 ]);
            buf += (DIGITS[0x0F & b1]);
            buf += (DIGITS[(0xF0 & b2) >> 4 ]);
            buf += (DIGITS[0x0F & b2]);
        }
    }
};


Handler::Handler()
{
}

Handler::~Handler()
{
}


const XMLCh* Handler::getProtocolFamily() const
{
    return nullptr;
}

void Handler::log(PEPRequest::SPLogLevel level, const string& msg) const
{
    Category::getInstance(SHIBSP_LOGCAT".Handler").log(
        (level == PEPRequest::SPDebug ? Priority::DEBUG :
        (level == PEPRequest::SPInfo ? Priority::INFO :
        (level == PEPRequest::SPWarn ? Priority::WARN :
        (level == PEPRequest::SPError ? Priority::ERROR : Priority::CRIT)))),
        msg
        );
}

AbstractHandler::AbstractHandler(
    const DOMElement* e, Category& log, DOMNodeFilter* filter, const map<string,string>* remapper
    ) : m_log(log), m_configNS(pepconstants::SHIB2SPCONFIG_NS) {
    load(e, nullptr, filter, remapper);
}

AbstractHandler::~AbstractHandler()
{
}

void AbstractHandler::log(PEPRequest::SPLogLevel level, const string& msg) const
{
    m_log.log(
        (level == PEPRequest::SPDebug ? Priority::DEBUG :
        (level == PEPRequest::SPInfo ? Priority::INFO :
        (level == PEPRequest::SPWarn ? Priority::WARN :
        (level == PEPRequest::SPError ? Priority::ERROR : Priority::CRIT)))),
        msg
        );
}

#ifndef PEP_LITE

const char* Handler::getType() const
{
    return getString("type").second;
}


#endif


pair<bool,bool> AbstractHandler::getBool(const char* name, const PEPRequest& request, unsigned int type) const
{
    if (type & HANDLER_PROPERTY_REQUEST) {
        const char* param = request.getParameter(name);
        if (param && *param)
            return make_pair(true, (*param=='t' || *param=='1'));
    }
    
/*    if (type & HANDLER_PROPERTY_MAP) {
        pair<bool,bool> ret = request.getRequestSettings().first->getBool(name);
        if (ret.first)
            return ret;
    }*/

    if (type & HANDLER_PROPERTY_FIXED) {
        return getBool(name);
    }

    return make_pair(false,false);
}

pair<bool,const char*> AbstractHandler::getString(const char* name, const PEPRequest& request, unsigned int type) const
{
    if (type & HANDLER_PROPERTY_REQUEST) {
        const char* param = request.getParameter(name);
        if (param && *param)
            return make_pair(true, param);
    }
    
/*    if (type & HANDLER_PROPERTY_MAP) {
        pair<bool,const char*> ret = request.getRequestSettings().first->getString(name);
        if (ret.first)
            return ret;
    }*/

    if (type & HANDLER_PROPERTY_FIXED) {
        return getString(name);
    }

    return pair<bool,const char*>(false,nullptr);
}

pair<bool,unsigned int> AbstractHandler::getUnsignedInt(const char* name, const PEPRequest& request, unsigned int type) const
{
    if (type & HANDLER_PROPERTY_REQUEST) {
        const char* param = request.getParameter(name);
        if (param && *param) {
            try {
                return pair<bool,unsigned int>(true, lexical_cast<unsigned int>(param));
            }
            catch (bad_lexical_cast&) {
                return pair<bool,unsigned int>(false,0);
            }
        }
    }
    
/*    if (type & HANDLER_PROPERTY_MAP) {
        pair<bool,unsigned int> ret = request.getRequestSettings().first->getUnsignedInt(name);
        if (ret.first)
            return ret;
    }*/

    if (type & HANDLER_PROPERTY_FIXED) {
        return getUnsignedInt(name);
    }

    return pair<bool,unsigned int>(false,0);
}

pair<bool,int> AbstractHandler::getInt(const char* name, const PEPRequest& request, unsigned int type) const
{
    if (type & HANDLER_PROPERTY_REQUEST) {
        const char* param = request.getParameter(name);
        if (param && *param)
            return pair<bool,int>(true, atoi(param));
    }

    if (type & HANDLER_PROPERTY_FIXED) {
        return getInt(name);
    }

    return pair<bool,int>(false,0);
}
