#include <stdio.h>
#include <stdlib.h>
#include <pep/api/api.h>

#include <sys/types.h>
#include <apr-1/apr_strings.h>
#include <apache2/ap_config.h>
#include <apache2/httpd.h>
#include <apache2/http_config.h>
#include <apache2/http_core.h>
#include <apache2/http_log.h>
#include <apache2/http_protocol.h>
#include <apache2/http_request.h>
#include <apache2/mod_auth.h>
#include <unistd.h>

#define PEP_LITE

typedef struct {
	const char* config_path;
} t_mod_pep_sc;

typedef struct {
	int enabled;
	char* app_id;
} t_mod_pep_dc;

static t_mod_pep_sc mod_pep_config;

static void* pep_config;

module AP_MODULE_DECLARE_DATA pep_module;

static void *create_pep_authz_dir_config(apr_pool_t *p, char *d) {
	t_mod_pep_dc *conf = apr_palloc(p, sizeof(*conf));
	conf->enabled = 0;
	conf->app_id = apr_palloc(p, sizeof(char) * 100);

	strcpy(conf->app_id, "default");
	return conf;
}

const char *set_pep_config_path(cmd_parms *cmd, void *cfg, const char *arg) {
printf("conf ath %s",arg);
	mod_pep_config.config_path = arg;
	return NULL ;
}

static const command_rec pep_authz_cmds[] =
		{ AP_INIT_FLAG("PEPAuthoritative", ap_set_flag_slot,
						(void *)APR_OFFSETOF(t_mod_pep_dc, enabled),
										OR_AUTHCFG,
														"Set to 'Off' to allow access control to be passed along to "
																		"lower modules. (default: On)."),

																								AP_INIT_TAKE1("PEPConfig", set_pep_config_path, NULL, RSRC_CONF, "Path to PEP Config file."),
																												{ NULL } };

//static const command_rec pep_authz_cmds[] =
//		{
//						AP_INIT_FLAG("PEPAuthoritative", ap_set_flag_slot, (void *)APR_OFFSETOF(t_mod_pep_dc, enabled), OR_AUTHCFG, "Set to 'Off' to allow access control to be passed to lower modules. (default: On)."),
//						AP_INIT_TAKE1("PEPAppId", ap_set_string_slot , (void *)APR_OFFSETOF(t_mod_pep_dc, app_id), OR_AUTHCFG, "Set app id for mod_pep."),
//						AP_INIT_TAKE1("PEPConfig", set_pep_config_path, NULL, RSRC_CONF, "Path to PEP Config file."), 
//				{ NULL } };

const char* prepare_uri(request_rec* r) {
	return r->unparsed_uri;
}

const char* prepare_scheme(request_rec* r) {
	return ap_http_scheme(r);
}

const char* prepare_hostname(request_rec* r) {
	return ap_get_server_name(r);
}

int prepare_port(request_rec* r) {
	return ap_get_server_port(r);
}

const char* prepare_method(request_rec* r) {
	return r->method;
}

const char* prepare_query_string(request_rec* r) {
	return r->args;
}

const char* prepare_remote_addr(request_rec* r) {
	return r->connection->remote_ip;
}

const char* prepare_remote_user(request_rec* r) {
if(r->user){
   ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "remote_user=%s",r->user);
}else {

   ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "no remote_user");
}

	return r->user ? r->user : NULL ;
}

const char* prepare_auth_type(request_rec* r) {
	return r->ap_auth_type ? r->ap_auth_type : NULL ;
}

const char* prepare_client_cert(request_rec* r) {
	//const char* cert = apr_table_get(r->subprocess_env, "SSL_CLIENT_CERT");

	//printf("\n\nIn ap_headers_early: About to call ssl_var_lookup\n");

	typedef char* (*ssl_var_lookup_t)(apr_pool_t*, server_rec*, conn_rec*, request_rec*, char*);
	ssl_var_lookup_t ssl_var_lookup = 0;
	ssl_var_lookup = (ssl_var_lookup_t) apr_dynamic_fn_retrieve("ssl_var_lookup");
	const char * foo = ssl_var_lookup(r->pool, r->server, r->connection, r, "SSL_CLIENT_CERT");
	//printf("In ap_headers_early: SSL_CLIENT_CERT=[%s]\n", foo);

	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "ssl looked up: %s.", foo);

	return foo;

}

void set_remote_user(request_rec* r, const char* user) {

	r->user = apr_pstrdup(r->pool, user);
	apr_table_set(r->headers_in, "REMOTE_USER", user);

}

static int check_pep_authz(request_rec *r) {

	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "Started proceeding authz_user_check.");

	t_mod_pep_dc *conf = ap_get_module_config(r->per_dir_config,
			&pep_module) ;

	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "enabled=%d",conf->enabled);
