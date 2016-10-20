/* === S Y N F I G ========================================================= */
/*!	\file state_rectangle.cpp
**	\brief Rectangle tool state
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2008 Chris Moore
**	Copyright (c) 2010 Carlos López
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

#include <synfig/valuenodes/valuenode_dynamiclist.h>

#include "state_rectangle.h"

#include "docks/dock_toolbox.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;
using namespace studio;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

StateRectangle studio::state_rectangle;

/* === C L A S S E S & S T R U C T S ======================================= */

/* === M E T H O D S ======================================================= */

StateRectangle::StateRectangle():
	StateShape<StateRectangle_Context>("rectangle")
{
}

StateRectangle::~StateRectangle()
{
}

void
StateRectangle_Context::do_load_settings()
{
	StateShape_Context::do_load_settings();
	String value;

	if(settings.get_value("rectangle.expand",value))
		set_expand_size(Distance(atof(value.c_str()), App::distance_system));
	else
		set_expand_size(Distance(0, App::distance_system)); // default expansion
}

void
StateRectangle_Context::do_save_settings()
{
	StateShape_Context::do_save_settings();
	settings.set_value("rectangle.expand",expand_dist.get_value().get_string());
}

StateRectangle_Context::StateRectangle_Context(CanvasView* canvas_view):
	StateShape_Context(canvas_view)
{
	// expansion
	expand_label.set_label(_("Expansion:"));
	expand_label.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
	expand_label.set_sensitive(false);

	expand_dist.set_digits(2);
	expand_dist.set_range(0, 1000000);
	expand_dist.set_sensitive(false);
}

