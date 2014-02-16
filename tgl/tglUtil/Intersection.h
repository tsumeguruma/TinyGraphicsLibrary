/*
 * Intersection.h
 */

#ifndef TGL_UTIL_INTERSECTION_H_
#define TGL_UTIL_INTERSECTION_H_

#include <Eigen/Core>

namespace tgl
{

Eigen::Vector3d calcIntersectionPointAndLine(const Eigen::Vector3d& p, const Eigen::Vector3d& line_p1, const Eigen::Vector3d& line_p2);

// np : near point
bool calcIntersectionLineAndLine(Eigen::Vector3d& line1_np, Eigen::Vector3d& line2_np,
		const Eigen::Vector3d& line1_p1, const Eigen::Vector3d& line1_p2,
		const Eigen::Vector3d& line2_p1, const Eigen::Vector3d& line2_p2
		);

Eigen::Vector3d calcIntersectionLineAndPlane(const Eigen::Vector3d& line_p1, const Eigen::Vector3d& line_p2,
		const Eigen::Vector3d& plane_p1, const Eigen::Vector3d& plane_p2, const Eigen::Vector3d& plane_p3
		);

bool calcIntersectionLineAndSphere(Eigen::Vector3d& intersection_p,
		const Eigen::Vector3d& line_p1, const Eigen::Vector3d& line_p2,
		const Eigen::Vector3d& sphere_p, double radius
		);

}

#endif /* INTERSECTION_H_ */
