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

#include <cstddef>

#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/any_range.hpp>

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

namespace temp_iter {

template <class Iter>
boost::optional<Iter> optional_iter(boost::iterator_range<Iter> range, Iter iter)
{
	if (iter == boost::end(range))
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
class AnimatedInterface : public synfig::ValueNode_Interface
{
public:
	using Range = boost::any_range<Waypoint, boost::forward_traversal_tag, Waypoint&, std::ptrdiff_t>;
	using Iter = Range::iterator;
	using MaybeIter = boost::optional<Iter>;

	using ConstRange = boost::any_range<Waypoint, boost::forward_traversal_tag, const Waypoint&, std::ptrdiff_t>;
	using ConstIter = ConstRange::iterator;
	using MaybeConstIter = boost::optional<ConstIter>;

// protected:
public:
	virtual Range access_all() = 0;
	virtual Range access_timeline(const String& timeline) = 0;
	virtual ConstRange get_all() const = 0;
	virtual ConstRange get_timeline(const String& timeline) const = 0;

public:
	// Waypoint search functions
	// All of them return optional waypoint pointer
	//! Find waypoint by its UID
	virtual MaybeIter get_by_uid(const UniqueID& uid);

	//! Find waypoint exactly at `time`
	virtual MaybeIter at_time(const Time& time);

	//! Find waypoint before `time`
	virtual MaybeIter before_time(const Time& time);

	//! Find waypoint after `time`
	virtual MaybeIter after_time(const Time& time);

public:
	virtual bool empty() const;
	virtual size_t count_all() const;
	virtual size_t count_timeline(const String& timeline) const;

protected:
	virtual MaybeIter before_or_after_time(const Time& time, std::function<bool(const Time&, const Time&)> cmp);

public:
	//! Add a waypoint
	virtual MaybeIter add_waypoint(const synfig::Time& time);

public:
	// Waypoint edit functions

	//! Clear everything
	virtual void clear_all();
	//! Clear the whole timeline
	virtual void clear_timeline(const String& timeline);
	//! Remove a waypoint
	virtual void erase(const UniqueID& uid);
	//! Process all waypoints
	virtual void apply_function(std::function<void (Waypoint& wp)> f);

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
