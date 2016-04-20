// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Daemon.class.cpp                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tpageard <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/04/13 11:33:02 by tpageard          #+#    #+#             //
//   Updated: 2016/04/13 11:37:56 by tpageard         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <netinet/in.h>
#include <sys/select.h>
#include "Tintin_reporter.class.hpp"

#define MYPORT 4242  // the port users will be connecting to
#define BACKLOG 3     // how many pending connections queue will hold
#define MAXDATASIZE 255 // max number of bytes we can get at once
#define CLIENTS_MAX 3

#ifndef DAEMON_CLASS_HPP
# define DAEMON_CLASS_HPP


class Daemon {

public:

	
	// constructor
	Daemon( void );

	// copy by reference
	Daemon( Daemon const &src );

	// overload operator equal
	Daemon	&operator=( Daemon const &rhs );

	void		resetServer( void );
	void		runServer( void );
	void		acceptConnection( void );
	
	// destructor
	~Daemon( void );

private:
	bool				_running;

	int                 _sock;
	struct sockaddr_in  _sin;

	fd_set				_readfds;
	int					_nfds;
	int                 _newfd;
	
	int					_clients[CLIENTS_MAX];
	struct timeval		_timeout;

	Tintin_reporter		_logger;

};

#endif // ************************ DAEMON_CLASS_HPP ************************* //