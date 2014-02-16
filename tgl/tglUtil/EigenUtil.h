/*
 * EigenUtil.h
 */

#ifndef TGL_UTIL_EIGENUTIL_H_
#define TGL_UTIL_EIGENUTIL_H_

#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace tgl {

inline double degree(double rad) { return (180.0 * rad / M_PI); }
inline double radian(double deg) { return (M_PI * deg / 180.0); }

template<typename Derived>
inline Eigen::Matrix<typename Eigen::MatrixBase<Derived>::Scalar, 3, 1>
rpyFromRot(const Eigen::MatrixBase<Derived>& R) {
    Eigen::Vector3d ea = R.eulerAngles(2, 1, 0);
    return Eigen::Vector3d(ea[2], ea[1], ea[0]); // exchange element order to be our conventional one !
}

Eigen::Matrix3d rotFromRpy(double r, double p, double y);

inline Eigen::Matrix3d rotFromRpy(const Eigen::Vector3d& rpy) {
    return rotFromRpy((double)rpy[0], (double)rpy[1], (double)rpy[2]);
}

inline Eigen::Matrix3d rotationX(double rad)
{
	return rotFromRpy(rad, 0.0, 0.0);
}

inline Eigen::Matrix3d rotationY(double rad)
{
	return rotFromRpy(0.0, rad, 0.0);
}

inline Eigen::Matrix3d rotationZ(double rad)
{
	return rotFromRpy(0.0, 0.0, rad);
}

inline Eigen::Matrix3d rodrigues(const Eigen::Vector3d& axis, double q)
{
	Eigen::AngleAxisd axisd(q, axis);
	return axisd.toRotationMatrix();
}

inline Eigen::Matrix3d rodorigues(const Eigen::Vector3d& w)
{
	double a = w.norm();
	if (a < 1.0e-6) { // identity
		return Eigen::Matrix3d::Identity();
	} else {
		return rodrigues(w.normalized(), a);
	}
}

} /* namespace tgl */
#endif /* EIGENUTIL_H_ */
