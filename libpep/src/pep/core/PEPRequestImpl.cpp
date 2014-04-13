/*
 * PEPRequestImpl.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: watanabe
 */

#include "PEPRequestImpl.h"
#include "util/DOMPropertySet.h"
#include <memory>
#include <string>
#include <iostream>
#include <xmltooling/unicode.h>
#include "internal.h"
#include "exceptions.h"

using namespace std;
using namespace xmltooling;

#ifdef PEP_LITE

namespace pep {

PEPRequestImpl::PEPRequestImpl(const t_authz_request* req) :
		AbstractPEPRequest("PEPRequestImpl"), m_AuthzRequest(req) {

	DOMPropertySet * propSet = new DOMPropertySet();

	propSet->setProperty("applicationId", m_AuthzRequest->app_id);
	//propSet->setProperty("REMOTE_ADDR", req->http_req.remote_addr);
	//propSet->setProperty("REMOTE_USER", req->http_req.remote_user);

	m_RequestSettings.first = propSet;
	m_RequestSettings.second = nullptr;
	setRequestURI(m_AuthzRequest->http_req.uri);
	setAuthType(m_AuthzRequest->http_auth_info.auth_type);
}

//explicitne nastavit getHandlerUrl
PEPRequestImpl::~PEPRequestImpl() {
	delete m_AuthzRequest;
}

const char* PEPRequestImpl::getHandlerURL(const char* resource) const {
	return "/handler/Authorization";
}

Settings PEPRequestImpl::getRequestSettings() const {
	return m_RequestSettings;
}

long PEPRequestImpl::returnDecline(void) {
	return 500;
}

long PEPRequestImpl::returnOK(void) {
	return 200;
}

const char* PEPRequestImpl::getScheme() const {
	return m_AuthzRequest->http_req.scheme;
}

const char* PEPRequestImpl::getHostname() const {
	return m_AuthzRequest->http_req.hostname;
}

int PEPRequestImpl::getPort() const {
	return m_AuthzRequest->http_req.port;

}
;
string PEPRequestImpl::getContentType() const {
	//TODO search through headers for content-type header
	return string("");
}

long PEPRequestImpl::getContentLength() const {
	//TODO search through headers for content-length header nebo to ziskat od apache?
	return 0;
}

const char* PEPRequestImpl::getRequestBody() const {
	// TODO
	return 0;
}

std::string PEPRequestImpl::getRemoteUser() const {
	return m_AuthzRequest->http_auth_info.remote_user?m_AuthzRequest->http_auth_info.remote_user:"";
}

const vector<string>& PEPRequestImpl::getClientCertificates() const {

	const char * clientCert = m_AuthzRequest->http_auth_info.client_cert;

	if (clientCert) {

		  m_ClientCerts.push_back(clientCert);

//		if (m_ClientCert.empty()) {
//
//			try {
//				auto_ptr<XSECCryptoX509> x509(XSECPlatformUtils::g_cryptoProvider->X509());
//				if (strstr(client_cert, "BEGIN"))
//					x509->loadX509PEM(client_cert, strlen(client_cert));
//				else
//					x509->loadX509Base64Bin(client_cert, strlen(client_cert));
//				m_ClientCert.push_back(x509.get());
//				x509.release();
//			} catch (XSECException& e) {
//				auto_ptr_char temp(e.getMsg());
//				Category::getInstance(SHIBSP_LOGCAT".PEPRequestImpl").error(
//						"XML-Security exception loading client certificate: %s", temp.get());
//			} catch (XSECCryptoException& e) {
//				Category::getInstance(SHIBSP_LOGCAT".PEPRequestImpl").error(
//						"XML-Security exception loading client certificate: %s", e.getMsg());
//			}
		//}
	}
	return m_ClientCerts;

}

const char* PEPRequestImpl::getMethod() const {
	return m_AuthzRequest->http_req.method;
}

const char* PEPRequestImpl::getQueryString() const {
	return m_AuthzRequest->http_req.query_string;
}

string PEPRequestImpl::getHeader(const char* name) const {
//TODO search headers
	//Xacml-Response-Decision=Permit
	return string("");
}

long PEPRequestImpl::sendResponse(istream& inputStream, long status) {

	// todo -> je potreba deserializovat inputStream do xml, a z tohoto xml vytahnout informace o xacml uspechu
	// a nastavit podle toho lokalni promennou, na kterou je pak mozne se z venci dotazat
	cout << "send response called";
	return 200;
}

void PEPRequestImpl::clearHeader(const char* rawname, const char* cginame) {
	// TODO
}
void PEPRequestImpl::setHeader(const char* name, const char* value) {
	// TODO
}

void PEPRequestImpl::setRemoteUser(const char* user) {

	// zpracovani obligaci
	// tj napr element <Action>setRemoteUser</Action>

// TODO zde je potreba doplnit implementaci kvuli nastaveni uid pri prihlaseni certifikatem
}

string PEPRequestImpl::getResponseHeader(const char* name) const {

	std::map<std::string, std::string>::const_iterator it = m_ResponseHeaderMap.find(name);

	if (it == m_ResponseHeaderMap.end()) {
		return string("");
	}

	return it->second;

}

void PEPRequestImpl::setResponseHeader(const char* name, const char* value) {
	HTTPResponse::setResponseHeader(name, value);

	m_ResponseHeaderMap[name] = value;

}

//
//class ShibTargetApache : public AbstractSPRequest
//{
//  mutable string m_body;
//  mutable bool m_gotBody,m_firsttime;
//  mutable vector<string> m_certs;
//  set<string> m_allhttp;
//
//
//public:
//  bool m_handler;
//  request_rec* m_req;
//  shib_dir_config* m_dc;
//  shib_server_config* m_sc;
//  shib_request_config* m_rc;
//
//  ShibTargetApache(request_rec* req) : AbstractSPRequest(SHIBSP_LOGCAT".Apache"),
//        m_gotBody(false),m_firsttime(true),
//
//        m_handler(false), m_req(req), m_dc(nullptr), m_sc(nullptr), m_rc(nullptr) {
//  }
//  virtual ~ShibTargetApache() {
//
//  }
//
//  bool isInitialized() const {
//      return (m_sc != nullptr);
//  }
//
//  bool init(bool handler, bool check_user) {
//    m_handler = handler;
//    if (m_sc)
//        return !check_user; // only initialize once
//    m_sc = (shib_server_config*)ap_get_module_config(m_req->server->module_config, &mod_shib);
//    m_dc = (shib_dir_config*)ap_get_module_config(m_req->per_dir_config, &mod_shib);
//    m_rc = (shib_request_config*)ap_get_module_config(m_req->request_config, &mod_shib);
//
//    setRequestURI(m_req->unparsed_uri);
//
//    if (check_user && m_dc->bUseHeaders == 1) {
//        // Try and see if this request was already processed, to skip spoof checking.
//        if (!ap_is_initial_req(m_req)) {
//            m_firsttime = false;
//        }
//        else if (!g_spoofKey.empty()) {
//            const char* hdr = ap_table_get(m_req->headers_in, "Shib-Spoof-Check");
//            if (hdr && g_spoofKey == hdr)
//                m_firsttime = false;
//        }
//        if (!m_firsttime)
//            log(SPDebug, "shib_check_user running more than once");
//    }
//    return true;
//  }
//
//  const char* getScheme() const {
//    return m_sc->szScheme ? m_sc->szScheme : ap_http_method(m_req);
//  }
//  bool isSecure() const {
//      return HTTPRequest::isSecure();
//  }
//  const char* getHostname() const {
//
//      return ap_get_server_name(m_req);
//
//  }
//  int getPort() const {
//    return ap_get_server_port(m_req);
//  }
//  const char* getMethod() const {
//    return m_req->method;
//  }
//  string getContentType() const {
//    const char* type = ap_table_get(m_req->headers_in, "Content-Type");
//    return type ? type : "";
//  }
//  long getContentLength() const {
//      return m_gotBody ? m_body.length() : m_req->remaining;
//  }
//  string getRemoteAddr() const {
//    string ret = AbstractSPRequest::getRemoteAddr();
//    if (!ret.empty())
//        return ret;
//
//    return m_req->connection->remote_ip;
//
//  }
//  void log(SPLogLevel level, const string& msg) const {
//    AbstractSPRequest::log(level,msg);
//    ap_log_rerror(
//        APLOG_MARK,
//        (level == SPDebug ? APLOG_DEBUG :
//        (level == SPInfo ? APLOG_INFO :
//        (level == SPWarn ? APLOG_WARNING :
//        (level == SPError ? APLOG_ERR : APLOG_CRIT))))|APLOG_NOERRNO,
//        SH_AP_R(m_req),
//        "%s",
//        msg.c_str()
//        );
//  }
//  const char* getQueryString() const { return m_req->args; }
//  const char* getRequestBody() const {
//    if (m_gotBody || m_req->method_number==M_GET)
//        return m_body.c_str();
//
//    const char *data;
//    apr_size_t len;
//    int seen_eos = 0;
//    apr_bucket_brigade* bb = apr_brigade_create(m_req->pool, m_req->connection->bucket_alloc);
//    do {
//        apr_bucket *bucket;
//        apr_status_t rv = ap_get_brigade(m_req->input_filters, bb, AP_MODE_READBYTES, APR_BLOCK_READ, HUGE_STRING_LEN);
//        if (rv != APR_SUCCESS) {
//            log(SPError, "Apache function (ap_get_brigade) failed while reading request body.");
//            break;
//        }
//
//        for (bucket = APR_BRIGADE_FIRST(bb); bucket != APR_BRIGADE_SENTINEL(bb); bucket = APR_BUCKET_NEXT(bucket)) {
//            if (APR_BUCKET_IS_EOS(bucket)) {
//                seen_eos = 1;
//                break;
//            }
//
//            /* We can't do much with this. */
//            if (APR_BUCKET_IS_FLUSH(bucket))
//                continue;
//
//            /* read */
//            apr_bucket_read(bucket, &data, &len, APR_BLOCK_READ);
//            if (len > 0)
//                m_body.append(data, len);
//        }
//        apr_brigade_cleanup(bb);
//    } while (!seen_eos);
//    apr_brigade_destroy(bb);
//    m_gotBody=true;
//
//    return m_body.c_str();
//  }
//  const char* getParameter(const char* name) const {
//      return AbstractSPRequest::getParameter(name);
//  }
//  vector<const char*>::size_type getParameters(const char* name, vector<const char*>& values) const {
//      return AbstractSPRequest::getParameters(name, values);
//  }
//  void clearHeader(const char* rawname, const char* cginame) {
//    if (m_dc->bUseHeaders == 1) {
//       // ap_log_rerror(APLOG_MARK,APLOG_DEBUG|APLOG_NOERRNO,SH_AP_R(m_req), "shib_clear_header: hdr\n");
//        if (g_checkSpoofing && m_firsttime) {
//            if (m_allhttp.empty()) {
//                // First time, so populate set with "CGI" versions of client-supplied headers.
//
//
//                const apr_array_header_t *hdrs_arr = apr_table_elts(m_req->headers_in);
//                const apr_table_entry_t *hdrs = (const apr_table_entry_t *) hdrs_arr->elts;
//
//                for (int i = 0; i < hdrs_arr->nelts; ++i) {
//                    if (!hdrs[i].key)
//                        continue;
//                    string cgiversion("HTTP_");
//                    const char* pch = hdrs[i].key;
//                    while (*pch) {
//                        cgiversion += (isalnum(*pch) ? toupper(*pch) : '_');
//                        pch++;
//                    }
//                    m_allhttp.insert(cgiversion);
//                }
//            }
//
//            if (m_allhttp.count(cginame) > 0)
//                throw opensaml::SecurityPolicyException("Attempt to spoof header ($1) was detected.", params(1, rawname));
//        }
//        ap_table_unset(m_req->headers_in, rawname);
//        ap_table_set(m_req->headers_in, rawname, g_unsetHeaderValue.c_str());
//    }
//  }
//  void setHeader(const char* name, const char* value) {
//    if (m_dc->bUseEnvVars != 0) {
//       if (!m_rc) {
//          // this happens on subrequests
//          // ap_log_rerror(APLOG_MARK,APLOG_DEBUG|APLOG_NOERRNO,SH_AP_R(m_req), "shib_setheader: no_m_rc\n");
//          m_rc = get_request_config(m_req);
//       }
//       if (!m_rc->env)
//           m_rc->env = ap_make_table(m_req->pool, 10);
//       // ap_log_rerror(APLOG_MARK,APLOG_DEBUG|APLOG_NOERRNO,SH_AP_R(m_req), "shib_set_env: %s=%s\n", name, value?value:"Null");
//       ap_table_set(m_rc->env, name, value ? value : "");
//    }
//    if (m_dc->bUseHeaders == 1)
//       ap_table_set(m_req->headers_in, name, value);
//  }
//  string getHeader(const char* name) const {
//    const char* hdr = ap_table_get(m_req->headers_in, name);
//    return string(hdr ? hdr : "");
//  }
//  string getSecureHeader(const char* name) const {
//    if (m_dc->bUseEnvVars != 0) {
//       const char *hdr;
//       if (m_rc && m_rc->env)
//           hdr = ap_table_get(m_rc->env, name);
//       else
//           hdr = nullptr;
//       return string(hdr ? hdr : "");
//    }
//    return getHeader(name);
//  }
//  void setRemoteUser(const char* user) {
//      SH_AP_USER(m_req) = user ? ap_pstrdup(m_req->pool, user) : nullptr;
//      if (m_dc->bUseHeaders == 1) {
//          if (user) {
//              ap_table_set(m_req->headers_in, "REMOTE_USER", user);
//          }
//          else {
//              ap_table_unset(m_req->headers_in, "REMOTE_USER");
//              ap_table_set(m_req->headers_in, "REMOTE_USER", g_unsetHeaderValue.c_str());
//          }
//      }
//  }
//  string getRemoteUser() const {
//    return string(SH_AP_USER(m_req) ? SH_AP_USER(m_req) : "");
//  }
//  void setAuthType(const char* authtype) {
//      if (authtype && m_dc->bBasicHijack == 1)
//          authtype = "Basic";
//      SH_AP_AUTH_TYPE(m_req) = authtype ? ap_pstrdup(m_req->pool, authtype) : nullptr;
//  }
//  string getAuthType() const {
//    return string(SH_AP_AUTH_TYPE(m_req) ? SH_AP_AUTH_TYPE(m_req) : "");
//  }
//  void setContentType(const char* type) {
//      m_req->content_type = ap_psprintf(m_req->pool, "%s", type);
//  }
//  void setResponseHeader(const char* name, const char* value) {
//   HTTPResponse::setResponseHeader(name, value);
//
//    ap_table_add(m_req->err_headers_out, name, value);
//  }
//  long sendResponse(istream& in, long status) {
//    if (status != XMLTOOLING_HTTP_STATUS_OK)
//        m_req->status = status;
//    ap_send_http_header(m_req);
//    char buf[1024];
//    while (in) {
//        in.read(buf,1024);
//        ap_rwrite(buf,in.gcount(),m_req);
//    }
//#if (defined(SHIB_APACHE_20) || defined(SHIB_APACHE_22) || defined(SHIB_APACHE_24))
//    if (status != XMLTOOLING_HTTP_STATUS_OK && status != XMLTOOLING_HTTP_STATUS_ERROR)
//        return status;
//#endif
//    return DONE;
//  }
//  long sendRedirect(const char* url) {
//    HTTPResponse::sendRedirect(url);
//    ap_table_set(m_req->headers_out, "Location", url);
//    if (m_dc->bExpireRedirects != 0) {
//        ap_table_set(m_req->err_headers_out, "Expires", "Wed, 01 Jan 1997 12:00:00 GMT");
//        ap_table_set(m_req->err_headers_out, "Cache-Control", "private,no-store,no-cache,max-age=0");
//    }
//    return REDIRECT;
//  }
//  const vector<string>& getClientCertificates() const {
//      if (m_certs.empty()) {
//          const char* cert = ap_table_get(m_req->subprocess_env, "SSL_CLIENT_CERT");
//          if (cert)
//              m_certs.push_back(cert);
//          int i = 0;
//          do {
//              cert = ap_table_get(m_req->subprocess_env, ap_psprintf(m_req->pool, "SSL_CLIENT_CERT_CHAIN_%d", i++));
//              if (cert)
//                  m_certs.push_back(cert);
//          } while (cert);
//      }
//      return m_certs;
//  }
//  long returnDecline(void) { return DECLINED; }
//  long returnOK(void) { return OK; }

} /* namespace pep */
#endif
