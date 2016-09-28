/* === S Y N F I G ========================================================= */
/*!	\file layer_patchgroup.cpp
**	\brief "Group Patch" layer
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

#include "layer_patchgroup.h"

#include "../valuenodes/valuenode_patch.h"

#include <synfig/general.h>
#include <synfig/localization.h>

#include <synfig/canvas.h>
#include <synfig/context.h>
#include <synfig/string.h>
#include <synfig/time.h>
#include <synfig/paramdesc.h>
#include <synfig/value.h>
#include <synfig/valuenode.h>


#endif

/* === U S I N G =========================================================== */

using namespace etl;
using namespace std;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === C L A S S E S ======================================================= */

/* === G L O B A L S ======================================================= */

SYNFIG_LAYER_INIT(Layer_PatchGroup);
SYNFIG_LAYER_SET_NAME(Layer_PatchGroup,"patchgroup");
SYNFIG_LAYER_SET_LOCAL_NAME(Layer_PatchGroup,N_("Patch Group"));
SYNFIG_LAYER_SET_CATEGORY(Layer_PatchGroup,N_("Other"));
SYNFIG_LAYER_SET_VERSION(Layer_PatchGroup,"0.1");
SYNFIG_LAYER_SET_CVS_ID(Layer_PatchGroup,"$Id$");

/* === M E T H O D S ======================================================= */

Layer_PatchGroup::Layer_PatchGroup()
{

	param_patch=ValueBase(new ValueNode_Patch());

	SET_INTERPOLATION_DEFAULTS();
	SET_STATIC_DEFAULTS();
}

Layer_PatchGroup::~Layer_PatchGroup()
{
}

String
Layer_PatchGroup::get_local_name()const
{
	String s = Layer_Group::get_local_name();
	return s.empty() ? _("Patch Group") : '[' + s + ']';
}

Layer::Vocab
Layer_PatchGroup::get_param_vocab()const
{
	Layer::Vocab ret(Layer_Group::get_param_vocab());

	ret.push_back(ParamDesc("patch")
		.set_local_name(_("Patch"))
		.set_description(_("Patch applied to child node"))
	);

	return ret;
}

bool
Layer_PatchGroup::set_param(const String & param, const ValueBase &value)
{
	IMPORT_VALUE_PLUS(param_patch,
		update_children_patch();
	);
	return Layer_Group::set_param(param,value);
}

void
Layer_PatchGroup::update_children_patch()
{
	printf("applying patch..\n");
	Canvas::Handle canvas = get_sub_canvas();
	if (!canvas) {
		printf("canvas not ready yet!\n");
		return;
	}
	for (auto i = canvas->get_independent_context(); *i; ++i)
	{
		Layer::Handle child = *i;
		printf("%s\n", child->get_name().c_str());
		for (const auto &p : param_patch.get(ValueNode_Patch::Handle())->params) {
			child->connect_dynamic_param(p.first, p.second);
		}
	}
}

void
Layer_PatchGroup::on_childs_changed()
{
	Layer_Group::on_childs_changed();
	update_children_patch();
}

ValueBase
Layer_PatchGroup::get_param(const String& param)const
{
	EXPORT_VALUE(param_patch);

	EXPORT_NAME();
	EXPORT_VERSION();

	return Layer_Group::get_param(param);
}

void
Layer_PatchGroup::on_canvas_set()
{
	Layer_Group::on_canvas_set();
	update_children_patch();
}
