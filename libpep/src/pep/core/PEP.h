/*
 * PEP.h
 *
 *  Created on: Mar 31, 2013
 *      Author: watanabe
 */

#ifndef PEP_H_
#define PEP_H_

#include <xercesc/dom/DOMElement.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

#include <util/PropertySet.h>

#include <set>
#include <vector>
#include <xmltooling/Lockable.h>
#include <remoting/ListenerService.h>

namespace pep {

class Application;


class Remoted;
class PEPRequest;


class PEP {
public:
	PEP();
	virtual ~PEP();

	bool validate() const;

	void setListenerServiceFactory(ListenerServiceFactory factoryFn,
			const xercesc::DOMElement* const & config);

	void regApplication(boost::shared_ptr<Application> application);
	/**
	 * Returns an Application instance matching the specified ID.
	 *
	 * @param applicationId the ID of the application, or nullptr for the default
	 * @return  pointer to the application, or nullptr
	 */
	virtual const Application* getApplication(const char* applicationId) const;

	/**
	 * Register for a message. Returns existing remote service, allowing message hooking.
	 *
	 * @param address   message address to register
	 * @param svc       pointer to remote service
	 * @return  previous service registered for message, if any
	 */
	virtual Remoted* regListener(const char* address, Remoted* svc);

	/**
	 * Unregisters service from an address, possibly restoring an original.
	 *
	 * @param address   message address to modify
	 * @param current   pointer to unregistering service
	 * @param restore   service to "restore" registration for
	 * @return  true iff the current service was still registered
	 */
	virtual bool unregListener(const char* address, Remoted* current,
			Remoted* restore = nullptr);

	/**
	 * Returns current service registered at an address, if any.
	 *
	 * @param address message address to access
	 * @return  registered service, or nullptr
	 */
	virtual Remoted* lookupListener(const char* address) const;

	virtual std::pair<bool, long> doAuthorization(PEPRequest& request) const;

	virtual std::pair<bool, long> doHandler(PEPRequest& request) const;

	ListenerService* getListenerService() const {
		return m_ListenerService;
	}

private:
	ListenerService* m_ListenerService;
	std::map<std::string, boost::shared_ptr<Application> > m_AppMap;
	std::map<std::string, Remoted*> m_listenerMap;
};

} /* namespace pep */
#endif /* PEP_H_ */
