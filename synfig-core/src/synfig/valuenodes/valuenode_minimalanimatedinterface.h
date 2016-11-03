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
	template <class W>
	using GenericRange = boost::any_range<Waypoint, boost::forward_traversal_tag, W, std::ptrdiff_t>;

	using Range = GenericRange<Waypoint&>;
	using Iter = Range::iterator;
	using MaybeIter = boost::optional<Iter>;

	using ConstRange = GenericRange<const Waypoint&>;
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
	virtual MaybeConstIter get_by_uid(const UniqueID& uid) const;

	//! Find waypoint exactly at `time`
	virtual MaybeConstIter at_time(const Time& time) const;

	//! Find waypoint before `time`
	virtual MaybeConstIter before_time(const Time& time) const;

	//! Find waypoint after `time`
	virtual MaybeConstIter after_time(const Time& time) const;

public:
	// Waypoint accessor search functions
	virtual MaybeIter access_by_uid(const UniqueID& uid);
	virtual MaybeIter access(MaybeConstIter iter);
	// Unsafe
	virtual Iter direct_access(const UniqueID& uid);

public:
	virtual bool empty() const;
	virtual size_t count_all() const;
	virtual size_t count_timeline(const String& timeline) const;

protected:
	virtual MaybeConstIter before_or_after_time(const Time& time, const Time& start_from, std::function<bool(const Time&, const Time&)> cmp) const;

public:
	//! Add a waypoint, using calculated value
// 	virtual MaybeIter new_auto_waypoint(const synfig::Time& time);
	//! Add a waypoint with given value
	virtual MaybeIter new_waypoint(const Time& time, ValueBase value);
	//! Add a waypoint linked to another valuenode
	virtual MaybeIter new_linked_waypoint(const Time& time, ValueNode::Handle node);

protected:
	virtual MaybeIter add_waypoint(Waypoint waypoint) = 0;

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
	//! Process waypoints read-only
	virtual void scan_function(std::function<void (Waypoint const& wp)> f) const;

public:
	virtual boost::optional<ValueBase> value_at_time(const Time& time) const;

public:
	// deprecated interface
	//! Returns a new waypoint at a given time but it is not inserted in the Waypoint List.
	/*! \note this does not add any waypoint to the ValueNode! */
	[[deprecated]]
	Waypoint new_waypoint_at_time(const Time& t) const;
	[[deprecated]]
	bool waypoint_is_only_use_of_valuenode(Waypoint &waypoint) const;
	[[deprecated]]
	int collect_waypoints(const Time& begin, const Time& end, std::vector<Waypoint*>& result);

public:
	explicit AnimatedInterface(ValueNode& node) : ValueNode_Interface(node) {}
	virtual ~AnimatedInterface() = default;
};

}; // END of namespace valuenodes
}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
