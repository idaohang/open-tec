#pragma ident "$Id: $"

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  Copyright 2009, The University of Texas at Austin
//
//============================================================================

#ifndef XCNAVEPHEMERISPNBGPSNAVMSG_HPP
#define XCNAVEPHEMERISPNBGPSNAVMSG_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "CNAVEphemeris.hpp"

using namespace std;

class xCNAVEphemerisPNBgpsNavMsg: public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (xCNAVEphemerisPNBgpsNavMsg);
	CPPUNIT_TEST (firstTest);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);

	protected:
		void firstTest (void);
		bool fileEqualTest (char*, char*);

	private:

};

#endif
