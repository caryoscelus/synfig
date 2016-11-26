/* === S Y N F I G ========================================================= */
/*!	\file valuenode_add.cpp
**	\brief Simple Catch tests of Add valuenode
**
**	These tests are pretty minimal and can be used as a template
**	when writing new valuenode tests.
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

#include "../valuenode_add.h"
#include <synfig/valuenodes/valuenode_const.h>

using namespace std;
using namespace synfig;

TEST_CASE("Add value node", "[valuenodes]") {
	Type::initialize_all();

	SECTION("Simple addition", "[valuenodes]") {
		ValueBase first = 10;
		ValueBase second = 5;
		auto summ = ValueNode_Add::create(first);
		REQUIRE((*summ)(0) == 10);
		summ->set_link("rhs", ValueNode_Const::create(second));
		REQUIRE((*summ)(0) == 15);
	}
}
