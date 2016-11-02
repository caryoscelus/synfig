/* === S Y N F I G ========================================================= */
/*!	\file valuenode_minimalanimatedinterface.cpp
**	\brief New animated node interface
**
**	$Id$
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
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#include "valuenode_minimalanimatedinterface.h"

/* === U S I N G =========================================================== */

using namespace synfig;
using namespace synfig::valuenodes;

/* === M A C R O S ========================================================= */

/* === M E T H O D S ======================================================= */

AnimatedInterface::MaybeIter
AnimatedInterface::access_by_uid(const UniqueID& uid)
{
	auto range = access_all();
	auto iter = boost::find(range, uid);
	return temp_iter::optional_iter(range, iter);
}

AnimatedInterface::Iter
AnimatedInterface::direct_access(const UniqueID& uid)
{
	// TODO: this is supposed to be effective
	return *access_by_uid(uid);
}

AnimatedInterface::MaybeConstIter
AnimatedInterface::get_by_uid(const UniqueID& uid) const
{
	auto range = get_all();
	auto iter = boost::find(range, uid);
	return temp_iter::optional_iter(range, iter);
}

AnimatedInterface::MaybeConstIter
AnimatedInterface::at_time(const Time& time) const
{
	// this is not an effective algo
	// TODO: optional sorting
	auto range = get_timeline(time.get_timeline());
	auto iter = boost::find_if(range, [time](auto const& waypoint) {
		return waypoint.get_time() == time;
	});
	return temp_iter::optional_iter(range, iter);
}

AnimatedInterface::MaybeConstIter
AnimatedInterface::before_or_after_time(const Time& time, const Time& start_from, std::function<bool(const Time&, const Time&)> cmp) const
{
	// generic method
	// override this in subclass for effectiveness
	auto uid = UniqueID::nil();
	auto last_time = start_from;
	scan_function([&uid, &last_time, time, cmp](auto const& waypoint) {
		auto t = waypoint.get_time();
		if (!time.comparable(t))
			return;
		if (cmp(last_time, t) && cmp(t, time))
		{
			last_time = t;
			uid = (UniqueID)waypoint;
		}
	});
	if (!uid)
		return boost::none;
	assert(get_by_uid(uid));
	return get_by_uid(uid);
}

AnimatedInterface::MaybeConstIter
AnimatedInterface::before_time(const Time& time) const
{
	return before_or_after_time(time, Time::begin(time), [](auto const& a, auto const& b) -> bool {
		return a < b;
	});
}

AnimatedInterface::MaybeConstIter
AnimatedInterface::after_time(const Time& time) const
{
	return before_or_after_time(time, Time::end(time), [](auto const& a, auto const& b) -> bool {
		return a > b;
	});
}

bool
AnimatedInterface::empty() const
{
	return count_all() == 0;
}

size_t
AnimatedInterface::count_all() const
{
	return boost::size(get_all());
}

size_t
AnimatedInterface::count_timeline(const String& timeline) const
{
	return boost::size(get_timeline(timeline));
}

AnimatedInterface::MaybeIter
AnimatedInterface::new_waypoint(const Time& time, ValueBase value)
{
	return add_waypoint(Waypoint(value, time));
}

AnimatedInterface::MaybeIter
AnimatedInterface::new_linked_waypoint(const Time& time, ValueNode::Handle node)
{
	return add_waypoint(Waypoint(node, time));
}

void
AnimatedInterface::clear_all()
{
}

void
AnimatedInterface::clear_timeline(const String& timeline)
{
}

void
AnimatedInterface::erase(const UniqueID& uid)
{
}

void
AnimatedInterface::scan_function(std::function<void (Waypoint const& wp)> f) const
{
	for (auto const& wp : get_all())
	{
		f(wp);
	}
}

void
AnimatedInterface::apply_function(std::function<void (Waypoint& wp)> f)
{
	for (auto& wp : access_all())
	{
		f(wp);
	}
}

boost::optional<ValueBase>
AnimatedInterface::value_at_time(const Time& time) const
{
	using boost::make_optional;
	using boost::none;

	auto at = at_time(time);
	if (at)
		return make_optional((*at)->get_value(time));

	auto before = before_time(time);
	auto after = after_time(time);

	if (!before && !after)
		return none;

	if (before && !after)
		return make_optional((*before)->get_value(time));

	if (after && !before)
		return make_optional((*after)->get_value(time));

	// TODO: implement interpolation
	return make_optional((*before)->get_value(time));
}

// deprecated
Waypoint
AnimatedInterface::new_waypoint_at_time(const Time& time) const
{
	Waypoint waypoint;
// 	waypoint.set_value((*this)(time));
	waypoint.set_time(time);
// 	waypoint.set_parent_value_node(&const_cast<*>(this)->node());
	return waypoint;
}

bool
AnimatedInterface::waypoint_is_only_use_of_valuenode(Waypoint &waypoint) const
{
	// small lie could never hurt, right?
	return false;
}

int
AnimatedInterface::collect_waypoints(const Time& begin, const Time& end, std::vector<Waypoint*>& result)
{
	if (!begin.comparable(end))
		return 0;

	// FIXME: very dumb implementation
	apply_function([&result, begin, end](Waypoint& wp) {
		auto t = wp.get_time();
		if (t.comparable(begin) && t >= begin && t <= end)
		{
			result.push_back(&wp);
		}
	});
}
