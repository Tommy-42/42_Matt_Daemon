#include "Daemon.class.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>

Daemon::Daemon( void )
{
    std::cout << "Constructor" << std::endl;
    _logger.log(Log::Info, "Server Started");
    
    
    struct protoent     *proto;

    _timeout.tv_sec = 0;
    _timeout.tv_usec = 0;

    std::memset( _clients, 0, 3 * sizeof(int) );
    FD_ZERO( &_readfds );
    _running = false;

    proto = getprotobyname("tcp");
    if( proto == 0 )
    {
        _logger.log(Log::Error, "getprotobyname failure");
        exit(EXIT_FAILURE);
    }

    _sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
    if( _sock == -1 )
    {
        _logger.log(Log::Error, "socket failure");
        exit(EXIT_FAILURE);
    }

    _nfds = _sock;

    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(MYPORT);
    _sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind( _sock, (const struct sockaddr *)&_sin, sizeof(_sin) ) == -1 )
    {
        _logger.log(Log::Error, "bind failure");
        exit(EXIT_FAILURE);
    }

    if( listen(_sock, BACKLOG) == -1 )
    {
        _logger.log(Log::Error, "listen failure");
        exit(EXIT_FAILURE);
    }
}

Daemon::Daemon( Daemon const &src )
{
	*this = src;
	return;
}

Daemon&
Daemon::operator=( Daemon const &rhs )
{
	if( this != &rhs )
	{
        _running = rhs._running;
        _sock = rhs._sock;
        _sin = rhs._sin;
        _readfds = rhs._readfds;
        _nfds = rhs._nfds;
        _newfd = rhs._newfd;
        std::memcpy(_clients, rhs._clients, 3 * sizeof(int));
        _timeout = rhs._timeout;
	}
    return *this;
}

void
Daemon::resetServer( void )
{
    FD_ZERO( &_readfds );
    FD_SET( _sock, &_readfds );
    for( int i = 0; i < CLIENTS_MAX; i++ )
    {
        if( _clients[i] > 0 )
            FD_SET( _clients[i], &_readfds );
    }
}

void
Daemon::runServer( void )
{

    int                 r;
    char                buf[1024];

    _running = true;

    while ( _running )
    {
        resetServer();

        if( select( _nfds + 1, &_readfds, NULL, NULL, &_timeout )  == -1 )
        {
            _logger.log(Log::Error, "select failure");
            exit(EXIT_FAILURE);
        }

        if( FD_ISSET( _sock, &_readfds) )
            acceptConnection();

        for( int i = 0; i < CLIENTS_MAX; i++ )
        {
            if( FD_ISSET(_clients[i], &_readfds) )
            {
                std::memset(buf, 0, 1024);
                if( (r = recv(_clients[i], &buf, 1023, 0)) <= 0 )
                {
                    close(_clients[i]);
                    _clients[i] = 0;
                    _logger.log(Log::Log, "client disconnect");
                    continue;
                }
                buf[r] = 0;
                _logger.log(Log::Log, std::string(buf));
            }
        }
    }
}

void
Daemon::acceptConnection( void )
{
    int                 i;
    struct sockaddr_in  client;
    socklen_t           size = sizeof(client);

    for( i=0; i < CLIENTS_MAX; i++ )
    {
        if( _clients[i] == 0 )
        {
            _newfd = accept( _sock,  (struct sockaddr *)&client, &size);
            
            if( _newfd == -1 )
            {
                _clients[i] = 0;
                _logger.log(Log::Error, "connection refused");
                return;
            }
            _clients[i] = _newfd;
            break;
        }
    }
    if( i != CLIENTS_MAX )
        _nfds = (_newfd > _nfds) ? _newfd : _nfds;
    else
    {
        _logger.log(Log::Log, "no room for new client");
        close( _newfd );
    }
    _logger.log(Log::Info, "Connection accepted.");
}

Daemon::~Daemon( void )
{
	_logger.log(Log::Info, "client disconnect");
    for (int i = 0; i < CLIENTS_MAX; i++)
    {
        if (_clients[i])
            close(_clients[i]);
    }
    close(_sock);
    //close(_lock);
    //remove("/var/lock/matt_daemon.lock");

}
