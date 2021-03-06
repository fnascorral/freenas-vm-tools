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

#include "../src/Config.hh.in"

#include <unistd.h>
#include <iostream>
#include <Poco/Foundation.h>
#include <Poco/AutoPtr.h>
#include <Poco/SyslogChannel.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/HelpFormatter.h>
#include "../src/Context.hh"
#include "UnixDevice.hh"

class Application: public Poco::Util::ServerApplication
{
protected:
	virtual void defineOptions(Poco::Util::OptionSet &options);
	virtual void displayHelp(const std::string &name, const std::string &value);
	virtual int main(const std::vector<std::string> &args);

    	bool m_daemon = true;
    	std::string m_config;
};


void
Application::defineOptions(Poco::Util::OptionSet &options)
{
	Poco::Util::ServerApplication::defineOptions(options);

	options.addOption(
	    Poco::Util::Option("help", "h", "Display help")
		.required(false)
		.repeatable(false)
		.callback(Poco::Util::OptionCallback<Application>(
		    this, &Application::displayHelp))
	);

	options.addOption(
	    Poco::Util::Option("d", "plugin-dir", "Use plugin directory")
	        .required(false)
	        .repeatable(false)
	        .binding("plugin-dir")
	);
}

void
Application::displayHelp(const std::string &name, const std::string &value)
{
	Poco::Util::HelpFormatter fmt(options());

	fmt.setCommand(commandName());
	fmt.setUsage("OPTIONS");
	fmt.format(std::cout);
	terminate();
}

int
Application::main(const std::vector<std::string> &args)
{
	Poco::AutoPtr<Poco::SyslogChannel> channel(
	    new Poco::SyslogChannel("freenas-vm-tools"));
	Poco::Logger::root().setChannel(channel);

	Context ctx;
        ctx.addDevice(Poco::SharedPtr<UnixDevice>(new UnixDevice()));
        ctx.init(config().getString("plugin-dir", PLUGIN_PATH));
        ctx.run();

	Poco::Logger::root().information("Started");
	waitForTerminationRequest();
	return (0);
}

POCO_SERVER_MAIN(Application);
