// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tpageard <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/04/12 14:40:20 by tpageard          #+#    #+#             //
//   Updated: 2016/04/13 11:32:45 by tpageard         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Daemon.class.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

int		main(void)
{
	pid_t	pid, s_sid;
	
	pid = fork();
	if ( pid < 0 )
	{
		std::cout << "Error fork();" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if ( pid > 0 )
	{
		std::cout << "Quit Parent" << std::endl;
		exit(EXIT_SUCCESS);
	}

	umask(0);
	s_sid = setsid();
	std::cout << s_sid << std::endl;
	if( s_sid == -1 )
	{
		std::cout << "Error setsid(); " << s_sid << std::endl;
		std::cout << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
	// close(STDERR_FILENO);

	chdir("/nfs/2013/t/tpageard/Projects-github/42_Matt_Daemon/var");

	Daemon daemon;
	daemon.runServer();

	return 0;
}


