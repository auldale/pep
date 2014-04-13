/*
 * api.c
 *
 *  Created on: Apr 1, 2013
 *      Author: watanabe
 */

#include "api/api.h"
#include "config/PEPConfig.h"
#include "core/PEP.h"
#include "remoting/ListenerService.h"
#include "core/Application.h"
#include "core/PEPRequestImpl.h"
#include "core/AuthorizationConstants.h"
#include <string>
#include <log4shib/Category.hh>

using namespace pep;
using namespace std;
using namespace log4shib;
#ifdef PEP_LITE
int pep_initialize(const char* config_path, void** config) {
	PEPConfig& conf = PEPConfig::getConfig();
	conf.setFeatures(PEPConfig::InProcess);
	string conf_str(config_path);
	conf.initialize(conf_str);
	*config = (void*) &conf;
	return 0;
}

int processResponse(PEPRequestImpl* response, t_authz_response* authz_res) {

	Category& log = Category::getInstance("API");

	string decision_header = response->getResponseHeader(PEP_HEADER_DECISION);

	if (decision_header.empty()) {
		log.debug("Did not recieve decision header in response. Setting deny result");
		authz_res->decision = t_authz_decision::DENY;
	} else {

		if (decision_header.compare(PEP_HEADER_VAL_PERMIT) == 0) {
			log.debug("Recieved Permit decision.");
			authz_res->decision = t_authz_decision::PERMIT;
		} else {
			log.debug("Recieved Permit decision.");
			authz_res->decision = t_authz_decision::DENY;
		}
	}

	string status_header = response->getResponseHeader(PEP_HEADER_STATUS);

	if (status_header.empty()) {
		log.debug(
				"Did not recieve status header in response. Setting decision to deny. Status code to ERROR");
		authz_res->status = t_authz_status::ERROR;
		authz_res->decision = t_authz_decision::DENY;
	} else {

		if (status_header.compare(PEP_HEADER_STATUS) == 0) {
			log.debug("status code OK");
			authz_res->status = t_authz_status::OK;

		} else {
			log.debug("status code ERROR");
			authz_res->status = t_authz_status::ERROR;
		}
	}

	string remote_user = response->getResponseHeader(PEP_HEADER_OBLIGATION_REMOTE_USER);

	if (!remote_user.empty()) {
		log.debug("Recieved set-remote-user Obligation in response.");
		authz_res->remote_user = new char[remote_user.length() + 1];
		strcpy(authz_res->remote_user, remote_user.c_str());
	} else {
		authz_res->remote_user = NULL;
	}

	string redirect = response->getResponseHeader(PEP_HEADER_OBLIGATION_REDIRECT);

	if (!redirect.empty()) {
		log.debug("Recieved redirect Obligation in response.");
		authz_res->redirect = new char[redirect.length() + 1];
		strcpy(authz_res->redirect, redirect.c_str());
	} else {
		authz_res->redirect = NULL;
	}

	return 0;
}

int pep_check_authorization(void* config, const char * app_id, t_authz_request* authz_req,
		t_authz_response * authz_res) {
	PEPConfig* conf = static_cast<PEPConfig*>(config);
	PEP* pep = conf->getPEP();
	PEPRequestImpl* pepReq = new PEPRequestImpl(authz_req);
	pep->doAuthorization(*pepReq);

	processResponse(pepReq, authz_res);
	return 0;
}

int pep_terminate(void* config) {
	PEPConfig* conf = static_cast<PEPConfig*>(config);
	conf->terminate();
	return 0;
}
#endif

//PEP* getConfigHandler modul si to ulozi pri startu do sve konf promenne a bude to predavat s kazdym volanim metody api
