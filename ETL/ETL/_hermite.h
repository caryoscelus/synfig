/*! ========================================================================
** Extended Template Library
** Hermite Template Class Implementation
** $Id$
**
** Copyright (c) 2002 Robert B. Quattlebaum Jr.
**
** This package is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License as
** published by the Free Software Foundation; either version 2 of
** the License, or (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** === N O T E S ===========================================================
**
** This is an internal header file, included by other ETL headers.
** You should not attempt to use it directly.
**
** ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __ETL__HERMITE_H
#define __ETL__HERMITE_H

/* === H E A D E R S ======================================================= */

#include "bezier"

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace etl {

template <typename V,typename T=float>
class hermite : public bezier<V,T>
{
public:
	typedef V value_type;
	typedef T time_type;



public:
	hermite() { }
	hermite(const value_type &p1, const value_type &p2, const value_type &t1, const value_type &t2):
    	P1(p1),P2(p2),T1(t1),T2(t2) { sync(); }
	hermite(const value_type &p1, const value_type &p2):
    	P1(p1),P2(p2),T1(p2-p1),T2(p2-p1) { sync(); }

	value_type P1,P2,T1,T2;

	value_type &p1() { return P1; }
	value_type &p2() { return P2; }
	value_type &t1() { return T1; }
	value_type &t2() { return T2; }

	void sync()
	{
		bezier<V,T>::operator[](0)=P1;
		bezier<V,T>::operator[](1)=P1+T1/3;
		bezier<V,T>::operator[](2)=P2-T2/3;
		bezier<V,T>::operator[](3)=P2;

		bezier<V,T>::sync();
	}
};

};

/* === E X T E R N S ======================================================= */

/* === E N D =============================================================== */

#endif
