/* === S Y N F I G ========================================================= */
/*!	\file make_layer.h
**	\brief Base class for layer-making
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

#ifndef __SYNFIG_STUDIO_MAKE_LAYER_H
#define __SYNFIG_STUDIO_MAKE_LAYER_H

/* === H E A D E R S ======================================================= */

#include "state_shape.h"
#include "canvasview.h"
#include <synfig/general.h>
#include <synfig/valuenodes/valuenode_bline.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */
namespace studio {

class MakeLayer {
protected:
	StateShape_Context* context;

#define CONTEXT_METHOD(name) inline auto name() { return context->name(); }
	CONTEXT_METHOD(disable_egress_on_selection_change)
	CONTEXT_METHOD(enable_egress_on_selection_change)
	CONTEXT_METHOD(get_canvas_interface)
	CONTEXT_METHOD(get_canvas)
	CONTEXT_METHOD(get_canvas_view)
	CONTEXT_METHOD(get_id)
	CONTEXT_METHOD(get_blend)
	CONTEXT_METHOD(get_opacity)
	CONTEXT_METHOD(get_bline_width)
	CONTEXT_METHOD(get_feather_size)
	CONTEXT_METHOD(get_invert)
	CONTEXT_METHOD(get_layer_link_origins_flag)
	CONTEXT_METHOD(layers_to_create)
#undef CONTEXT_METHOD

public:
	MakeLayer(StateShape_Context* context_) : context(context_) {}

	virtual void make_layer(
		synfig::Canvas::Handle canvas,
		int depth,
		synfigapp::Action::PassiveGrouper& group,
		synfigapp::SelectionManager::LayerList& layer_selection,
		synfig::ValueNode_BLine::Handle value_node_bline,
		synfig::Vector& origin,
		synfig::ValueNode::Handle value_node_origin
	) = 0;
};

}; // END of namespace studio

/* === E N D =============================================================== */
#endif