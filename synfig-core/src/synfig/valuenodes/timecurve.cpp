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

#include "valuenode_bline.h"

#include <synfig/valuenode_registry.h>
#include <synfig/general.h>

/* === U S I N G =========================================================== */

using namespace synfig;
using namespace synfig::valuenodes;

/* === G L O B A L S ======================================================= */

REGISTER_VALUENODE(TimeCurve, RELEASE_VERSION_CURRENT, "timecurve", "Time Curve")

/* === M E T H O D S ======================================================= */

void
TimeCurve::sync_path(Time time) const
{
	auto vertex_list = (*curve_)(time).get_list();

	auto p2g = [](auto p) -> Geom::Point {
		return Geom::Point(p[0], p[1]);
	};
	// converting point/tangent list to Geom::Path
	if (vertex_list.size() > 0)
	{
		for (auto i = 1; i < vertex_list.size(); ++i)
		{
			auto prev = vertex_list[i-1].get(BLinePoint());
			auto prev_p = prev.get_vertex();
			auto prev_t = prev_p+prev.get_tangent2()/3; // tangents are weird

			auto next = vertex_list[i].get(BLinePoint());
			auto next_p = next.get_vertex();
			auto next_t = next_p-next.get_tangent1()/3; // tangents are weird

			if (i == 1) {
				curve_cached.start(p2g(prev_p));
			}
			curve_cached.appendNew<Geom::CubicBezier>(p2g(prev_t), p2g(next_t), p2g(next_p));
		}
		// make sure curve is not closed and we don't get extra roots
		curve_cached.close(false);
	}
}

Geom::Path const&
TimeCurve::get_curve(Time time) const {
	sync_path(time);
	return curve_cached;
}

ValueBase
TimeCurve::operator()(Time time) const
{
	auto const& curve = get_curve(time);
	// TODO: deal with time properly
	auto curve_roots = curve.roots((double)time, Geom::X);
	assert(curve_roots.size() <= 1);
	// TODO: use first or last value depending on time
	auto t = curve_roots.size() == 1 ? curve_roots[0] : Geom::PathTime();
	auto result = curve.valueAt(t, Geom::Y);
	auto vb = ValueBase(get_type());
	vb.set(result);
	return vb;
}

TimeCurve::TimeCurve(const ValueBase& value) :
	LinkableValueNode(value.get_type()),
	curve_cached(Geom::Path())
{
	auto& type = value.get_type();

	set_link("path", new ValueNode_BLine());
}

bool
TimeCurve::check_type(Type &type)
{
	return type==type_angle
		|| type==type_integer
		|| type==type_real
		|| type==type_time;
}

bool
TimeCurve::set_link_vfunc(int i, ValueNode::Handle value)
{
	// TODO: get rid of manual set_link_vfunc
	assert(i == 0);
	// TODO: get rid of this macro
	CHECK_TYPE_AND_SET_VALUE(curve_, type_list);
	return false;
}

ValueNode::LooseHandle
TimeCurve::get_link_vfunc(int i) const
{
	// TODO: get rid of manual code
	if (i == 0)
		return curve_;
	return nullptr;
}

LinkableValueNode::Vocab
TimeCurve::get_children_vocab_vfunc() const
{
	auto vocab = LinkableValueNode::Vocab();
	vocab.push_back(ParamDesc(ValueBase(), "path"));
	return vocab;
}
