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
 * AbstractSPRequest.cpp
 *
 * Abstract base for SPRequest implementations.
 */

#include "internal.h"
#include "exceptions.h"
#include "core/AbstractPEPRequest.h"
#include "core/Application.h"
#include "config/PEPConfig.h"

/*
#include "GSSRequest.h"
#include "ServiceProvider.h"
#include "SessionCache.h"
*/

#include "util/CGIParser.h"

#include <boost/lexical_cast.hpp>

using namespace xmltooling;
using namespace std;
using namespace pep;

PEPRequest::PEPRequest()
{
}

PEPRequest::~PEPRequest()
{
}

string PEPRequest::getSecureHeader(const char* name) const
{
    return getHeader(name);
}

void PEPRequest::setAuthType(const char* authtype)
{
}


AbstractPEPRequest::AbstractPEPRequest(const char* category)
    : m_PEP(PEPConfig::getConfig().getPEP()),
        m_app(nullptr),
        m_log(&Category::getInstance(category))
{
    //m_sp->lock();
}

AbstractPEPRequest::~AbstractPEPRequest()
{
    /*if (m_session)
        m_session->unlock();
    if (m_mapper)
        m_mapper->unlock();
    if (m_sp)
        m_sp->unlock();*/
}

/*const ServiceProvider& AbstractSPRequest::getServiceProvider() const
{
    return *m_sp;
}*/


const PEP& AbstractPEPRequest::getPEP() const
{
    return *m_PEP;
}

/*RequestMapper::Settings AbstractPEPRequest::getRequestSettings() const
{
    if (!m_mapper) {
        // Map request to application and content settings.
        m_mapper = m_PEP->getRequestMapper();
        m_mapper->lock();
        m_settings = m_mapper->getSettings(*this);

        if (reinterpret_cast<Category*>(m_log)->isDebugEnabled()) {
            reinterpret_cast<Category*>(m_log)->debug(
                "mapped %s to %s", getRequestURL(), m_settings.first->getString("applicationId").second
                );
        }
    }
    return m_settings;
}*/

/*
Settings AbstractPEPRequest::getRequestSettings() const
{
    if (!m_mapper) {
        // Map request to application and content settings.
        m_mapper = m_PEP->getRequestMapper();
        m_mapper->lock();
        m_settings = m_mapper->getSettings(*this);

        if (reinterpret_cast<Category*>(m_log)->isDebugEnabled()) {
            reinterpret_cast<Category*>(m_log)->debug(
                "mapped %s to %s", getRequestURL(), m_settings.first->getString("applicationId").second
                );
        }
    }
	//getParameter

    return pair<const PropertySet*,void*>(nullptr,nullptr);
}*/

const Application& AbstractPEPRequest::getApplication() const
{

    if (!m_app) {
        // Now find the application from the URL settings
        m_app = m_PEP->getApplication(getRequestSettings().first->getString("applicationId").second);
        if (!m_app)
            throw ConfigurationException("Unable to map non-default applicationId to an ApplicationOverride, check configuration.");
    }
    return *m_app;
}

/*Session* AbstractSPRequest::getSession(bool checkTimeout, bool ignoreAddress, bool cache)
{
    // Only attempt this once.
    if (cache && m_sessionTried)
        return m_session;
    else if (cache)
        m_sessionTried = true;

    // Need address checking and timeout settings.
    time_t timeout = 3600;
    if (checkTimeout || !ignoreAddress) {
        const PropertySet* props = getApplication().getPropertySet("Sessions");
        if (props) {
            if (checkTimeout) {
                pair<bool,unsigned int> p = props->getUnsignedInt("timeout");
                if (p.first)
                    timeout = p.second;
            }
            pair<bool,bool> pcheck = props->getBool("consistentAddress");
            if (pcheck.first)
                ignoreAddress = !pcheck.second;
        }
    }

    // The cache will either silently pass a session or nullptr back, or throw an exception out.
    Session* session = getServiceProvider().getSessionCache()->find(
        getApplication(), *this, (ignoreAddress ? nullptr : getRemoteAddr().c_str()), (checkTimeout ? &timeout : nullptr)
        );
    if (cache)
        m_session = session;
    return session;
}*/

static char _x2c(const char *what)
{
    register char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
    return(digit);
}

void AbstractPEPRequest::setRequestURI(const char* uri)
{
    // Fix for bug 574, secadv 20061002
    // Unescape URI up to query string delimiter by looking for %XX escapes.
    // Adapted from Apache's util.c, ap_unescape_url function.
    if (uri) {
        while (*uri) {
            if (*uri == '?') {
                m_uri += uri;
                break;
            }
            else if (*uri != '%') {
                m_uri += *uri;
            }
            else {
                ++uri;
                if (!isxdigit(*uri) || !isxdigit(*(uri+1)))
                    throw ConfigurationException("Bad request, contained unsupported encoded characters.");
                m_uri += _x2c(uri);
                ++uri;
            }
            ++uri;
        }
    }
}

const char* AbstractPEPRequest::getRequestURI() const
{
    return m_uri.c_str();
}

const char* AbstractPEPRequest::getRequestURL() const
{
    if (m_url.empty()) {
        // Compute the full target URL
        int port = getPort();
        const char* scheme = getScheme();
        m_url = string(scheme) + "://" + getHostname();
        if (!isDefaultPort())
            m_url += ":" + boost::lexical_cast<string>(port);
        m_url += m_uri;
    }
    return m_url.c_str();
}

