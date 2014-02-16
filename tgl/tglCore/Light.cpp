/*
 * Light.cpp
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include "Light.h"

namespace tgl {

Light::Light(int index) {
	index_ = index >= 0 ? index : 0;

	isEnable_ = true;

	position_[0] = 100.0;
	position_[1] = 100.0;
	position_[2] = 150.0;
	position_[3] = 0.0;

	p_ << position_[0], position_[1], position_[2];
}

Light::~Light() {

}

void Light::setAmbient(float ambient[4])
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}

void Light::setDiffuse(float diffuse[4])
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

void Light::setSpecular(float specular[4])
{
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void Light::setPosition(const double position[3])
{
	for (int i = 0; i < 4; ++i) {
		position_[i] = position[i];
	}

	for (int i = 0; i < 3; ++i) {
		p_[i] = position[i];
	}
}

void Light::setPosition(const Eigen::Vector3d& position)
{
	setPosition(position.data());
}

void Light::initializeConfiguration()
{
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0 + index_);
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv (GL_LIGHT0 + index_, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0 + index_, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0 + index_, GL_SPECULAR, light_specular);
}

void Light::setEnable(bool enable)
{
	isEnable_ = enable;
	if (enable) {
		 glEnable(GL_LIGHT0 + index_);
	} else {
		 glDisable(GL_LIGHT0 + index_);
	}
}

void Light::update()
{
	if (enable()) {
		glLightfv(GL_LIGHT0 + index_, GL_POSITION, position_);
	}
}

} /* namespace tgl */
