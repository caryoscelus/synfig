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
AnimatedInterface::get_by_uid(const UniqueID& uid)
{
	auto range = access_all();
	auto iter = boost::find(range, uid);
	return temp_iter::optional_iter(range, iter);
}

AnimatedInterface::MaybeIter
AnimatedInterface::at_time(const Time& time)
{
	// this is not an effective algo
	// TODO: optional sorting
	auto range = access_timeline(time.get_timeline());
	auto iter = boost::find_if(range, [time](auto const& waypoint) {
		return waypoint.get_time() == time;
	});
	return temp_iter::optional_iter(range, iter);
}

AnimatedInterface::MaybeIter
AnimatedInterface::before_or_after_time(const Time& time, std::function<bool(const Time&, const Time&)> cmp)
{
	// generic method
	// override this in subclass for effectiveness
	auto uid = UniqueID::nil();
	auto last_time = Time::begin(time);
	apply_function([&uid, &last_time, time, cmp](auto const& waypoint) {
		auto t = waypoint.get_time();
		if (!time.comparable(t))
			return;
		if (cmp(last_time, t) && cmp(t, time))
		{
			last_time = t;
			uid = waypoint;
		}
	});
	if (!uid)
		return boost::none;
	return get_by_uid(uid);
}

AnimatedInterface::MaybeIter
AnimatedInterface::before_time(const Time& time)
{
	return before_or_after_time(time, [](auto const& a, auto const& b) -> bool {
		return a < b;
	});
}

AnimatedInterface::MaybeIter
AnimatedInterface::after_time(const Time& time)
{
	return before_or_after_time(time, [](auto const& a, auto const& b) -> bool {
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
AnimatedInterface::add_waypoint(const Time& time)
{
	return boost::none;
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
AnimatedInterface::apply_function(std::function<void (Waypoint& wp)> f)
{
	for (auto& wp : access_all())
	{
		f(wp);
	}
}
