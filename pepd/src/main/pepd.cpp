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

/*
 * shibd.cpp -- the shibd "main" code.
 */

// eventually we might be able to support autoconf via cygwin...
/*
 #if defined (_MSC_VER) || defined(__BORLANDC__)
 # include "config_win32.h"
 #else
 # include "config.h"
 #endif
 */

/*
 #ifdef WIN32
 # define _CRT_NONSTDC_NO_DEPRECATE 1
 # define _CRT_SECURE_NO_DEPRECATE 1
 #endif
 */

//#include <shibsp/SPConfig.h>
#include <pep/config/PEPConfig.h>

# include <unistd.h>
# include <sys/select.h>
#include <stdio.h>
#include <signal.h>
#include <pep/remoting/ListenerService.h>
#include <pep/remoting/impl/UnixListener.h>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xmltooling/XMLToolingConfig.h>
#include <xmltooling/util/XMLConstants.h>
#include <xmltooling/util/XMLHelper.h>

using namespace xmltooling;
using namespace std;
using namespace pep;

bool shibd_shutdown = false;
const char* shar_config = nullptr;
const char* shar_schemadir = nullptr;
const char* shar_prefix = nullptr;
bool shar_checkonly = false;
bool shar_version = false;
static bool unlink_socket = false;
const char* pidfile = nullptr;

int daemon_wait = 3;
bool shibd_running = false;
bool daemonize = true;
const char* runasuser = nullptr;
const char* runasgroup = nullptr;

static void term_handler(int arg) {
	shibd_shutdown = true;
}

static void run_handler(int arg) {
	shibd_running = true;
}

static void child_handler(int arg) {
	// Terminate the parent's wait/sleep if the newly born daemon dies early.
}

static int setup_signals(void) {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGPIPE, &sa, nullptr) < 0) {
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = term_handler;
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGHUP, &sa, nullptr) < 0) {
		return -1;
	}
	if (sigaction(SIGINT, &sa, nullptr) < 0) {
		return -1;
	}
	if (sigaction(SIGQUIT, &sa, nullptr) < 0) {
		return -1;
	}
	if (sigaction(SIGTERM, &sa, nullptr) < 0) {
		return -1;
	}

	if (daemonize) {
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = run_handler;

		if (sigaction(SIGUSR1, &sa, nullptr) < 0) {
			return -1;
		}

		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = child_handler;

		if (sigaction(SIGCHLD, &sa, nullptr) < 0) {
			return -1;
		}
	}

	return 0;
}

static void usage(char* whoami) {
	fprintf(stderr, "usage: %s [-dcxtfFpwugvh]\n", whoami);
	fprintf(stderr, "  -d\tinstallation prefix to use\n");
	fprintf(stderr, "  -c\tconfig file to use\n");
	fprintf(stderr, "  -x\tXML schema catalogs to use\n");
	fprintf(stderr, "  -t\ttest configuration file for problems\n");
	fprintf(stderr, "  -f\tforce removal of listener socket\n");
	fprintf(stderr, "  -F\tstay in the foreground\n");
	fprintf(stderr, "  -p\tpid file to use\n");
	fprintf(stderr, "  -w\tseconds to wait for successful daemonization\n");
	fprintf(stderr, "  -u\tuser to run under\n");
	fprintf(stderr, "  -g\tgroup to run under\n");
	fprintf(stderr, "  -v\tprint software version\n");
	fprintf(stderr, "  -h\tprint this help message\n");
	exit(1);
}

