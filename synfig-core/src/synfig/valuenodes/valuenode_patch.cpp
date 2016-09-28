/* === S Y N F I G ========================================================= */
/*!	\file valuenode_patch.cpp
**	\brief Patch node
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

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "valuenode_patch.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

ValueNode_Patch::ValueNode_Patch()
{
}

ValueNode_Patch::~ValueNode_Patch()
{
}

ValueNode::Handle
ValueNode_Patch::clone(etl::loose_handle<Canvas> canvas, const GUID& deriv_guid)const
{
	return 0;
}

void ValueNode_Patch::get_times_vfunc(Node::time_set &set)const
{
    
}

String ValueNode_Patch::get_name()const
{
	return "patch";
}

String ValueNode_Patch::get_local_name()const
{
	return _("Patch");
}

ValueNode_Patch* ValueNode_Patch::create(const ValueBase &x, etl::loose_handle<Canvas> canvas)
{
	return 0;
}

bool
ValueNode_Patch::check_type(Type & /* type */)
{
	return true;
}

void ValueNode_Patch::add_param(String name, ValueNode::RHandle param)
{
	params.insert(pair<String,ValueNode::RHandle>(name, param));
}

/* === M E T H O D S ======================================================= */