if(conf->app_id){
	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "appid=%s",conf->app_id);
	}

	if (conf->enabled == 0) {
		return OK;
	}

	t_authz_request* authz_req = (t_authz_request*) apr_palloc(r->pool, sizeof(t_authz_request));

	//if (conf->app_id != 0) {
	//	authz_req->app_id = conf->app_id;
	//} else {
		authz_req->app_id = "default";
	//}
	//const char* auth_type_val =
	authz_req->http_auth_info.auth_type = prepare_auth_type(r);
	authz_req->http_auth_info.remote_user = prepare_remote_user(r);
	authz_req->http_auth_info.client_cert = prepare_client_cert(r);
	authz_req->http_req.uri = prepare_uri(r);
	authz_req->http_req.hostname = prepare_hostname(r);
	authz_req->http_req.method = prepare_method(r);
	authz_req->http_req.scheme = prepare_scheme(r);
	authz_req->http_req.query_string = prepare_query_string(r);
	authz_req->http_req.remote_addr = prepare_remote_addr(r);
	authz_req->http_req.port = prepare_port(r);

	t_authz_response * authz_res = (t_authz_response*) apr_palloc(r->pool,
			sizeof(t_authz_response));


	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "calling pep_check_authorization");
	pep_check_authorization(pep_config, "default", authz_req, authz_res);

	// process remote_user obligation
	if (authz_res->remote_user) {
		ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "setting remote user to %s",
				authz_res->remote_user);
		set_remote_user(r, authz_res->remote_user);
	}

	// process redirect	 obligation
	if (authz_res->redirect) {
		ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "redirecting to %s", authz_res->redirect);

		apr_table_set(r->headers_out, "Location", authz_res->redirect);

		apr_table_set(r->err_headers_out, "Expires", "Wed, 01 Jan 1997 12:00:00 GMT");
		apr_table_set(r->err_headers_out, "Cache-Control", "private,no-store,no-cache,max-age=0");

		return HTTP_MOVED_TEMPORARILY;

	}

	if (authz_res->decision == DECISION_PERMIT) {
		return OK;
	}

	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
			"access to %s failed, reason: user '%s' does not meet "
					"'require'ments.", r->uri, "user");

	ap_note_auth_failure(r);
	return HTTP_UNAUTHORIZED;
}

apr_status_t pep_config_terminate(void* data) {
	if (pep_config) {
		pep_terminate(pep_config);
		pep_config = NULL;
	}

	ap_log_error(APLOG_MARK, APLOG_INFO | APLOG_NOERRNO, 0, data,
			"pep_exit: mod_pep shutdown in pid (%d)", (int) getpid());
	return OK;
}

static int pep_config_init(apr_pool_t* p, apr_pool_t* p1, apr_pool_t* p2, server_rec* s) {
	// Initialize runtime components.
	ap_log_error(APLOG_MARK, APLOG_INFO | APLOG_NOERRNO, 0, s,
			"post_config: mod_pep initializing in pid (%d)", (int) getpid());

	if (pep_config) {
		ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
				"post_config: mod_pep already initialized in pid (%d)", (int) getpid());
		return !OK;
	}

	if(!mod_pep_config.config_path) {
		ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
				"maissing pep config parh");

		}
	pep_initialize(mod_pep_config.config_path, &pep_config);

	// Set the cleanup handler, passing in the server_rec for logging.
	apr_pool_cleanup_register(p, s, &pep_config_terminate, apr_pool_cleanup_null);

	return OK;
}

static authn_status check_password(request_rec *r, const char *user, const char *password) {
	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "pep-override provider: check_password.");

	return AUTH_GRANTED;
}

static authn_status get_realm_hash(request_rec *r, const char *user, const char *realm,
		char **rethash) {

	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "pep-override provider: check_password.");

	*rethash = NULL;

	return AUTH_USER_FOUND;
}

static const authn_provider authn_pep_provider = { &check_password, &get_realm_hash, };

static void register_hooks(apr_pool_t *p) {

	ap_register_provider(p, AUTHN_PROVIDER_GROUP, "pep-override", "0", &authn_pep_provider);

	ap_hook_auth_checker(check_pep_authz, NULL, NULL, APR_HOOK_FIRST);
	ap_hook_post_config(pep_config_init, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA pep_module = { STANDARD20_MODULE_STUFF, create_pep_authz_dir_config, /* dir config creater */
NULL, /* dir merger --- default is to override */
NULL, /* server config */
NULL, /* merge server config */
pep_authz_cmds, /* command apr_table_t */
register_hooks /* register hooks */
};
