/*
 * SE3.h
 */

#ifndef TGL_UTIL_SE3_H_
#define TGL_UTIL_SE3_H_

#include <memory>
#include <Eigen/Core>

namespace tgl {

class SE3;
typedef std::shared_ptr<SE3> SE3Ptr;

class SE3 {
public:
	SE3() {
		p_.setZero();
		R_.setIdentity();
	}

	SE3(const Eigen::Vector3d& p, const Eigen::Matrix3d& R) {
		p_ = p;
		R_ = R;
	}

	virtual ~SE3() { }

	virtual void setPosition(const Eigen::Vector3d& p) { p_ = p; }
	virtual void setAttitude(const Eigen::Matrix3d& R) { R_ = R; }

	virtual const Eigen::Vector3d& position() const { return p_; }
	virtual const Eigen::Matrix3d& attitude() const { return R_; }

protected:
	Eigen::Vector3d p_;
	Eigen::Matrix3d R_;
};

} /* namespace tgl */

#endif /* TGL_UTIL_SE3_H_ */