string AbstractPEPRequest::getRemoteAddr() const
{
    pair<bool,const char*> addr = getRequestSettings().first->getString("REMOTE_ADDR");
    return addr.first ? getHeader(addr.second) : "";
}

const char* AbstractPEPRequest::getParameter(const char* name) const
{
    if (!m_parser.get())
        m_parser.reset(new CGIParser(*this));

    pair<CGIParser::walker,CGIParser::walker> bounds = m_parser->getParameters(name);
    return (bounds.first==bounds.second) ? nullptr : bounds.first->second;
}

vector<const char*>::size_type AbstractPEPRequest::getParameters(const char* name, vector<const char*>& values) const
{
    if (!m_parser.get())
        m_parser.reset(new CGIParser(*this));

    pair<CGIParser::walker,CGIParser::walker> bounds = m_parser->getParameters(name);
    while (bounds.first != bounds.second) {
        values.push_back(bounds.first->second);
        ++bounds.first;
    }
    return values.size();
}

//const char* AbstractPEPRequest::getHandlerURL(const char* resource) const
//{
//    if (!resource)
//        resource = getRequestURL();
//
//    if (!m_handlerURL.empty() && resource && !strcmp(getRequestURL(), resource))
//        return m_handlerURL.c_str();
//
//    // Check for relative URL.
//    string stackresource;
//    if (resource && *resource == '/') {
//        // Compute a URL to the root of the site and point resource at constructed string.
//        int port = getPort();
//        const char* scheme = getScheme();
//        stackresource = string(scheme) + "://" + getHostname();
//        if (!isDefaultPort())
//            stackresource += ":" + boost::lexical_cast<string>(port);
//        stackresource += resource;
//        resource = stackresource.c_str();
//    }
//
///*
//#ifdef HAVE_STRCASECMP
//    if (!resource || (strncasecmp(resource,"http://",7) && strncasecmp(resource,"https://",8)))
//#else
//*/
//    if (!resource || (strncasecmp(resource,"http://",7) && strncasecmp(resource,"https://",8)))
////#endif
//        throw ConfigurationException("Target resource was not an absolute URL.");
//
//    bool ssl_only = true;
//    const char* handler = nullptr;
//    const PropertySet* props = getApplication().getPropertySet("Sessions");
//    if (props) {
//        pair<bool,bool> p = props->getBool("handlerSSL");
//        if (p.first)
//            ssl_only = p.second;
//        pair<bool,const char*> p2 = props->getString("handlerURL");
//        if (p2.first)
//            handler = p2.second;
//    }
//
//    if (!handler) {
//        handler = "/Shibboleth.sso";
//    }
//    else if (*handler!='/' && strncmp(handler,"http:",5) && strncmp(handler,"https:",6)) {
//        throw ConfigurationException(
//            "Invalid handlerURL property ($1) in <Sessions> element for Application ($2)",
//            params(2, handler ? handler : "null", m_app->getId())
//            );
//    }
//
//    // The "handlerURL" property can be in one of three formats:
//    //
//    // 1) a full URI:       http://host/foo/bar
//    // 2) a hostless URI:   http:///foo/bar
//    // 3) a relative path:  /foo/bar
//    //
//    // #  Protocol  Host        Path
//    // 1  handler   handler     handler
//    // 2  handler   resource    handler
//    // 3  resource  resource    handler
//    //
//    // note: if ssl_only is true, make sure the protocol is https
//
//    const char* path = nullptr;
//
//    // Decide whether to use the handler or the resource for the "protocol"
//    const char* prot;
//    if (*handler != '/') {
//        prot = handler;
//    }
//    else {
//        prot = resource;
//        path = handler;
//    }
//
//    // break apart the "protocol" string into protocol, host, and "the rest"
//    const char* colon = strchr(prot, ':');
//    colon += 3;
//    const char* slash = strchr(colon, '/');
//    if (!path)
//        path = slash;
//
//    // Compute the actual protocol and store in member.
//    if (ssl_only)
//        m_handlerURL.assign("https://");
//    else
//        m_handlerURL.assign(prot, colon-prot);
//
//    // create the "host" from either the colon/slash or from the target string
//    // If prot == handler then we're in either #1 or #2, else #3.
//    // If slash == colon then we're in #2.
//    if (prot != handler || slash == colon) {
//        colon = strchr(resource, ':');
//        colon += 3;      // Get past the ://
//        slash = strchr(colon, '/');
//    }
//    string host(colon, (slash ? slash-colon : strlen(colon)));
//
//    // Build the handler URL
//    m_handlerURL += host + path;
//    return m_handlerURL.c_str();
//}

void AbstractPEPRequest::log(SPLogLevel level, const std::string& msg) const
{
    reinterpret_cast<Category*>(m_log)->log(
        (level == SPDebug ? Priority::DEBUG :
        (level == SPInfo ? Priority::INFO :
        (level == SPWarn ? Priority::WARN :
        (level == SPError ? Priority::ERROR : Priority::CRIT)))),
        msg
        );
}

bool AbstractPEPRequest::isPriorityEnabled(SPLogLevel level) const
{
    return reinterpret_cast<Category*>(m_log)->isPriorityEnabled(
        (level == SPDebug ? Priority::DEBUG :
        (level == SPInfo ? Priority::INFO :
        (level == SPWarn ? Priority::WARN :
        (level == SPError ? Priority::ERROR : Priority::CRIT))))
        );
}
