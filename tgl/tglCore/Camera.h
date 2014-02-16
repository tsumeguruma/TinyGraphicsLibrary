/*
 * Camera.h
 */

#ifndef TGL_CORE_CAMERA_H_
#define TGL_CORE_CAMERA_H_

#include <memory>
#include <array>
#include <Eigen/Core>

namespace tgl {

class MouseEvent;
class WheelEvent;

class Camera;
typedef std::shared_ptr<Camera> CameraPtr;

class Camera {
public:
	Camera();
	virtual ~Camera();

	int width() const { return viewport_[2]; }
	int height() const { return viewport_[3]; }

	void setPosition(const Eigen::Vector3d& p) { p_ = p; }
	void setRotation(const Eigen::Matrix3d& R) { R_ = R; }

	const Eigen::Vector3d& position() const { return p_; }
	const Eigen::Matrix3d& rotation() const { return R_; }
	const Eigen::Matrix3d& inverseRotation() const { return Rt_; }

	// object -> window
	Eigen::Vector3d project(double x, double y, double z);

	// window -> object
	Eigen::Vector3d unProject(int x, int y);

	// object -> window
	Eigen::Vector3d project2D(double x, double y, double z);

	// window -> object
	Eigen::Vector3d unProject2D(int x, int y);

	virtual void initializeConfiguration();

	virtual void update();

	virtual void mousePressEvent(MouseEvent* e);
	virtual void mouseMoveEvent(MouseEvent* e);
	virtual void mouseReleaseEvent(MouseEvent* e);
	virtual void wheelEvent(WheelEvent* e);

	void updateProject();

protected:
	Eigen::Vector3d p_;
	Eigen::Matrix3d R_;
	Eigen::Matrix3d Rt_;

	std::array<double, 16> modelview_;
	std::array<double, 16> projection_;
	std::array<int, 4> viewport_;
};

} /* namespace tgl */

#endif /* TGL_CORE_CAMERA_H_ */
