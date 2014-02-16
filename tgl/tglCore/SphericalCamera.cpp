/*
 * SphericalCamera.cpp
 */

#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>

#include "tglUtil/EigenUtil.h"
#include "InputEvent.h"
#include "SphericalCamera.h"

#define RAD_TO_DEG (180.0/M_PI)
#define DEG_TO_RAD (M_PI/180.0)

namespace tgl {

SphericalCamera::SphericalCamera() {

}

SphericalCamera::~SphericalCamera() {

}

void SphericalCamera::setCenter(const double pos[3])
{
	center_[0] = pos[0];
	center_[1] = pos[1];
	center_[2] = pos[2];
	view_xyz_[1] = -pos[0];
	view_xyz_[0] = -pos[1];
	view_xyz_[2] = pos[2];

	p_(0) = center_[0];
	p_(1) = center_[1];
	p_(2) = center_[2];
}

void SphericalCamera::initializeConfiguration()
{
	center_[0] = 0.0; center_[1] = 0.0; center_[2] = 0.0;
	distance_ = 5.0;
	heading_ = 0.0;
	elevation_ = -45;

	view_xyz_[0] = 0;
	view_xyz_[1] = 0;
	view_xyz_[2] = 0;

	view_hpr_[0] = 0;
	view_hpr_[1] = 0;
	view_hpr_[2] = 0;
}

static void rotateR(const double R[9])
{
	GLdouble matrix[16];
	matrix[0] = R[0];
	matrix[1] = R[3];
	matrix[2] = R[6];
	matrix[3] = 0.0;
	matrix[4] = R[1];
	matrix[5] = R[4];
	matrix[6] = R[7];
	matrix[7] = 0.0;
	matrix[8] = R[2];
	matrix[9] = R[5];
	matrix[10] = R[8];
	matrix[11] = 0.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = 0.0;
	matrix[15] = 1.0;
	glMultMatrixd(matrix);
}


void SphericalCamera::update()
{
	R_ = rotationX(elevation_*DEG_TO_RAD) * rotationZ(heading_*DEG_TO_RAD);
	Rt_ = R_.transpose();

	glTranslated(0.0, 0.0, -distance_);
	rotateR(R_.data());
	glTranslated(-center_[0], -center_[1], -center_[2]);
}

void SphericalCamera::mousePressEvent(MouseEvent* /* event */)
{

}

void SphericalCamera::mouseMoveEvent(MouseEvent* event)
{
    int dx = event->dx();
    int dy = event->dy();

    switch (event->button()) {
    case MouseEvent::MouseBotton::LeftButton:

    	break;
    case MouseEvent::MouseBotton::RightButton:
    	viewMotion1(dx, dy);
    	break;
    case MouseEvent::MouseBotton::MiddleButton:
    	viewMotion3(dx, dy);
    	break;
    default:
    	break;
    }
}

void SphericalCamera::mouseReleaseEvent(MouseEvent* event)
{

}

void SphericalCamera::wheelEvent(WheelEvent* event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees * 2;

    distance_ += 0.02 * numTicks;
}

// left
void SphericalCamera::viewMotion1(int dx, int dy)
{
	view_hpr_[0] += double (dx) * 0.5f;
	view_hpr_[1] += double (dy) * 0.5f;

	heading_ += double (dx) * 0.5f;
	elevation_ += double (dy) * 0.5f;

	wrapCameraAngles();
}

// rigth
void SphericalCamera::viewMotion2(int dx, int dy)
{
	double side = 0.01 * double(dx);
	double fwd = 0.01f * double(dy);
	double s = (double)sin(view_hpr_[0]*DEG_TO_RAD);
	double c = (double)cos(view_hpr_[0]*DEG_TO_RAD);

	view_xyz_[0] += -s*side + c*fwd;
	view_xyz_[1] += c*side + s*fwd;

	wrapCameraAngles();
}

// mid
void SphericalCamera::viewMotion3(int dx, int dy)
{
	double side = 0.01 * double(dx);
	double fwd = 0.0f;
	double s = (double)sin(view_hpr_[0]*DEG_TO_RAD);
	double c = (double)cos(view_hpr_[0]*DEG_TO_RAD);

//	cout << "view hrp " << view_hpr_[0] << endl;

	view_xyz_[0] += -s*side + c*fwd;
	view_xyz_[1] += c*side + s*fwd;
	view_xyz_[2] += 0.01f * double(dy);

	center_[0] = -view_xyz_[1];
	center_[1] = -view_xyz_[0];
	center_[2] = view_xyz_[2];
	p_(0) = center_[0];
	p_(1) = center_[1];
	p_(2) = center_[2];

	wrapCameraAngles();
}

void SphericalCamera::wrapCameraAngles()
{
    for (int i = 0; i < 3; ++i) {
    	while (view_hpr_[i] > 180) view_hpr_[i] -= 360;
    	while (view_hpr_[i] < -180) view_hpr_[i] += 360;
    }
}

} /* namespace tgl */
