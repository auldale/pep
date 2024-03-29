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
 * UnixListener.cpp
 * 
 * Unix Domain-based SocketListener implementation.
 */

#include "../../internal.h"
#include "SocketListener.h"
#include "UnixListener.h"

#include <xercesc/util/XMLUniDefs.hpp>
#include <xmltooling/XMLToolingConfig.h>
#include <xmltooling/unicode.h>
#include <xmltooling/util/PathResolver.h>
#include <xmltooling/util/XMLHelper.h>

# include <sys/socket.h>
# include <sys/un.h>
# include <unistd.h>
# include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>		/* for chmod() */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

using namespace pep;
using namespace xmltooling;
using namespace xercesc;
using namespace std;

UnixListener::UnixListener(const DOMElement* e) :
		SocketListener(e), m_address(
				XMLHelper::getAttrString(e, getenv("SHIBSP_LISTENER_ADDRESS"), address)), m_bound(
				false) {
	if (m_address.empty())
		m_address = "shibd.sock";
	// TODO ?
	printf(m_address.c_str());
	//XMLToolingConfig::getConfig().getPathResolver()->resolve(m_address,
	//		PathResolver::XMLTOOLING_RUN_FILE);
}

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX 100
#endif

bool UnixListener::create(ShibSocket& sock) const {
	sock = socket(PF_UNIX, SOCK_STREAM, 0);
	if (sock < 0)
		return log_error("socket");
	return true;
}

bool UnixListener::bind(ShibSocket& s, bool force) const {
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, m_address.c_str(), UNIX_PATH_MAX);

	if (force)
		unlink(m_address.c_str());

	if (::bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		log_error("bind");
		close(s);
		return false;
	}

	// Make sure that only the creator can read -- we don't want just
	// anyone connecting, do we?
	if (chmod(m_address.c_str(), 0777) < 0) {
		log_error("chmod");
		close(s);
		unlink(m_address.c_str());
		return false;
	}

	listen(s, 3);
	return m_bound = true;
}

bool UnixListener::connect(ShibSocket& s) const {
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, m_address.c_str(), UNIX_PATH_MAX);

	if (::connect(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		return log_error("connect");
	return true;
}

bool UnixListener::close(ShibSocket& s) const {
	::close(s);
	return true;
}

bool UnixListener::accept(ShibSocket& listener, ShibSocket& s) const {
	s = ::accept(listener, nullptr, nullptr);
	if (s < 0)
		return log_error("accept");
	return true;
}
