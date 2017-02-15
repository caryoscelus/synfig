/* === S Y N F I G ========================================================= */
/*!	\file docks/xsheet_view.h
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

#ifndef __SYNFIG_STUDIO_DOCK_XSHEET_VIEW_H
#define __SYNFIG_STUDIO_DOCK_XSHEET_VIEW_H

#include <memory>

#include "docks/dock_canvasspecific.h"

namespace studio {
namespace docks {

class XsheetView : public Dock_CanvasSpecific
{
public:
	XsheetView();
	virtual ~XsheetView();

protected:
	virtual void init_canvas_view_vfunc(etl::loose_handle<CanvasView> canvas_view) override;
	virtual void changed_canvas_view_vfunc(etl::loose_handle<CanvasView> canvas_view) override;

private:
	class Impl;
	std::unique_ptr<Impl> impl;
}; // END of XsheetView


} // END of namespace docks
} // END of namespace studio

#endif
