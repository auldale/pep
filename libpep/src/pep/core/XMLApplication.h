/*
 * XMLApplication.h
 *
 *  Created on: Apr 3, 2013
 *      Author: watanabe
 */

#ifndef XMLAPPLICATION_H_
#define XMLAPPLICATION_H_


#include "internal.h"
#include "exceptions.h"
#include "core/Application.h"
#include "config/PEPConfig.h"
#include "core/PEPRequest.h"
#include "remoting/ListenerService.h"
#include "util/DOMPropertySet.h"
#include "util/SPConstants.h"


# include <log4cpp/PropertyConfigurator.hh>

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>
#include <xmltooling/XMLToolingConfig.h>
#include <xmltooling/version.h>
#include <xmltooling/util/NDC.h>
#include <xmltooling/util/ReloadableXMLFile.h>
#include <xmltooling/util/TemplateEngine.h>
#include <xmltooling/util/Threads.h>
#include <xmltooling/util/XMLHelper.h>


# include <xmltooling/security/ChainingTrustEngine.h>
# include <xmltooling/security/CredentialResolver.h>
# include <xmltooling/security/SecurityHelper.h>
# include <xmltooling/util/ReplayCache.h>
# include <xmltooling/util/StorageService.h>
# include <xsec/utils/XSECPlatformUtils.hpp>


namespace pep {

class XMLApplication: public Application, public DOMPropertySet, public DOMNodeFilter {
public:
	XMLApplication(const PEP*, DOMElement* appConfigElem,
			const XMLApplication* base = nullptr);
	~XMLApplication();

	const char* getHash() const {
		return m_hash.c_str();
	}

	const Handler* getHandler(const char* path) const;
	void getHandlers(std::vector<const Handler*>& handlers) const;

	/**
	 * Ensures no value exists for a request header, allowing for application-specific customization.
	 *
	 * @param request  SP request to modify
	 * @param rawname  raw name of header to clear
	 * @param cginame  CGI-equivalent name of header, <strong>MUST</strong> begin with "HTTP_".
	 */
	virtual void clearHeader(PEPRequest& request, const char* rawname, const char* cginame) const;

	/**
	 * Sets a value for a request header allowing for application-specific customization.
	 *
	 * @param request   SP request to modify
	 * @param name      name of header to set
	 * @param value     value to set
	 */
	virtual void setHeader(PEPRequest& request, const char* name, const char* value) const;

	/**
	 * Returns a non-spoofable request header value allowing for application-specific customization.
	 *
	 * @param request   SP request to access
	 * @param name      the name of the secure header to return
	 * @return  the header's value, or an empty string
	 */
	virtual std::string getSecureHeader(const PEPRequest& request, const char* name) const;

	/**
	 * Clears any headers that may be used to hold attributes after export.
	 *
	 * @param request   SP request to clear
	 */
	//virtual void clearAttributeHeaders(PEPRequest& request) const;

	// Provides filter to exclude special config elements.
#ifdef SHIBSP_XERCESC_SHORT_ACCEPTNODE
	short
#else
	FilterAction
#endif
	acceptNode(const DOMNode* node) const;

private:
	const XMLApplication* m_base;
	std::string m_hash;
	std::pair<std::string, std::string> m_attributePrefix;

	// maps location (path info) to applicable handlers
	std::map<std::string, const Handler*> m_handlerMap;

	// manage handler objects
	std::vector<boost::shared_ptr<Handler> > m_handlers;

	void doHandlers(const xercesc::DOMElement* e, Category& log);

