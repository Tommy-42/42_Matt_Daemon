#include "Tintin_reporter.class.hpp"

#include <iostream>
#include <string.h>
#include <ctime>

Tintin_reporter::Tintin_reporter( void )
{
	_log.open("./var/log/matt_daemon/matt_daemon.log", std::ios::out | std::ios::app);
}

Tintin_reporter::Tintin_reporter( Tintin_reporter const &src )
{
	*this = src;
	return;
}

Tintin_reporter&
Tintin_reporter::operator=( Tintin_reporter const &rhs )
{
	if( this != &rhs ) {
		_log.copyfmt(rhs._log);
		_log.clear(rhs._log.rdstate());
		_log.basic_ios<char>::rdbuf(rhs._log.rdbuf());
	}
    return *this;
}

std::string
Tintin_reporter::getCurrentTime( void )
{
	std::time_t		now;
	char 			strtime[65];

	now = std::time(NULL);
	std::strftime(strtime, sizeof(strtime), "[%d/%m/%Y-%H:%M:%S]", std::localtime(&now));

	return( std::string(strtime) );
}

void
Tintin_reporter::log( Log type, std::string msg )
{
	std::string logType;
	std::string logMsg;

	std::string now = getCurrentTime();

	if (!msg.empty() && msg[msg.length() - 1] == '\n')
		msg.erase(msg.length() - 1);

	if (!_log.is_open())
		_log.open("./var/log/matt_daemon/matt_daemon.log", std::ios::out | std::ios::app);
/*
	switch(type) {
		case Log::Info:
			logType = "[ INFO ]";
		case Log::Error:
			logType = "[ ERROR ]";
		case Log::Log:
			logType = "[ Log ]";
		case LogType:
			logType = "[ SIG ]";
	}
*/
	if (type == Log::Info)
		logType = " [INFO]  ";
	else if (type == Log::Error)
		logType = " [ERROR] ";
	else if (type == Log::Log)
		logType = " [LOG]   ";
	else
		logType = " [SIG]   ";

	logMsg = now + logType + msg;
	if ( _log.is_open() )
		_log << logMsg << std::endl;
}

Tintin_reporter::~Tintin_reporter( void ) {}
