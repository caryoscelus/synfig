/* === S Y N F I G ========================================================= */
/*!	\file layer_timepatch.cpp
**	\brief "Time Patch" layer
**
**	$Id$
**	This layer patches its children, cloning their waypoints
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

#include "layer_timepatch.h"

#include <synfig/general.h>
#include <synfig/localization.h>

#include <synfig/time.h>
#include <synfig/canvas.h>
#include <synfig/context.h>
#include <synfig/string.h>
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

SYNFIG_LAYER_INIT(Layer_TimePatch);
SYNFIG_LAYER_SET_NAME(Layer_TimePatch,"timepatchgroup");
SYNFIG_LAYER_SET_LOCAL_NAME(Layer_TimePatch,N_("Time Patch Group"));
SYNFIG_LAYER_SET_CATEGORY(Layer_TimePatch,N_("Time"));
SYNFIG_LAYER_SET_VERSION(Layer_TimePatch,"0.1");
SYNFIG_LAYER_SET_CVS_ID(Layer_TimePatch,"$Id$");

/* === M E T H O D S ======================================================= */

Layer_TimePatch::Layer_TimePatch()
{

	param_time_patch=ValueBase(Time());

	SET_INTERPOLATION_DEFAULTS();
	SET_STATIC_DEFAULTS();
}

Layer_TimePatch::~Layer_TimePatch()
{
}

String
Layer_TimePatch::get_local_name()const
{
	String s = Layer_Group::get_local_name();
	return s.empty() ? _("Patch Group") : '[' + s + ']';
}

Layer::Vocab
Layer_TimePatch::get_param_vocab()const
{
	Layer::Vocab ret(Layer_Group::get_param_vocab());

	ret.push_back(ParamDesc("time_patch")
		.set_local_name(_("Time Patch"))
		.set_description(_("Time patch applied to children"))
	);

	return ret;
}

bool
Layer_TimePatch::set_param(const String & param, const ValueBase &value)
{
	IMPORT_VALUE_PLUS(param_time_patch,
		update_children_patch();
	);
	return Layer_Group::set_param(param,value);
}

void
Layer_TimePatch::update_children_patch()
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
		printf("Patching %s..\n", child->get_name().c_str());
		for (auto &param : child->dynamic_param_list()) {
			printf("found param %s\n", param.first.c_str());
		}
	}
}

void
Layer_TimePatch::on_childs_changed()
{
	Layer_Group::on_childs_changed();
	update_children_patch();
}

ValueBase
Layer_TimePatch::get_param(const String& param)const
{
	EXPORT_VALUE(param_time_patch);

	EXPORT_NAME();
	EXPORT_VERSION();

	return Layer_Group::get_param(param);
}

void
Layer_TimePatch::on_canvas_set()
{
	Layer_Group::on_canvas_set();
	update_children_patch();
}
