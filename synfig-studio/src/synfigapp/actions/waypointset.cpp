/* === S Y N F I G ========================================================= */
/*!	\file waypointset.cpp
**	\brief WaypointSet - action used when waypoint is set somewhere
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
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

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <synfig/general.h>

#include "waypointset.h"
#include <synfigapp/canvasinterface.h>

#include <synfigapp/localization.h>

#endif

using namespace std;
using namespace etl;
using namespace synfig;
using namespace synfigapp;
using namespace Action;

/* === M A C R O S ========================================================= */

ACTION_INIT(Action::WaypointSet);
ACTION_SET_NAME(Action::WaypointSet,"WaypointSet");
ACTION_SET_LOCAL_NAME(Action::WaypointSet,N_("Set Waypoint"));
ACTION_SET_TASK(Action::WaypointSet,"set");
ACTION_SET_CATEGORY(Action::WaypointSet,Action::CATEGORY_WAYPOINT);
ACTION_SET_PRIORITY(Action::WaypointSet,0);
ACTION_SET_VERSION(Action::WaypointSet,"0.0");
ACTION_SET_CVS_ID(Action::WaypointSet,"$Id$");

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Action::WaypointSet::WaypointSet()
{
	set_dirty(true);
}

Action::ParamVocab
Action::WaypointSet::get_param_vocab()
{
	ParamVocab ret(Action::CanvasSpecific::get_param_vocab());

	ret.push_back(ParamDesc("value_node",Param::TYPE_VALUENODE)
		.set_local_name(_("Destination ValueNode (Animated)"))
	);

	ret.push_back(ParamDesc("waypoint",Param::TYPE_WAYPOINT)
		.set_local_name(_("Waypoint"))
		.set_desc(_("Waypoint to be changed"))
		.set_supports_multiple()
	);

	return ret;
}

bool
Action::WaypointSet::is_candidate(const ParamList &x)
{
	return candidate_check(get_param_vocab(),x);
}

bool
Action::WaypointSet::set_param(const synfig::String& name, const Action::Param &param)
{
	if(name=="value_node" && param.get_type()==Param::TYPE_VALUENODE)
	{
		value_node=ValueNode_Animated::Handle::cast_dynamic(param.get_value_node());

		return static_cast<bool>(value_node);
	}
	if(name=="waypoint" && param.get_type()==Param::TYPE_WAYPOINT)
	{
		//NOTE: at the moment there is no error checking for multiple sets!!!
		waypoints.push_back(param.get_waypoint());

		return true;
	}

	return Action::CanvasSpecific::set_param(name,param);
}

bool
Action::WaypointSet::is_ready()const
{
	if(!value_node || waypoints.empty())
		return false;
	return Action::CanvasSpecific::is_ready();
}

void
Action::WaypointSet::perform()
{
	// XXX: this doesn't look very elegant
	// i'm just rewriting this with c++11 feature & new animated API,
	// but the logic remains as it was

	ValueNode_Animated::Iter iter;

	vector<ValueNode_Animated::Iter> iters;

	for (auto const& wp : waypoints)
	{
		auto maybe_iter = value_node->get_by_uid(wp);
		if (!maybe_iter.is_initialized())
			throw Error(_("Unable to find waypoint"));
		iters.push_back(*maybe_iter);
	}

	//check to see which valuenodes are going to override because of the time...
	for(auto const& wp : waypoints)
	{
		auto maybe_at_time = value_node->at_time(wp.get_time());

		//we only want to track overwrites (not waypoints that are also being modified)
		if (!maybe_at_time.is_initialized())
			continue;

		auto iter = *maybe_at_time;

		// if iter is not one pointing to our waypoints, record it, and then remove them all later
		// not sure if this can really happen..
		if (std::find(begin(iters), end(iters), iter) == end(iters))
		{
			overwritten_waypoints.push_back(*iter);
		}
	}

	//overwrite all the valuenodes we're supposed to set
	{
		auto i = waypoints.begin();
		for(auto ii = iters.begin(); ii != iters.end() && i != end(waypoints); ++ii, ++i)
		{
			old_waypoints.push_back(**ii);
			**ii = *i; //set the point to the corresponding point in the normal waypoint list
		}
	}

	//remove all the points we're supposed to be overwriting
	for (auto const& wp : overwritten_waypoints)
	{
		value_node->erase(wp);
	}

	// Signal that a valuenode has been changed
	value_node->changed();
}

void
Action::WaypointSet::undo()
{
	ValueNode_Animated::Iter iter;

	for (auto const& wp : old_waypoints)
	{
		auto maybe_iter = value_node->get_by_uid(wp);
		if (!maybe_iter.is_initialized())
			throw Error(_("Unable to find waypoint"));
		auto iter = *maybe_iter;

		//overwrite with old one
		*iter = wp;
	}

	//add back in all the points that we removed before...
	for (auto const& wp : overwritten_waypoints)
	{
		value_node->add(wp);
	}

	// Signal that a valuenode has been changed
	value_node->changed();
}
