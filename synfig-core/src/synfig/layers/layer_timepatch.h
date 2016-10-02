/* === S Y N F I G ========================================================= */
/*!	\file layer_timepatch.h
**	\brief "Time Patch" layer (header)
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_LAYER_TIMEPATCH_H
#define __SYNFIG_LAYER_TIMEPATCH_H

/* === H E A D E R S ======================================================= */

#include "layer_group.h"

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {
/*!	\class Layer_TimePatch
**	\brief Class of the Patch Group layer
*/
class Layer_TimePatch : public Layer_Group
{
	//! Layer module: defines the needed members to belong to a layer's factory.
	SYNFIG_LAYER_MODULE_EXT
private:
	//! Parameter: (Patch) Patch
	ValueBase param_time_patch;
	bool unique_canvas = false;

public:
	//! Default constructor
	Layer_TimePatch();
	//! Destructor
	virtual ~Layer_TimePatch();
	//! Returns a string with the localized name of this layer
	virtual String get_local_name()const;

	//!	Sets the parameter described by \a param to \a value. \see Layer::set_param
	virtual bool set_param(const String & param, const synfig::ValueBase &value);
	void update_children_patch();
	//! Get the value of the specified parameter. \see Layer::get_param
	virtual ValueBase get_param(const String & param)const;
	//! Gets the parameter vocabulary
	virtual Vocab get_param_vocab()const;

	virtual void on_canvas_set();
}; // END of class Layer_TimePatch

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
