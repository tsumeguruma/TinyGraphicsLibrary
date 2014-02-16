/*
 * Intersection.cpp
 */

#include <Eigen/Geometry>
#include "Intersection.h"

namespace tgl
{

Eigen::Vector3d calcIntersectionPointAndLine(const Eigen::Vector3d& p, const Eigen::Vector3d& line_p1, const Eigen::Vector3d& line_p2)
{
	Eigen::Vector3d d(line_p2 - line_p1);
	d.normalize();
	Eigen::Vector3d v(p - line_p1);
	double t = d.dot(v);

	return Eigen::Vector3d(line_p1 + t*d);
}

bool calcIntersectionLineAndLine(Eigen::Vector3d& line1_np, Eigen::Vector3d& line2_np,
		const Eigen::Vector3d& line1_p1, const Eigen::Vector3d& line1_p2,
		const Eigen::Vector3d& line2_p1, const Eigen::Vector3d& line2_p2
		)
{

	const Eigen::Vector3d& p1 = line1_p1;
	Eigen::Vector3d d1(line1_p2 - line1_p1);

	const Eigen::Vector3d& p2 = line2_p1;
	Eigen::Vector3d d2(line2_p2 - line2_p1);

	Eigen::Vector3d c(d1.cross(d2));
	double a = c.squaredNorm();
	if (a < 0.000001) {	// 平行

		return false;
	} else {
		Eigen::Vector3d v1(Eigen::Vector3d(p2 - p1).cross(d2));
		Eigen::Vector3d v2(Eigen::Vector3d(p2 - p1).cross(d1));
		double t1 = v1.dot(c) / a;
		double t2 = v2.dot(c) / a;

		line1_np = p1 + t1 * d1;
		line2_np = p2 + t2 * d2;

		return true;
	}
}

Eigen::Vector3d calcIntersectionLineAndPlane(const Eigen::Vector3d& line_p1, const Eigen::Vector3d& line_p2,
		const Eigen::Vector3d& plane_p1, const Eigen::Vector3d& plane_p2, const Eigen::Vector3d& plane_p3
		)
{
	const Eigen::Vector3d& p1 = line_p1;
	Eigen::Vector3d d1(line_p2 - line_p1);

	Eigen::Vector3d n = Eigen::Vector3d(plane_p1 - plane_p2).cross(Eigen::Vector3d(plane_p1 - plane_p3));
	n.normalize();
	double d = n.dot(plane_p1);

	double t;
	double dn = d1.dot(n);
	if (abs(dn) < 0.0000001) {
		return Eigen::Vector3d(0,0,0);
	} else {
		t = (d - p1.dot(n)) / dn;
	}

	return Eigen::Vector3d(p1 + t * d1);
}

bool calcIntersectionLineAndSphere(Eigen::Vector3d& intersection_p,
		const Eigen::Vector3d& line_p1, const Eigen::Vector3d& line_p2,
		const Eigen::Vector3d& sphere_p, double radius
		)
{
	Eigen::Vector3d d = line_p2 - line_p1;
	d.normalize();

	Eigen::Vector3d ev(sphere_p - line_p1);
	double a = ev.dot(d);
	double e2 = ev.dot(ev);

	double sq = sqrt(radius*radius - e2 + a*a);
	if (sq < 0) {
		return false;
	} else {
		double t = a - sq;
		intersection_p = line_p1 + t * d;
		return true;
	}
}

}
