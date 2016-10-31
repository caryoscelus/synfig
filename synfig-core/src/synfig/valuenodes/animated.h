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

#include <memory>

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {
namespace valuenodes {

class Animated : public ValueNode, public AnimatedInterface {
public:
	virtual Range access_all() override;
	virtual Range access_timeline(const String& timeline) override;
	virtual ConstRange get_all() const override;
	virtual ConstRange get_timeline(const String& timeline) const override;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

}; // END of namespace valuenodes
}; // END of namespace synfig
/* === E N D =============================================================== */

#endif
