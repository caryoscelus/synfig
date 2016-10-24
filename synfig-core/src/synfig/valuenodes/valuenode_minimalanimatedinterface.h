/* === S Y N F I G ========================================================= */
/*!	\file valuenode_minimalanimatedinterface.h
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_VALUENODE_MINIMALANIMATEDINTERFACE_H
#define __SYNFIG_VALUENODE_MINIMALANIMATEDINTERFACE_H

/* === H E A D E R S ======================================================= */

#include <synfig/valuenode.h>
#include <synfig/waypoint.h>

#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

namespace temp_iter {

template <class Iter>
boost::optional<Iter> optional_iter(boost::iterator_range<Iter> range, Iter iter)
{
	if (iter == end(range))
		return boost::none;
	else
		return boost::make_optional(iter);
}

};

namespace valuenodes {

/*! \class AnimatedInterface
 *  \brief The new *interface* for animated value nodes.
 *
 * The interface lets you find, add, remove and change waypoints.
 * It is aware of multiple timelines
 */
template <class WPointer, class WIterator>
class AnimatedInterface : public synfig::ValueNode_Interface
{
public:
	using WRange = boost::iterator_range<WIterator>;

protected:
	virtual WRange get_all() = 0;
	virtual WRange get_timeline(const String& timeline) = 0;

public:
	// Waypoint search functions
	// All of them return optional waypoint pointer
#define ANIMATED_INTERFACE_GET_WAYPOINT virtual boost::optional<WPointer>
#ifdef GET_WP
#error "GET_WP was already defined"
#endif
#define GET_WP ANIMATED_INTERFACE_GET_WAYPOINT
	//! Find waypoint by its UID
	GET_WP get_by_uid(const UniqueID& uid)
	{
		auto range = get_all();
		auto iter = boost::find(range, uid);
		return temp_iter::optional_iter(range, iter);
	}

	//! Find waypoint exactly at `time`
	GET_WP at_time(const Time& time)
	{
		// this is not an effective algo
		// TODO: optional sorting
		auto range = get_timeline(time.get_timeline());
		auto iter = boost::find_if(range, [time](auto const& waypoint) {
			return waypoint.get_time() == time;
		});
		return temp_iter::optional_iter(range, iter);
	}

	//! Find waypoint before `time`
	GET_WP before_time(const Time& time)
	{
		return boost::none;
	}

	//! Find waypoint after `time`
	GET_WP after_time(const Time& time)
	{
		return boost::none;
	}

public:
	//! Add a waypoint
	GET_WP add_waypoint(const synfig::Time& time)
	{
		return boost::none;
	}
#undef GET_WP

public:
	// Waypoint edit functions

	//! Clear the whole timeline
	virtual void clear_timeline(const String& timeline)
	{
	}
	//! Remove waypoints from a certain timeline
	virtual void remove_waypoint(WPointer waypoint)
	{
	}
	//! Process all waypoints
	virtual void apply_function(void f(WPointer waypoint))
	{
	}

public:
	AnimatedInterface(ValueNode& node) : ValueNode_Interface(node) {}
	virtual ~AnimatedInterface()
	{
	}
};

}; // END of namespace valuenodes
}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
