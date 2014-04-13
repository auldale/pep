/*
 * PEPRequestImpl.h
 *
 *  Created on: Apr 6, 2013
 *      Author: watanabe
 */

#ifndef PEPREQUESTIMPL_H_
#define PEPREQUESTIMPL_H_

#include "api/api.h"
#include "core/AbstractPEPRequest.h"

# include "util/CGIParser.h"

#ifdef PEP_LITE


namespace pep {

class PEPRequestImpl: public AbstractPEPRequest {
public:
	PEPRequestImpl(const t_authz_request* req);
	virtual ~PEPRequestImpl();

	const char* getHandlerURL(const char* resource = nullptr) const;
	virtual Settings getRequestSettings() const;

	const char* getScheme() const;
	const char* getHostname() const;
	int getPort() const;
	std::string getContentType() const;
	long getContentLength() const;
	const char* getRequestBody() const;
	std::string getRemoteUser() const;
	const std::vector<std::string>& getClientCertificates() const;
	const char* getMethod() const;
	const char* getQueryString() const;
	std::string getHeader(const char* name) const;
	long sendResponse(std::istream& inputStream, long status);
	void clearHeader(const char* rawname, const char* cginame);
	void setHeader(const char* name, const char* value);
	void setRemoteUser(const char* user);

	virtual std::string getResponseHeader(const char* name) const;
	virtual void setResponseHeader(const char* name, const char* value);

	long returnDecline(void);
	long returnOK(void);


private:
	Settings m_RequestSettings;
	const t_authz_request* m_AuthzRequest;
	std::map<std::string, std::string>  m_ResponseHeaderMap;
	mutable std::vector<std::string> m_ClientCerts;
};

} /* namespace pep */
#endif
#endif /* PEPREQUESTIMPL_H_ */
