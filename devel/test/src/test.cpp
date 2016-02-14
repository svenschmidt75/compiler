/***************************************************************************
 *   Copyright (C) 2005 by Sven Schmidt   *
 *   sschmidt@linux   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <vector>
#include "ctest.h"
#include <limits>
#include <string.h>

using namespace std;

//std::ostream &operator<<( std::ostream &os, CTest &a );
class T;

int main( int argc, char *argv[] )
{
	char	*tail;
//	errno = 0;

	unsigned long  a = strtoul( "190", &tail, 10 );

	int a1 = numeric_limits<unsigned char>::min();
	int a2 = numeric_limits<unsigned char>::max();



	vector< vector<T> * >	sven;

	vector<int>	*elem1;
	sven.push_back( elem1 );









	return EXIT_SUCCESS;
}

