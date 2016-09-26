/* === S Y N F I G ========================================================= */
/*!	\file valuenode_patch.h
**	\brief Patch node (header)
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

#ifndef __SYNFIG_VALUENODE_PATCH_H
#define __SYNFIG_VALUENODE_PATCH_H

/* === H E A D E R S ======================================================= */

#include <synfig/valuenode.h>
#include <synfig/localization.h>
#include <map>

/* === M A C R O S ========================================================= */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

class ValueNode_Patch : public ValueNode
{
public:
	typedef etl::handle<ValueNode_Patch> Handle;
	typedef etl::handle<const ValueNode_Patch> ConstHandle;

protected:
	std::map<String, ValueNode::RHandle> params;

public:
	void add_param(String name, ValueNode::RHandle param);

public:
	ValueNode_Patch();
	~ValueNode_Patch();
	virtual ValueNode::Handle clone(etl::loose_handle<Canvas> canvas, const GUID& deriv_guid=GUID())const;

	virtual String get_name()const;
	virtual String get_local_name()const;

	//! Returns the cached times values for all the children (linked Value Nodes)
	virtual void get_times_vfunc(Node::time_set &set) const;

	static ValueNode_Patch* create(const ValueBase &x, etl::loose_handle<Canvas> canvas = 0);
	static bool check_type(Type &type);
};

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
