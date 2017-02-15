/* === S Y N F I G ========================================================= */
/*!	\file docks/xsheet_view.cpp
**	\brief X-Sheet dock - frame viewer
**
**	Currently works for switch layers only, but can be extended later
**
**	\legal
**	Copyright (c) 2017 caryoscelus
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

#include "xsheet_view.h"

#include "../canvasview.h"

#include <synfig/general.h>
#include <synfig/localization.h>
#include <synfig/main.h>

#include <synfig/layers/layer_switch.h>

#include <gtkmm/builder.h>

namespace studio {
namespace docks {

class XsheetView::Impl {
private:
	Gtk::Box* content;

	// TODO: use smart pointers
	CanvasView* canvas_view;

public:
	void set_canvas_view(CanvasView* canvas_view_) {
		canvas_view = canvas_view_;
	}

public:
	Impl(Gtk::Container* parent)
	{
		// TODO: fix path
		auto builder = Gtk::Builder::create_from_file(synfig::Main::get_instance().share_path+"/synfig/ui/xsheet.glade");

		builder->get_widget("content", content);

		Gtk::Widget* base = nullptr;
		builder->get_widget("xsheet_view", base);
		parent->add(*base);

		Gtk::Button* update_button = nullptr;
		builder->get_widget("update", update_button);
		update_button->signal_clicked().connect(
			sigc::mem_fun(*this, &XsheetView::Impl::update_xsheet)
		);

		parent->show_all_children();
	}
	virtual ~Impl() {}

public:
	void update_xsheet()
	{
		synfig::info("XsheetView::update_xsheet()");
		if (!canvas_view)
		{
			synfig::warning("No canvas view attached!");
			return;
		}
		auto manager = canvas_view->get_selection_manager();
		auto layer = manager->get_selected_layer();
		if (!layer)
		{
			synfig::warning("No layer selected!");
			return;
		}
		// TODO: use smart pointers
		auto switch_layer = dynamic_cast<synfig::Layer_Switch*>(layer.get());
		if (!switch_layer)
		{
			synfig::warning("Selected layer is not switch layer!");
			return;
		}

		auto old_children = content->get_children();
		for (auto old_child : old_children) {
			content->remove(*old_child);
		}

		auto sub_canvas = switch_layer->get_sub_canvas();
		for (auto child_layer : *sub_canvas)
		{
			synfig::info("Found layer");
			auto fname_value = child_layer->get_param("filename");
			if (fname_value.get_type() != synfig::type_string) {
				synfig::warning("Not an import layer!");
				continue;
			}
			auto fname = fname_value.get(std::string());
			synfig::info(fname);
			auto pixbuf = Gdk::Pixbuf::create_from_file(fname, 100, 100);
			auto image = Gtk::manage(new Gtk::Image(pixbuf));
			content->pack_start(*image, false, false);
		}
		content->show_all_children();
	}
};

XsheetView::XsheetView():
	Dock_CanvasSpecific("xsheet", _("X-Sheet"), Gtk::StockID("synfig-group")),
	impl(std::make_unique<XsheetView::Impl>(this))
{
}

XsheetView::~XsheetView()
{
}

void
XsheetView::init_canvas_view_vfunc(etl::loose_handle<CanvasView> canvas_view)
{
	synfig::info("XsheetView::init_canvas_view_vfunc()");
}

void
XsheetView::changed_canvas_view_vfunc(etl::loose_handle<CanvasView> canvas_view)
{
	synfig::info("XsheetView::changed_canvas_view_vfunc()");
	impl->set_canvas_view(canvas_view.get());
}

} // END of namespace docks
} // END of namespace studio
