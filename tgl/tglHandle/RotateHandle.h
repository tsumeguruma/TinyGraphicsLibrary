/*
 * RotateHandle.h
 */

#ifndef TGL_HANDLE_ROTATEHANDLE_H_
#define TGL_HANDLE_ROTATEHANDLE_H_

#include <memory>
#include <array>
#include <boost/signals2.hpp>
#include "tglCore/GraphicsItem.h"
#include "tglUtil/SE3.h"

namespace tgl {
namespace handle {

class RotateHandle : public GraphicsItem {
public:
	RotateHandle();
	virtual ~RotateHandle();
};

} /* namespace handle */
} /* namespace tgl */

#endif /* TGL_HANDLE_ROTATEHANDLE_H_ */
