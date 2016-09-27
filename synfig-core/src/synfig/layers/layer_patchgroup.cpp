/* === S Y N F I G ========================================================= */
/*!	\file layer_patchgroup.cpp
**	\brief Implementation of the "Group Patch" layer
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007, 2008 Chris Moore
**	Copyright (c) 2011-2013 Carlos López
**	......... ... 2014 Ivan Mahonin
**	......... ... 2016 caryoscelus
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
	param_z_range=ValueBase(bool(false));
	param_z_range_position=ValueBase(Real(0.0));
	param_z_range_depth=ValueBase(Real(0.0));
	param_z_range_blur=ValueBase(Real(0.0));
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
	String s = Layer_PasteCanvas::get_local_name();
	return s.empty() ? _("Group") : '[' + s + ']';
}

Layer::Vocab
Layer_PatchGroup::get_param_vocab()const
{
	Layer::Vocab ret(Layer_PasteCanvas::get_param_vocab());

	ret.push_back(ParamDesc("z_range")
		.set_local_name(_("Z Range"))
		.set_description(_("When checked, only layers inside range are visible"))
		.set_static(true)
	);
	ret.push_back(ParamDesc("z_range_position")
		.set_local_name(_("Z Range Position"))
		.set_description(_("Starting position where layers are visible"))
	);
	ret.push_back(ParamDesc("z_range_depth")
		.set_local_name(_("Z Range Depth"))
		.set_description(_("Depth where layers are visible in range"))
	);
	ret.push_back(ParamDesc("z_range_blur")
		.set_local_name(_("Z Range Blur"))
		.set_description(_("Area where layers inside are partially visible"))
	);
	ret.push_back(ParamDesc("patch")
		.set_local_name(_("Patch"))
		.set_description(_("Patch applied to child node"))
	);

	return ret;
}

bool
Layer_PatchGroup::set_param(const String & param, const ValueBase &value)
{
	IMPORT_VALUE(param_z_range);
	IMPORT_VALUE(param_z_range_position);
	IMPORT_VALUE(param_z_range_depth);
	IMPORT_VALUE(param_z_range_blur);
	IMPORT_VALUE_PLUS(param_patch,
		update_children_patch();
	);
	return Layer_PasteCanvas::set_param(param,value);
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
		
	}
}

void
Layer_PatchGroup::on_childs_changed()
{
	Layer_PasteCanvas::on_childs_changed();
	update_children_patch();
}

ValueBase
Layer_PatchGroup::get_param(const String& param)const
{
	EXPORT_VALUE(param_z_range);
	EXPORT_VALUE(param_z_range_position);
	EXPORT_VALUE(param_z_range_depth);
	EXPORT_VALUE(param_z_range_blur);
	EXPORT_VALUE(param_patch);

	EXPORT_NAME();
	EXPORT_VERSION();

	return Layer_PasteCanvas::get_param(param);
}


void
Layer_PatchGroup::apply_z_range_to_params(ContextParams &cp)const
{
	if (optimized()) return; // z_range already applied while optimizxation

	cp.z_range=param_z_range.get(bool());
	cp.z_range_position=param_z_range_position.get(Real());
	cp.z_range_depth=param_z_range_depth.get(Real());
	cp.z_range_blur=param_z_range_blur.get(Real());
}



void
Layer_PatchGroup::on_canvas_set()
{
	printf("ON_CANVAS_SET\n");
	Layer_PasteCanvas::on_canvas_set();
	update_children_patch();
}