static int parse_args(int argc, char* argv[]) {
	int opt;

	while ((opt = getopt(argc, argv, "d:c:x:p:w:u:g:fFtvh")) > 0) {
		switch (opt) {
		case 'd':
			shar_prefix = optarg;
			break;
		case 'c':
			shar_config = optarg;
			break;
		case 'x':
			shar_schemadir = optarg;
			break;
		case 'f':
			unlink_socket = true;
			break;
		case 'F':
			daemonize = false;
			break;
		case 't':
			shar_checkonly = true;
			daemonize = false;
			break;
		case 'v':
			shar_version = true;
			break;
		case 'p':
			pidfile = optarg;
			break;
		case 'w':
			if (optarg)
				daemon_wait = atoi(optarg);
			if (daemon_wait <= 0)
				daemon_wait = 3;
			break;
		case 'u':
			if (optarg)
				runasuser = optarg;
			break;
		case 'g':
			if (optarg)
				runasgroup = optarg;
			break;
		default:
			return -1;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if (parse_args(argc, argv) != 0)
		usage(argv[0]);
	else if (shar_version) {
		//fprintf(stdout, PACKAGE_STRING"\n");
		return 0;
	}

	if (setup_signals() != 0)
		return -1;

	if (runasgroup) {
#ifdef HAVE_GETGRNAM
		struct group* grp = getgrnam(runasgroup);
		if (!grp) {
			fprintf(stderr, "getgrnam failed, check -g option\n");
			return -1;
		}
		if (setgid(grp->gr_gid) != 0) {
			fprintf(stderr, "setgid failed, check -g option\n");
			return -1;
		}
#else
		fprintf(stderr, "-g not supported on this platform");
		return -1;
#endif
	}

	if (runasuser) {
#ifdef HAVE_GETPWNAM
		struct passwd* pwd = getpwnam(runasuser);
		if (!pwd) {
			fprintf(stderr, "getpwnam failed, check -u option\n");
			return -1;
		}
		if (setuid(pwd->pw_uid) != 0) {
			fprintf(stderr, "setuid failed, check -u option\n");
			return -1;
		}
#else
		fprintf(stderr, "-u not supported on this platform");
		return -1;
#endif
	}

	// initialize the shib-target library
	/*    SPConfig& conf=SPConfig::getConfig();
	 conf.setFeatures(
	 SPConfig::Listener |
	 SPConfig::Caching |
	 SPConfig::Metadata |
	 SPConfig::Trust |
	 SPConfig::Credentials |
	 SPConfig::AttributeResolution |
	 SPConfig::Handlers |
	 SPConfig::OutOfProcess |
	 (shar_checkonly ? SPConfig::RequestMapping : SPConfig::Logging)
	 );
	 if (!conf.init(shar_schemadir, shar_prefix)) {
	 fprintf(stderr, "configuration is invalid, check console for specific problems\n");
	 return -1;
	 }*/

	PEPConfig& conf = PEPConfig::getConfig();
	conf.setFeatures(PEPConfig::OutOfProcess);


	if (daemonize) {
		// We must fork() early, while we're single threaded.
		// StorageService cleanup thread is about to start.
		switch (fork()) {
		case 0:
			break;
		case -1:
			perror("forking");
			exit(EXIT_FAILURE);
		default:
			sleep(daemon_wait);
			exit(shibd_running ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	string confPath = string(shar_config);
	conf.initialize(confPath);

	/*    if (!conf.instantiate(shar_config)) {
	 fprintf(stderr, "configuration is invalid, check console for specific problems\n");
	 conf.term();
	 return -2;
	 }*/

	shar_checkonly = false;

	if (shar_checkonly)
		fprintf(stderr,
				"overall configuration is loadable, check console for non-fatal problems\n");
	else {

		// Init the listener.
		if (!conf.getPEP()->validate()) {
			fprintf(stderr,
					"PEP configuration validation failed. Check pepd log for further details.\n");
			exit(EXIT_FAILURE);
		}
		ListenerService* listener = conf.getPEP()->getListenerService();

		if (!listener->init(unlink_socket)) {
			fprintf(stderr, "listener failed to initialize\n");
			//conf.term();
			return -3;
		}

		if (daemonize) {
			if (setsid() == -1) {
				perror("setsid");
				exit(EXIT_FAILURE);
			}
			if (chdir("/") == -1) {
				perror("chdir to root");
				exit(EXIT_FAILURE);
			}

			if (pidfile) {
				FILE* pidf = fopen(pidfile, "w");
				if (pidf) {
					fprintf(pidf, "%d\n", getpid());
					fclose(pidf);
				} else {
					perror(pidfile);
				}
			}

			freopen("/dev/null", "r", stdin);
			freopen("/dev/null", "w", stdout);
			freopen("/dev/null", "w", stderr);

			// Signal our parent that we are A-OK.
			kill(getppid(), SIGUSR1);
		}

		// Run the listener.
		if (!listener->run(&shibd_shutdown)) {
			fprintf(stderr, "listener failure during service\n");
			listener->term();
			//      conf.term();
			if (daemonize && pidfile)
				unlink(pidfile);
			return -3;
		}
		listener->term();
	}

	//conf.term();
	if (daemonize && pidfile)
		unlink(pidfile);
	return 0;
}

