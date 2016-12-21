/* === S Y N F I G ========================================================= */
/*!	\file timecurve.cpp
**	\brief TimeCurve node
**
**	Time curve is a value node that uses curve in time/value space to
**	interpolate its values.
**
**	\legal
**	Copyright (c) 2016 caryoscelus
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/

/* === H E A D E R S ======================================================= */

#include "timecurve.h"

#include <synfig/valuenode_registry.h>

/* === U S I N G =========================================================== */

using namespace std;
using namespace synfig;
using namespace synfig::valuenodes;

/* === G L O B A L S ======================================================= */

REGISTER_VALUENODE(TimeCurve, RELEASE_VERSION_CURRENT, "timecurve", "Time Curve")

/* === M E T H O D S ======================================================= */

ValueBase
TimeCurve::operator()(Time time) const
{
	// TODO: deal with time properly
	auto curve_roots = curve.roots((double)time, Geom::X);
	assert(curve_roots.size() <= 1);
	// TODO: use first or last value depending on time
	auto t = curve_roots.size() == 1 ? curve_roots[0] : 0;
	auto result = curve.valueAt(t, Geom::Y);
	auto vb = ValueBase(get_type());
	vb.set(result);
	return vb;
}

TimeCurve::TimeCurve(const ValueBase& value) :
	LinkableValueNode(value.get_type()),
	curve(Geom::BezierCurveN<3>())
{}

bool
TimeCurve::check_type(Type &type)
{
	return type==type_angle
		|| type==type_integer
		|| type==type_real
		|| type==type_time;
}

bool
TimeCurve::set_link_vfunc(int i,ValueNode::Handle value)
{
	return false;
}

ValueNode::LooseHandle
TimeCurve::get_link_vfunc(int i) const
{
	return nullptr;
}

LinkableValueNode::Vocab
TimeCurve::get_children_vocab_vfunc() const
{
	return LinkableValueNode::Vocab();
}
