/* === S Y N F I G ========================================================= */
/*!	\file animated.cpp
**	\brief New animated node
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

#include "animated.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <synfig/localization.h>

/* === N A M E S P A C E S ================================================= */

using namespace synfig;
using namespace synfig::valuenodes;
using namespace std;
using namespace boost::multi_index;
using boost::iterator_range;

/* === T Y P E D E F S ===================================================== */

using Container = boost::multi_index_container<
	Waypoint,
	indexed_by<
		// indexed by uid
		hashed_unique<
			identity<Waypoint>
		>,
		// indexed by time
		ordered_unique<
			const_mem_fun<Waypoint, const Time&, &Waypoint::get_time>
		>
	>
>;

/* === C L A S S E S & S T R U C T S ======================================= */

struct Animated::Impl {
	Container container;
};

/* === M E T H O D S ======================================================= */

Animated::Animated(ValueNode& node):
	AnimatedInterface(node),
	impl(new Animated::Impl())
{
}

Animated::Animated(Type& type):
	AnimatedInterface(*(ValueNode*)this),
	impl(new Animated::Impl())
{
// 	set_type(type);
}

Animated::~Animated() = default;

Animated::Range
Animated::access_all()
{
	auto cont = impl->container;
	return iterator_range<decltype(cont)::iterator>(begin(cont), end(cont));
}

Animated::ConstRange
Animated::get_all() const
{
	auto cont = impl->container;
	return iterator_range<decltype(cont)::iterator>(begin(cont), end(cont));
}

Animated::Range
Animated::access_timeline(const String& timeline)
{
	auto& view = impl->container.get<1>();
	return iterator_range<
		remove_reference<decltype(view)>::type::iterator
	>(begin(view), end(view));
}

Animated::ConstRange
Animated::get_timeline(const String& timeline) const
{
	auto& view = impl->container.get<1>();
	return iterator_range<
		remove_reference<decltype(view)>::type::iterator
	>(begin(view), end(view));
}

// Animated::MaybeIter
// Animated::access_by_uid(UniqueId const& uid)
// {
// 	auto& view = impl->container.get<0>();
// 	return view.find(uid);
// }

Animated::MaybeIter
Animated::add_waypoint(Waypoint waypoint)
{
	impl->container.insert(waypoint);
	return access_by_uid(waypoint);
}

ValueBase
Animated::operator()(Time time) const
{
	return *value_at_time(time);
}

void
Animated::get_times_vfunc(Node::time_set &set) const
{
	for (auto const& wp : get_all())
	{
		set.insert(wp.to_timepoint());
	}
}

ValueNode::Handle
Animated::clone(Canvas::LooseHandle canvas, const synfig::GUID& deriv_guid)const
{
	// Why?
	auto new_guid = get_guid()^deriv_guid;

	{
		// TODO: check this
		ValueNode* existing = find_value_node(new_guid).get();
		if (existing)
			return existing;
	}

	auto cloned = new Animated(get_type());

	cloned->set_guid(new_guid);
	cloned->set_parent_canvas(canvas);
	for (auto wp : get_all())
	{
		cloned->add_waypoint(wp.clone(canvas, new_guid));
	}
	return cloned;
}

String
Animated::get_name() const
{
	return "animated";
}

String
Animated::get_local_name() const
{
	return _("Animated");
}

String
Animated::get_string() const
{
	return "Animated";
}