	/*

	 static const XMLCh applicationId[] =        UNICODE_LITERAL_13(a,p,p,l,i,c,a,t,i,o,n,I,d);
	 */
	//static const XMLCh ApplicationOverride[] =
	//		UNICODE_LITERAL_19(A,p,p,l,i,c,a,t,i,o,n,O,v,e,r,r,i,d,e);
	//static const XMLCh ApplicationDefaults[] = UNICODE_LITERAL_19(A,p,p,l,i,c,a,t,i,o,n,D,e,f,a,u,l,t,s);
	/*  static const XMLCh _ArtifactMap[] =         UNICODE_LITERAL_11(A,r,t,i,f,a,c,t,M,a,p);
	 static const XMLCh _AttributeExtractor[] =  UNICODE_LITERAL_18(A,t,t,r,i,b,u,t,e,E,x,t,r,a,c,t,o,r);
	 static const XMLCh _AttributeFilter[] =     UNICODE_LITERAL_15(A,t,t,r,i,b,u,t,e,F,i,l,t,e,r);
	 static const XMLCh _AttributeResolver[] =   UNICODE_LITERAL_17(A,t,t,r,i,b,u,t,e,R,e,s,o,l,v,e,r);
	 static const XMLCh _AssertionConsumerService[] = UNICODE_LITERAL_24(A,s,s,e,r,t,i,o,n,C,o,n,s,u,m,e,r,S,e,r,v,i,c,e);
	 static const XMLCh _ArtifactResolutionService[] =UNICODE_LITERAL_25(A,r,t,i,f,a,c,t,R,e,s,o,l,u,t,i,o,n,S,e,r,v,i,c,e);
	 static const XMLCh _Audience[] =            UNICODE_LITERAL_8(A,u,d,i,e,n,c,e);
	 static const XMLCh Binding[] =              UNICODE_LITERAL_7(B,i,n,d,i,n,g);
	 static const XMLCh Channel[]=               UNICODE_LITERAL_7(C,h,a,n,n,e,l);
	 static const XMLCh _CredentialResolver[] =  UNICODE_LITERAL_18(C,r,e,d,e,n,t,i,a,l,R,e,s,o,l,v,e,r);
	 static const XMLCh _default[] =             UNICODE_LITERAL_7(d,e,f,a,u,l,t);
	 static const XMLCh _Extensions[] =          UNICODE_LITERAL_10(E,x,t,e,n,s,i,o,n,s);
	 static const XMLCh _fatal[] =               UNICODE_LITERAL_5(f,a,t,a,l);
	 static const XMLCh _Handler[] =             UNICODE_LITERAL_7(H,a,n,d,l,e,r);
	 static const XMLCh _id[] =                  UNICODE_LITERAL_2(i,d);
	 static const XMLCh _index[] =               UNICODE_LITERAL_5(i,n,d,e,x);
	 static const XMLCh InProcess[] =            UNICODE_LITERAL_9(I,n,P,r,o,c,e,s,s);
	 static const XMLCh Library[] =              UNICODE_LITERAL_7(L,i,b,r,a,r,y);
	 static const XMLCh Listener[] =             UNICODE_LITERAL_8(L,i,s,t,e,n,e,r);
	 static const XMLCh Location[] =             UNICODE_LITERAL_8(L,o,c,a,t,i,o,n);
	 static const XMLCh logger[] =               UNICODE_LITERAL_6(l,o,g,g,e,r);
	 static const XMLCh Logout[] =               UNICODE_LITERAL_6(L,o,g,o,u,t);
	 static const XMLCh _LogoutInitiator[] =     UNICODE_LITERAL_15(L,o,g,o,u,t,I,n,i,t,i,a,t,o,r);
	 static const XMLCh _ManageNameIDService[] = UNICODE_LITERAL_19(M,a,n,a,g,e,N,a,m,e,I,D,S,e,r,v,i,c,e);
	 static const XMLCh _MetadataProvider[] =    UNICODE_LITERAL_16(M,e,t,a,d,a,t,a,P,r,o,v,i,d,e,r);
	 static const XMLCh NameIDMgmt[] =           UNICODE_LITERAL_10(N,a,m,e,I,D,M,g,m,t);
	 static const XMLCh Notify[] =               UNICODE_LITERAL_6(N,o,t,i,f,y);
	 static const XMLCh _option[] =              UNICODE_LITERAL_6(o,p,t,i,o,n);
	 static const XMLCh OutOfProcess[] =         UNICODE_LITERAL_12(O,u,t,O,f,P,r,o,c,e,s,s);
	 static const XMLCh _path[] =                UNICODE_LITERAL_4(p,a,t,h);
	 static const XMLCh _policyId[] =            UNICODE_LITERAL_8(p,o,l,i,c,y,I,d);
	 static const XMLCh _ProtocolProvider[] =    UNICODE_LITERAL_16(P,r,o,t,o,c,o,l,P,r,o,v,i,d,e,r);
	 static const XMLCh _provider[] =            UNICODE_LITERAL_8(p,r,o,v,i,d,e,r);
	 static const XMLCh RelyingParty[] =         UNICODE_LITERAL_12(R,e,l,y,i,n,g,P,a,r,t,y);
	 static const XMLCh _ReplayCache[] =         UNICODE_LITERAL_11(R,e,p,l,a,y,C,a,c,h,e);
	 static const XMLCh _RequestMapper[] =       UNICODE_LITERAL_13(R,e,q,u,e,s,t,M,a,p,p,e,r);
	 static const XMLCh RequestMap[] =           UNICODE_LITERAL_10(R,e,q,u,e,s,t,M,a,p);
	 static const XMLCh SecurityPolicies[] =     UNICODE_LITERAL_16(S,e,c,u,r,i,t,y,P,o,l,i,c,i,e,s);
	 static const XMLCh _SecurityPolicyProvider[] = UNICODE_LITERAL_22(S,e,c,u,r,i,t,y,P,o,l,i,c,y,P,r,o,v,i,d,e,r);
	 static const XMLCh _SessionCache[] =        UNICODE_LITERAL_12(S,e,s,s,i,o,n,C,a,c,h,e);
	 static const XMLCh _SessionInitiator[] =    UNICODE_LITERAL_16(S,e,s,s,i,o,n,I,n,i,t,i,a,t,o,r);
	 static const XMLCh _SingleLogoutService[] = UNICODE_LITERAL_19(S,i,n,g,l,e,L,o,g,o,u,t,S,e,r,v,i,c,e);
	 static const XMLCh Site[] =                 UNICODE_LITERAL_4(S,i,t,e);
	 static const XMLCh SSO[] =                  UNICODE_LITERAL_3(S,S,O);
	 static const XMLCh _StorageService[] =      UNICODE_LITERAL_14(S,t,o,r,a,g,e,S,e,r,v,i,c,e);
	 static const XMLCh TCPListener[] =          UNICODE_LITERAL_11(T,C,P,L,i,s,t,e,n,e,r);
	 static const XMLCh tranLogFiller[] =        UNICODE_LITERAL_13(t,r,a,n,L,o,g,F,i,l,l,e,r);
	 static const XMLCh tranLogFormat[] =        UNICODE_LITERAL_13(t,r,a,n,L,o,g,F,o,r,m,a,t);
	 static const XMLCh TransportOption[] =      UNICODE_LITERAL_15(T,r,a,n,s,p,o,r,t,O,p,t,i,o,n);
	 static const XMLCh _TrustEngine[] =         UNICODE_LITERAL_11(T,r,u,s,t,E,n,g,i,n,e);
	 static const XMLCh _type[] =                UNICODE_LITERAL_4(t,y,p,e);
	 static const XMLCh UnixListener[] =         UNICODE_LITERAL_12(U,n,i,x,L,i,s,t,e,n,e,r);
	 */
};

/*PropertySet getUnsignedInt(const char* name, const char* ns = nullptr) const {
 return nullptr;
 }*/

}

#endif /* XMLAPPLICATION_H_ */
