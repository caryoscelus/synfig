/* === S Y N F I G ========================================================= */
/*!	\file animated.h
**	\brief New animated node
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

#ifndef __SYNFIG_VALUENODES_ANIMATED_H
#define __SYNFIG_VALUENODES_ANIMATED_H

/* === H E A D E R S ======================================================= */

#include "valuenode_minimalanimatedinterface.h"

#include <synfig/canvas.h>

#include <memory>

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {
namespace valuenodes {

class Animated : public ValueNode, public AnimatedInterface {
protected:
	explicit Animated(ValueNode& node);
	explicit Animated(Type& type);

public:
	virtual ValueNode::Handle clone(Canvas::LooseHandle canvas, const synfig::GUID& deriv_guid) const override;

public:
	virtual Range access_all() override;
	virtual Range access_timeline(const String& timeline) override;
	virtual ConstRange get_all() const override;
	virtual ConstRange get_timeline(const String& timeline) const override;
	virtual MaybeIter add_waypoint(Waypoint waypoint) override;

public:
	virtual ValueBase operator()(Time time) const override;
	virtual void get_times_vfunc(Node::time_set &set) const override;

	virtual String get_name() const override;
	virtual String get_local_name() const override;
	virtual String get_string() const override;

public:
	virtual ~Animated();

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

}; // END of namespace valuenodes
}; // END of namespace synfig
/* === E N D =============================================================== */

#endif
