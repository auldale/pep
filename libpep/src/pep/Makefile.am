
lib_LTLIBRARIES = libpep.la libpep-lite.la

libincludedir = $(includedir)/pep

apiincludedir = $(includedir)/pep/api

confincludedir = $(includedir)/pep/config

coreincludedir = $(includedir)/pep/core

handlincludedir = $(includedir)/pep/handler

remincludedir = $(includedir)/pep/remoting

remimplincludedir = $(includedir)/pep/remoting/impl

utilincludedir = $(includedir)/pep/util

libinclude_HEADERS = \
	base.h \
	exceptions.h \
	internal.h

apiinclude_HEADERS = \
	api/api.h

confinclude_HEADERS = \
	config/PEPConfig.h \
	config/ConfigConstants.h

coreinclude_HEADERS = \
	core/AbstractPEPRequest.h \
	core/Application.h \
	core/PEP.h \
	core/PEPRequest.h \
	core/PEPRequestImpl.h \
	core/XACMLConstants.h \
	core/XACMLRequestBuilder.h \
	core/XMLApplication.h \
	core/XMLConstants.h

handlinclude_HEADERS = \
	handler/AbstractHandler.h \
	handler/AuthorizationHandler.h \
	handler/Handler.h \
	handler/RemotedHandler.h

reminclude_HEADERS = \
	remoting/ddf.h \
	remoting/ListenerService.h

remimplinclude_HEADERS = \
	remoting/impl/SocketListener.h \
	remoting/impl/UnixListener.h	
	
utilinclude_HEADERS = \
	util/CGIParser.h \
	util/DOMPropertySet.h \
	util/IPRange.h \
	util/PropertySet.h \
	util/SPConstants.h
	
libpep_la_SOURCES = \
	config/PEPConfig.cpp \
	config/ConfigConstants.cpp \
	core/AbstractPEPRequest.cpp \
	core/Application.cpp \
	core/PEP.cpp \
	core/PEPRequestImpl.cpp \
	core/XACMLConstants.cpp \
	core/XACMLRequestBuilder.cpp \
	core/XMLApplication.cpp \
	handler/impl/AbstractHandler.cpp \
	handler/impl/AuthorizationHandler.cpp \
	handler/impl/RemotedHandler.cpp \
	remoting/impl/ListenerService.cpp \
	remoting/impl/SocketListener.cpp \
	remoting/impl/TCPListener.cpp \
	remoting/impl/UnixListener.cpp \
	remoting/impl/ddf.cpp \
	util/CGIParser.cpp \
	util/DOMPropertySet.cpp \
	util/IPRange.cpp \
	util/SPConstants.cpp
	
libpep_lite_la_SOURCES = \
	api/api.cpp \
	config/PEPConfig.cpp \
	config/ConfigConstants.cpp \
	core/AbstractPEPRequest.cpp \
	core/Application.cpp \
	core/PEP.cpp \
	core/PEPRequestImpl.cpp \
	core/XACMLConstants.cpp \
	core/XACMLRequestBuilder.cpp \
	core/XMLApplication.cpp \
	handler/impl/AbstractHandler.cpp \
	handler/impl/AuthorizationHandler.cpp \
	handler/impl/RemotedHandler.cpp \
	remoting/impl/ListenerService.cpp \
	remoting/impl/SocketListener.cpp \
	remoting/impl/TCPListener.cpp \
	remoting/impl/UnixListener.cpp \
	remoting/impl/ddf.cpp \
	util/CGIParser.cpp \
	util/DOMPropertySet.cpp \
	util/IPRange.cpp \
	util/SPConstants.cpp	
	
libpep_la_CPPFLAGS = ${DEPS_CFLAGS} -g3 -O0
libpep_la_LIBADD = ${DEPS_LIBS} -lxmltooling 

libpep_lite_la_CPPFLAGS = ${DEPS_CFLAGS} -g3 -O0 -DPEP_LITE -DXMLTOOLING_LITE
libpep_lite_la_LIBADD = ${DEPS_LIBS} -lxmltooling-lite

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pep.pc
