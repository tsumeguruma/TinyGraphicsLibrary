/*
 * StandardCamera.cpp
 */

#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>

#include "tglUtil/EigenUtil.h"
#include "InputEvent.h"
#include "StandardCamera.h"

// constants to convert degrees to radians and the reverse
#define RAD_TO_DEG (180.0/M_PI)
#define DEG_TO_RAD (M_PI/180.0)

namespace tgl {

StandardCamera::StandardCamera() {
    double xyz[3] = {2.7,-3.0, 1.6};
    double hpr[3] = {120.0,-12.0, 0.0};
    set(xyz[0], xyz[1], xyz[2], hpr[0], hpr[1], hpr[2]);

	allowThrow_ = false;
	thrown_ = allowThrow_;
}

StandardCamera::~StandardCamera() {

}


void StandardCamera::set(double x, double y, double z, double h, double p, double r)
{
	view_xyz_[0] = x; view_xyz_[1] = y; view_xyz_[2] = z;
	view_hpr_[0] = h; view_hpr_[1] = p; view_hpr_[2] = r;
}

void StandardCamera::update()
{
	if (thrown_) {
		switch (motion_) {
		case 1: viewMotion1(dx_, dy_); break;
		case 2: viewMotion2(dx_, dy_); break;
		case 3: viewMotion2(dx_, dy_); break;
		default: break;
		}
	}

	p_[0] = view_xyz_[0], p_[1] = view_xyz_[1], p_[2] = view_xyz_[2];
	setCamera();
}

void StandardCamera::setCamera(double x, double y, double z, double h, double p, double r)
{
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glRotatef (90, 0,0,1);
	glRotatef (90, 0,1,0);
	glRotatef (r, 1,0,0);
	glRotatef (p, 0,1,0);
	glRotatef (-h, 0,0,1);

	GLdouble m[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m);

	double* R = R_.data();
	R[0]=m[0]; R[1]=m[4]; R[2]=m[8];
	R[3]=m[1]; R[4]=m[5]; R[5]=m[9];
	R[6]=m[2]; R[7]=m[6]; R[8]=m[10];
	Rt_ = R_.transpose();

	glTranslatef (-x,-y,-z);
}

void StandardCamera::setCamera()
{
	setCamera(view_xyz_[0], view_xyz_[1], view_xyz_[2], view_hpr_[0], view_hpr_[1], view_hpr_[2]);
}

void StandardCamera::mousePressEvent(MouseEvent* event)
{
    px_ = event->x();
    py_ = event->y();
    thrown_ = false;
}

void StandardCamera::mouseMoveEvent(MouseEvent* event)
{
    int dx = event->x() - px_;
    int dy = event->y() - py_;
    dx_ = event->x() - px_;
    dy_ = event->y() - py_;
    px_ = event->x();
    py_ = event->y();
    switch (event->button()) {
    case MouseEvent::MouseBotton::LeftButton:

    	break;
    case MouseEvent::MouseBotton::RightButton:
    	viewMotion1(dx, dy);
    	motion_ = 1;
    	break;
    case MouseEvent::MouseBotton::MiddleButton:
    	viewMotion3(dx, dy);
    	motion_ = 3;
    	break;
    default:
    	break;
    }

	setCamera();

	thrown_ = false;
	thrown_ = allowThrow_;

}

void StandardCamera::mouseReleaseEvent(MouseEvent* /* event */)
{
	thrown_ = allowThrow_;
}

void StandardCamera::wheelEvent(WheelEvent* event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees * 2;
	viewMotion2(0, numTicks); // 垂直スクロール

	setCamera();
}

// left
void StandardCamera::viewMotion1(int dx, int dy)
{
	view_hpr_[0] += double (dx) * 0.5f;
	view_hpr_[1] += double (dy) * 0.5f;

	wrapCameraAngles();
}

// rigth
void StandardCamera::viewMotion2(int dx, int dy)
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
void StandardCamera::viewMotion3(int dx, int dy)
{
	double side = 0.01 * double(dx);
	double fwd = 0.0f;
	double s = (double)sin(view_hpr_[0]*DEG_TO_RAD);
	double c = (double)cos(view_hpr_[0]*DEG_TO_RAD);

	view_xyz_[0] += -s*side + c*fwd;
	view_xyz_[1] += c*side + s*fwd;
	view_xyz_[2] += 0.01f * double(dy);

	wrapCameraAngles();
}

void StandardCamera::wrapCameraAngles()
{
    for (int i = 0; i < 3; ++i) {
    	while (view_hpr_[i] > 180) view_hpr_[i] -= 360;
    	while (view_hpr_[i] < -180) view_hpr_[i] += 360;
    }
}

} /* namespace tgl */
