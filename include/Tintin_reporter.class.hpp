// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Tintin_reporter.class.cpp                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tpageard <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/04/13 11:33:02 by tpageard          #+#    #+#             //
//   Updated: 2016/04/13 11:37:56 by tpageard         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <fstream>
#include <ctime>
#include <string>

#ifndef TINTIN_REPORTER_CLASS_HPP
# define TINTIN_REPORTER_CLASS_HPP

enum class Log {
	Info,
	Error,
	Log,
	Sig
};

class Tintin_reporter {

public:
	
	// constructor
	Tintin_reporter( void );

	// copy by reference
	Tintin_reporter( Tintin_reporter const &src );

	// overload operator equal
	Tintin_reporter	&operator=( Tintin_reporter const &rhs );
	
	// logger
	void		log(Log type, std::string msg);
	// timer
	std::string getCurrentTime(void);

	// destructor
	~Tintin_reporter( void );

private:

	std::ofstream	_log;

};

#endif // ********************* TINTIN_REPORTER_CLASS_HPP ******************* //