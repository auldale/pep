/*
 * UnixListener.h
 *
 *  Created on: Mar 25, 2013
 *      Author: watanabe
 */

#ifndef UNIXLISTENER_H_
#define UNIXLISTENER_H_

#include "../../internal.h"
#include "SocketListener.h"

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


using namespace xmltooling;
using namespace xercesc;
using namespace std;

namespace pep {
class UnixListener: virtual public SocketListener {
public:

	static ListenerService* UnixListenerServiceFactory(const DOMElement* const & e) {
		return new UnixListener(e);
	}

	UnixListener(const DOMElement* e);
	~UnixListener() {
		if (m_bound)
			unlink(m_address.c_str());
	}

	bool create(ShibSocket& s) const;
	bool bind(ShibSocket& s, bool force = false) const;
	bool connect(ShibSocket& s) const;
	bool close(ShibSocket& s) const;
	bool accept(ShibSocket& listener, ShibSocket& s) const;

	int send(ShibSocket& s, const char* buf, int len) const {
		return ::send(s, buf, len, 0);
	}

	int recv(ShibSocket& s, char* buf, int buflen) const {
		return ::recv(s, buf, buflen, 0);
	}

private:
	string m_address;
	mutable bool m_bound;
};



static const XMLCh address[] = UNICODE_LITERAL_7(a,d,d,r,e,s,s);}
;

#endif /* UNIXLISTENER_H_ */
