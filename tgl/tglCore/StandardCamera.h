/*
 * StandardCamera.h
 */

#ifndef TGL_CORE_STANDARDCAMERA_H_
#define TGL_CORE_STANDARDCAMERA_H_

#include "Camera.h"

namespace tgl {

class StandardCamera;
typedef std::shared_ptr<StandardCamera> StandardCameraPtr;

class StandardCamera : public Camera {
public:
	StandardCamera();
	virtual ~StandardCamera();

	void set(double x, double y, double z, double h, double p, double r);

	virtual void update();

	virtual void mousePressEvent(MouseEvent* e);
	virtual void mouseMoveEvent(MouseEvent* e);
	virtual void mouseReleaseEvent(MouseEvent* e);
	virtual void wheelEvent(WheelEvent* e);

	void setAllowThrow(bool allow) { allowThrow_ = allow; }
	bool allowThrow() const { return allowThrow_; }

protected:
	void setCamera();
	void setCamera(double x, double y, double z, double h, double p, double r);

	void wrapCameraAngles();
	void viewMotion1(int dx, int dy);
	void viewMotion2(int dx, int dy);
	void viewMotion3(int dx, int dy);

	int px_;
	int py_;

	int dx_;
	int dy_;

	bool allowThrow_;
	bool thrown_;
	int motion_;

	float view_hpr_[3];		// position x,y,z
	float view_xyz_[3];		// heading, pitch, roll (degrees)

};

} /* namespace tgl */

#endif /* TGL_CORE_STANDARDCAMERA_H_ */
