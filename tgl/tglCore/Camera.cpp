/*
 * Camera.cpp
 */

#include <GL/gl.h>
#include <GL/glu.h>

#include "tglUtil/EigenUtil.h"
#include "Camera.h"
#include "InputEvent.h"

#include <iostream>
using namespace std;

namespace tgl {

Camera::Camera() {
	p_.setZero();
	R_.setIdentity();
	Rt_.setIdentity();
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}


void Camera::initializeConfiguration()
{
	p_.setZero();
	R_.setIdentity();
	Rt_.setIdentity();
}

void Camera::update()
{
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	glRotatef (90, 0,0,1);
	glRotatef (90, 0,1,0);

	Eigen::Vector3d rpy = tgl::rpyFromRot(R_);

	double r2d = 180/M_PI;
	glRotatef (-rpy[0]*r2d, 1,0,0);
	glRotatef (-rpy[1]*r2d, 0,1,0);
	glRotatef (-rpy[2]*r2d, 0,0,1);

	glTranslatef (-p_[0], -p_[1], -p_[2]);

}

void Camera::mousePressEvent(MouseEvent* /* e */)
{

}

void Camera::mouseMoveEvent(MouseEvent* /* e */)
{

}

void Camera::mouseReleaseEvent(MouseEvent* /* e */)
{

}

void Camera::wheelEvent(WheelEvent* /* e */)
{

}

void Camera::updateProject()
{
	//モデルビュー行列取得
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_.data());

	//透視投影行列取得
	glGetDoublev(GL_PROJECTION_MATRIX, projection_.data());

	//ビューポート取得
	glGetIntegerv(GL_VIEWPORT, viewport_.data());
}

// object -> window
Eigen::Vector3d Camera::project(double x, double y, double z)
{
	double winX, winY, winZ;//ウィンドウ座標格納用

	gluProject(x, y, z, modelview_.data(), projection_.data(), viewport_.data(), &winX, &winY, &winZ); //座標変換の計算

	return Eigen::Vector3d(winX, winY, 0.0);
}

// window -> object
Eigen::Vector3d Camera::unProject(int x, int y)
{
	float z;
	double objX;
	double objY;
	double objZ;

	glReadPixels(x, y, 1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
	gluUnProject(x, y, z,modelview_.data(), projection_.data(), viewport_.data(), &objX, &objY, &objZ);

	return Eigen::Vector3d(objX, objY, objZ);
}

// object -> window
Eigen::Vector3d Camera::project2D(double x, double y, double z)
{
	double winX, winY, winZ;//ウィンドウ座標格納用

	gluProject(x, y, z, modelview_.data(), projection_.data(), viewport_.data(), &winX, &winY, &winZ); //座標変換の計算

	int h = viewport_[3];
	return Eigen::Vector3d(winX, h - winY, 0.0);
}

// window -> object
Eigen::Vector3d Camera::unProject2D(int x, int y)
{
	y = - y + height();

	float z;
	double objX;
	double objY;
	double objZ;

	glReadPixels(x, y, 1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);

	gluUnProject(x, y, z,modelview_.data(), projection_.data(), viewport_.data(), &objX, &objY, &objZ);

	return Eigen::Vector3d(objX, objY, objZ);
}

} /* namespace tgl */
