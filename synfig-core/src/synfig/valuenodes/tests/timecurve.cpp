/* === S Y N F I G ========================================================= */
/*!	\file timecurve.cpp
**	\brief Simple Catch tests of Time Curve node
**
**	\legal
**	Copyright (c) 2016 caryoscelus
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
**	\endlegal
*/

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../timecurve.h"

using namespace std;
using namespace synfig;
using namespace synfig::valuenodes;
namespace g2 = Geom;

// this is used to get meaningful error messages in Catch REQUIRE
// when dealing with ValueBase
#define TO_DOUBLE(x) x.get(0.0)

TEST_CASE("Time curve value node", "[valuenodes]") {
	Type::initialize_all();

	SECTION("Time curve value at point", "[valuenodes]") {
		auto timecurve = TimeCurve::create(ValueBase(0.0));
		REQUIRE((*timecurve)(0) == 0.0);
		timecurve->get_curve().setPoints(vector<g2::Point>({
			g2::Point(0, 0),
			g2::Point(1, 10),
			g2::Point(2, 10),
			g2::Point(3, 0)
		}));
		REQUIRE(TO_DOUBLE((*timecurve)(3)) == 0.0);
		REQUIRE(TO_DOUBLE((*timecurve)(1.5)) == 7.5);
	}
}
