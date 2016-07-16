/*-
 * Copyright 2016 iXsystems, Inc.
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef FREENAS_VM_TOOLS_CONTEXT_HH
#define FREENAS_VM_TOOLS_CONTEXT_HH

#include <sys/cdefs.h>
#include <set>
#include <map>
#include <string>
#include <boost/log/sources/severity_logger.hpp>
#include "device.hh"
#include "server.hh"

class context
{
public:
    void init(const std::string &config_path);
    void add_log_backend(boost::shared_ptr<boost::log::sinks::sink> sink);
    void add_device(std::shared_ptr<device> device);
    int run();
    server &get_server();

private:
    void init_logging();

    server m_server;
    std::shared_ptr<device> m_device;
    std::map<const std::string, service *> m_services;
    boost::log::sources::severity_logger<> m_logger;
};

#define REGISTER_SERVICE(klass) \
        extern "C" klass service; \
        klass service;

#endif //FREENAS_VM_TOOLS_CONTEXT_HH
