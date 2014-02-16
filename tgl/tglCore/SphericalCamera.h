/*
 * SphericalCamera.h
 */

#ifndef TGL_CORE_SPHERICALCAMERA_H_
#define TGL_CORE_SPHERICALCAMERA_H_

#include "Camera.h"

namespace tgl {

class SphericalCamera;
typedef std::shared_ptr<SphericalCamera> SphericalCameraPtr;

class SphericalCamera : public Camera {
public:
	SphericalCamera();
	virtual ~SphericalCamera();

	void setCenter(const double pos[3]);
	const double* getCenter() const { return center_; }

	void setDistance(double distance) { distance_ = distance; }
	double getDistance() const { return distance_; }

	void setHeading(double azimuth) { heading_ = azimuth; }
	double getHeading() const { return heading_; }

	void setElevation(double elevation) { elevation_ = elevation; }
	double getElevation() const { return elevation_; }

	virtual void initializeConfiguration();
	virtual void update();

	virtual void mousePressEvent(MouseEvent* e);
	virtual void mouseMoveEvent(MouseEvent* e);
	virtual void mouseReleaseEvent(MouseEvent* e);
	virtual void wheelEvent(WheelEvent* e);

protected:
	void wrapCameraAngles();
	void viewMotion1(int dx, int dy);
	void viewMotion2(int dx, int dy);
	void viewMotion3(int dx, int dy);

	bool allowThrow_;
	bool thrown_;

	double center_[3];
	double distance_;
	double heading_;
	double elevation_;

	double view_hpr_[3];		// position x,y,z
	double view_xyz_[3];		// heading, pitch, roll (degrees)

};

} /* namespace tgl */

#endif /* TGL_CORE_SPHERICALCAMERA_H_ */
