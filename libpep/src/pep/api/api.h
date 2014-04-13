/*
 * api.h
 *
 *  Created on: Apr 1, 2013
 *      Author: watanabe
 */

#ifndef API_H_
#define API_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DECISION_PERMIT 1
#define DECISION_DENY 2

#define STATUS_OK 1
#define STATUS_ERROR 2

typedef struct {
	const char * name;
	const char * value;
} t_http_header;

typedef struct {
	const char* remote_user;
	const char* auth_type;
	const char* client_cert;
} t_http_auth_info;

typedef struct {
	const char* method;
	const char* scheme;
	const char* hostname;
	int port;
	const char* query_string;
	const char* uri;
	const char* remote_addr;
	t_http_header* headers;
} t_http_request;

typedef struct {
	t_http_request http_req;
	t_http_auth_info http_auth_info;
	const char* app_id;

} t_authz_request;

typedef enum {
	PERMIT = DECISION_PERMIT, DENY = DECISION_DENY
} t_authz_decision;

typedef enum {
	OK = STATUS_OK, ERROR = STATUS_ERROR
} t_authz_status;

typedef struct {
	t_authz_decision decision;
	t_authz_status status;
	char * remote_user;
	char * redirect;
} t_authz_response;

int pep_initialize(const char* config_path, void** config);

int pep_check_authorization(void* config, const char * app_id, t_authz_request* authz_req,
		t_authz_response * authz_res);

int pep_terminate(void* config);

#ifdef __cplusplus
}
#endif

#endif /* API_H_ */
