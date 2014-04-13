/*
 * AuthorizationHandler.h
 *
 *  Created on: Apr 2, 2013
 *      Author: watanabe
 */

#ifndef AUTHORIZATIONHANDLER_H_
#define AUTHORIZATIONHANDLER_H_


#include "internal.h"
//#include "core/Application.h"
#include "exceptions.h"
#include "handler/RemotedHandler.h"
#include "handler/AbstractHandler.h"

#include "util/CGIParser.h"

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/scoped_ptr.hpp>
#include <xmltooling/version.h>
#include <xmltooling/util/DateTime.h>
#include <xmltooling/ElementProxy.h>


using namespace xmltooling;
using namespace boost;
using namespace std;

namespace pep {

class Application;

    class AuthorizationHandler : public AbstractHandler, public RemotedHandler
    {
    public:
    	AuthorizationHandler(const DOMElement* e, const char* appId);
        virtual ~AuthorizationHandler() {}

        pair<bool,long> run(PEPRequest& request, bool isHandler=true) const;
        void receive(DDF& in, ostream& out);

    private:
        pair<bool,long> processAuthzRequest(const Application& application, const HTTPRequest& httpRequest, HTTPResponse& httpResponse) const;
        void processAuthzResponse(ElementProxy* xacmlResponse, HTTPResponse& httpResponse) const;
        void processUpdateRemoteUserObligation(xercesc::DOMElement* obligElem, HTTPResponse& httpResponse) const;
        void processRedirectObligation(xercesc::DOMElement* obligElem, HTTPResponse& httpResponse) const;
        std::string m_pdpURL;
    };


    Handler* AuthorizationHandlerFactory(const pair<const DOMElement*, const char*>& p);

}
#endif /* AUTHORIZATIONHANDLER_H_ */