void
StateRectangle_Context::enter()
{
	StateShape_Context::enter();
	// pack all options to the options_table

	// 0, title
	options_table.attach(title_label,
		0, 2, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 1, name
	options_table.attach(id_box,
		0, 2, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 2, layer types creation
	options_table.attach(layer_types_label,
		0, 2, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0
		);
	options_table.attach(layer_types_box,
		0, 2, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 3, blend method
	options_table.attach(blend_box,
		0, 1, 4, 5, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	options_table.attach(blend_enum,
		1, 2, 4, 5, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 4, opacity
	options_table.attach(opacity_label,
		0, 1, 5, 6, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	options_table.attach(opacity_hscl,
		1, 2, 5, 6, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 5, brush size
	options_table.attach(bline_width_label,
		0, 1, 6, 7, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	options_table.attach(bline_width_dist,
		1, 2, 6, 7, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 6, invert
	options_table.attach(invert_box,
		0, 2, 7, 8, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 7, feather
	options_table.attach(feather_label,
		0, 1, 8, 9, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	options_table.attach(feather_dist,
		1, 2, 8, 9, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 8, expansion
	options_table.attach(expand_label,
		0, 1, 9, 10, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	options_table.attach(expand_dist,
		1, 2, 9, 10, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 0, 0
		);
	// 9, link origins
	options_table.attach(link_origins_box,
		0, 2, 10, 11, Gtk::FILL, Gtk::FILL, 0, 0
		);

	// fine-tune options layout
	options_table.set_border_width(GAP*2); // border width
	options_table.set_row_spacings(GAP); // row gap
	options_table.set_row_spacing(0, GAP*2); // the gap between first and second row.
	options_table.set_row_spacing(2, 1); // row gap between label and icon of layer type
	options_table.set_row_spacing(11, 0); // the final row using border width of table

	finalize_init();
}

StateRectangle_Context::~StateRectangle_Context()
{
}

void
StateRectangle_Context::make_rectangle_layer(
	synfig::Canvas::Handle canvas,
	int depth,
	synfigapp::Action::PassiveGrouper& group,
	synfigapp::SelectionManager::LayerList& layer_selection,
	const synfig::Point& p1,
	const synfig::Point& p2,
	synfig::ValueNode::Handle value_node_origin
)
{
	disable_egress_on_selection_change();
	Layer::Handle layer=get_canvas_interface()->add_layer_to("rectangle",canvas,depth);
	enable_egress_on_selection_change();
	if (!layer)
	{
		get_canvas_view()->get_ui_interface()->error(_("Unable to create layer"));
		group.cancel();
		return;
	}
	layer_selection.push_back(layer);

	layer->set_param("point1",p1);
	get_canvas_interface()->signal_layer_param_changed()(layer,"point1");

	layer->set_param("point2",p2);
	get_canvas_interface()->signal_layer_param_changed()(layer,"point2");

	layer->set_param("expand",get_expand_size());
	get_canvas_interface()->signal_layer_param_changed()(layer,"expand");

	layer->set_param("invert",get_invert());
	get_canvas_interface()->signal_layer_param_changed()(layer,"invert");

	layer->set_param("blend_method", get_blend());
	get_canvas_interface()->signal_layer_param_changed()(layer, "blend_method");

	layer->set_param("amount", get_opacity());
	get_canvas_interface()->signal_layer_param_changed()(layer, "amount");

	layer->set_description(get_id());
	get_canvas_interface()->signal_layer_new_description()(layer,layer->get_description());
}

void
StateRectangle_Context::make_rectangle(const Point& _p1, const Point& _p2)
{
	int depth;
	Canvas::Handle canvas;
	synfigapp::SelectionManager::LayerList layer_selection;

	// TODO: find some better way to initialize everything
	// Should we just use separate class for building layers?
	synfigapp::Action::PassiveGrouper group = init_layer_creation(depth, canvas, layer_selection);

	const synfig::TransformStack& transform(get_work_area()->get_curr_transform_stack());
	const Point p1(transform.unperform(_p1));
	const Point p2(transform.unperform(_p2));
	Vector origin;
	Real x_min, x_max, y_min, y_max;
	if (p1[0] < p2[0]) { x_min = p1[0]; x_max = p2[0]; } else { x_min = p2[0]; x_max = p1[0]; }
	if (p1[1] < p2[1]) { y_min = p1[1]; y_max = p2[1]; } else { y_min = p2[1]; y_max = p1[1]; }
	x_min -= get_expand_size(); x_max += get_expand_size(); y_min -= get_expand_size(); y_max += get_expand_size();

	std::vector<BLinePoint> new_list;
	for (int i = 0; i < 4; i++)
	{
		new_list.push_back(*(new BLinePoint));
		new_list[i].set_width(1);
		new_list[i].set_vertex(Point((i==0||i==3)?x_min:x_max,
									 (i==0||i==1)?y_min:y_max));
		new_list[i].set_tangent(Point(0,0));
	}

	ValueNode_BLine::Handle value_node_bline(ValueNode_BLine::create(new_list));
	assert(value_node_bline);

	ValueNode::Handle value_node_origin(ValueNode_Const::create(origin));
	assert(value_node_origin);

	// Set the looping flag
	value_node_bline->set_loop(true);

	value_node_bline->set_member_canvas(canvas);

	///////////////////////////////////////////////////////////////////////////
	//   R E C T A N G L E
	///////////////////////////////////////////////////////////////////////////

	if (get_layer_shape_flag())
	{
		make_rectangle_layer(canvas, depth, group, layer_selection, p1, p2, value_node_origin);
	}

	generate_shape_layers(canvas, depth, group, layer_selection, value_node_bline, origin, value_node_origin);
	finalize_layer_creation(layer_selection);
}

Smach::event_result
StateRectangle_Context::event_mouse_click_handler(const Smach::event& x)
{
	const EventMouse& event(*reinterpret_cast<const EventMouse*>(&x));

	if(event.key==EVENT_WORKAREA_MOUSE_BUTTON_DOWN && event.button==BUTTON_LEFT)
	{
		point_holder=get_work_area()->snap_point_to_grid(event.pos);
		etl::handle<Duck> duck=new Duck();
		duck->set_point(point_holder);
		duck->set_name("p1");
		duck->set_type(Duck::TYPE_POSITION);
		get_work_area()->add_duck(duck);

		point2_duck=new Duck();
		point2_duck->set_point(point_holder);
		point2_duck->set_name("p2");
		point2_duck->set_type(Duck::TYPE_POSITION);
		point2_duck->set_box_duck(duck);
		get_work_area()->add_duck(point2_duck);

		return Smach::RESULT_ACCEPT;
	}

	if(event.key==EVENT_WORKAREA_MOUSE_BUTTON_DRAG && event.button==BUTTON_LEFT)
	{
		if (!point2_duck) return Smach::RESULT_OK;
		point2_duck->set_point(get_work_area()->snap_point_to_grid(event.pos));
		get_work_area()->queue_draw();
		return Smach::RESULT_ACCEPT;
	}

	if(event.key==EVENT_WORKAREA_MOUSE_BUTTON_UP && event.button==BUTTON_LEFT)
	{
		make_rectangle(point_holder, get_work_area()->snap_point_to_grid(event.pos));
		get_work_area()->clear_ducks();
		return Smach::RESULT_ACCEPT;
	}

	return Smach::RESULT_OK;
}

void
StateRectangle_Context::toggle_layer_creation()
{
  // don't allow none layer creation
  if (get_layer_shape_flag() +
     get_layer_region_flag() +
     get_layer_outline_flag() +
     get_layer_advanced_outline_flag() +
     get_layer_curve_gradient_flag() +
     get_layer_plant_flag() == 0)
  {
    if(layer_shape_flag) set_layer_shape_flag(true);
    else if(layer_region_flag) set_layer_region_flag(true);
    else if(layer_outline_flag) set_layer_outline_flag(true);
    else if(layer_advanced_outline_flag) set_layer_advanced_outline_flag(true);
    else if(layer_curve_gradient_flag) set_layer_curve_gradient_flag(true);
    else if(layer_plant_flag) set_layer_plant_flag(true);
  }

	// brush size
	if (get_layer_outline_flag() ||
		get_layer_advanced_outline_flag() ||
		get_layer_curve_gradient_flag())
	{
		bline_width_label.set_sensitive(true);
		bline_width_dist.set_sensitive(true);
	}
	else
	{
		bline_width_label.set_sensitive(false);
		bline_width_dist.set_sensitive(false);
	}

	// invert
	if (get_layer_shape_flag() ||
		get_layer_region_flag() ||
		get_layer_outline_flag() ||
		get_layer_advanced_outline_flag())
	{
		invert_box.set_sensitive(true);
	}
	else
		invert_box.set_sensitive(false);

	// feather size
	if (get_layer_shape_flag() ||
		get_layer_shape_flag() ||
		get_layer_region_flag() ||
		get_layer_outline_flag() ||
		get_layer_advanced_outline_flag())
	{
		feather_label.set_sensitive(true);
		feather_dist.set_sensitive(true);
	}
	else
	{
		feather_label.set_sensitive(false);
		feather_dist.set_sensitive(false);
	}

	// expansion
	if (get_layer_shape_flag())
	{

		expand_label.set_sensitive(true);
		expand_dist.set_sensitive(true);
	}
	else
	{
		expand_label.set_sensitive(false);
		expand_dist.set_sensitive(false);
	}

	// link origins
	if (get_layer_region_flag() +
		get_layer_outline_flag() +
		get_layer_advanced_outline_flag() +
		get_layer_plant_flag() +
		get_layer_curve_gradient_flag() >= 2)
		{
			link_origins_box.set_sensitive(true);
		}
	else link_origins_box.set_sensitive(false);

  // update layer flags
  layer_shape_flag = get_layer_shape_flag();
  layer_region_flag = get_layer_region_flag();
  layer_outline_flag = get_layer_outline_flag();
  layer_advanced_outline_flag = get_layer_advanced_outline_flag();
  layer_curve_gradient_flag = get_layer_curve_gradient_flag();
  layer_plant_flag = get_layer_plant_flag();
}
