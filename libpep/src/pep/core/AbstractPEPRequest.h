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
 * @file shibsp/AbstractSPRequest.h
 * 
 * Abstract base for SPRequest implementations.
 */

#ifndef __shibsp_abstreq_h__
#define __shibsp_abstreq_h__

#include <core/PEPRequest.h>
#include <boost/scoped_ptr.hpp>

namespace pep {
class PEP;
    
class CGIParser;

    /**
     * Abstract base for SPRequest implementations
     */
    class AbstractPEPRequest : public virtual PEPRequest
    {
    protected:
        /**
         * Constructor.
         *
         * @param category  logging category to use
         */
    	AbstractPEPRequest(const char* category);
        
        /**
         * Stores a normalized request URI to ensure it contains no %-encoded characters
         * or other undesirable artifacts.
         *
         * @param uri   the request URI as obtained from the client
         */
        void setRequestURI(const char* uri);

    public:
        virtual ~AbstractPEPRequest();

        // Virtual function overrides.
        const pep::PEP& getPEP() const;
        virtual Settings getRequestSettings() const=0;
        const Application& getApplication() const;
        //Session* getSession(bool checkTimeout=true, bool ignoreAddress=false, bool cache=true);
        const char* getRequestURI() const;
        const char* getRequestURL() const;
        std::string getRemoteAddr() const;
        const char* getParameter(const char* name) const;
        std::vector<const char*>::size_type getParameters(const char* name, std::vector<const char*>& values) const;
        virtual const char* getHandlerURL(const char* resource=nullptr) const=0;
        void log(SPLogLevel level, const std::string& msg) const;
        bool isPriorityEnabled(SPLogLevel level) const;

    private:
        pep::PEP* m_PEP;
/*        mutable RequestMapper* m_mapper;
        mutable RequestMapper::Settings m_settings;*/
        mutable const Application* m_app;
        //mutable bool m_sessionTried;
        //mutable Session* m_session;
        std::string m_uri;
        mutable std::string m_url;
        void* m_log; // declared void* to avoid log4cpp header conflicts in Apache
        //mutable std::string m_handlerURL;
        mutable boost::scoped_ptr<CGIParser> m_parser;
    };



};

#endif /* __shibsp_abstreq_h__ */
