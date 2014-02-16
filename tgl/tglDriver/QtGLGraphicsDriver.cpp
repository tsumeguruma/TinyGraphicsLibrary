/*
 * QtGLGraphicsDriver.cpp
 */

#include "tglCore/GraphicsView.h"
#include "QtGLGraphicsDriver.h"

#include <iostream>
using namespace std;

namespace tgl {

QtGLGraphicsDriver::QtGLGraphicsDriver(QWidget* parent)
	: QGLWidget(parent)
{
	frameRate_ = 30;
	requireTerminate_ = false;

	QGLWidget::setMouseTracking(true);
	QGLWidget::setAutoBufferSwap(true);
	QGLWidget::setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
	QGLWidget::resize(640, 480);
}

QtGLGraphicsDriver::~QtGLGraphicsDriver() {

}

void QtGLGraphicsDriver::initialize(tgl::GraphicsView* view)
{
	GraphicsDriver::initialize(view);
}

void QtGLGraphicsDriver::execute()
{
	QGLWidget::show();

	connect(&timer_, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer_.start(1000/frameRate_);
}

void QtGLGraphicsDriver::terminate()
{
	requireTerminate_ = true;
}

void QtGLGraphicsDriver::setWindowSize(int width, int height)
{
	QGLWidget::resize(width, height);
}

void QtGLGraphicsDriver::setWindowTitle(const std::string& title)
{
	QGLWidget::setWindowTitle(title.c_str());
}

void QtGLGraphicsDriver::setFrameRate(int fps)
{
	frameRate_ = fps;
}

const std::string& QtGLGraphicsDriver::windowTitle() const
{
	return QGLWidget::windowTitle().toStdString();
}

int QtGLGraphicsDriver::width() const
{
	return QGLWidget::width();
}

int QtGLGraphicsDriver::height() const
{
	return QGLWidget::height();
}

int QtGLGraphicsDriver::frameRate() const
{
	return frameRate_;
}

// qt event
void QtGLGraphicsDriver::initializeGL()
{
	executeGraphicsViewInitializeEvent();
}

void QtGLGraphicsDriver::paintGL()
{
	executeGraphicsViewRenderEvent();
}

void QtGLGraphicsDriver::resizeGL(int width, int height)
{
	executeGraphicsViewResizeEvent(this->width(), this->height());
}

void QtGLGraphicsDriver::mousePressEvent(QMouseEvent *event)
{
	auto me = getGraphicsViewMouseEvent();
	tgl::MouseEvent::MouseBotton btn = tgl::MouseEvent::MouseBotton::NoButton;
	switch (event->buttons()) {
	case Qt::LeftButton: btn = tgl::MouseEvent::MouseBotton::LeftButton; break;
	case Qt::MiddleButton: btn = tgl::MouseEvent::MouseBotton::MiddleButton; break;
	case Qt::RightButton: btn = tgl::MouseEvent::MouseBotton::RightButton; break;
	default:
		btn = tgl::MouseEvent::MouseBotton::NoButton;
		break;
	}

	me->setPressEvent(event->x(), event->y(), btn);
	executeGraphicsViewMousePressEvent(me);
}

void QtGLGraphicsDriver::mouseMoveEvent(QMouseEvent *event)
{
	auto me = getGraphicsViewMouseEvent();
	tgl::MouseEvent::MouseBotton btn = tgl::MouseEvent::MouseBotton::NoButton;
	switch (event->buttons()) {
	case Qt::LeftButton: btn = tgl::MouseEvent::MouseBotton::LeftButton; break;
	case Qt::MiddleButton: btn = tgl::MouseEvent::MouseBotton::MiddleButton; break;
	case Qt::RightButton: btn = tgl::MouseEvent::MouseBotton::RightButton; break;
	default:
		btn = tgl::MouseEvent::MouseBotton::NoButton;
		break;
	}

	me->setMoveEvent(event->x(), event->y(), btn);
	executeGraphicsViewMouseMoveEvent(me);
}

void QtGLGraphicsDriver::mouseReleaseEvent(QMouseEvent *event)
{
	auto me = getGraphicsViewMouseEvent();
	tgl::MouseEvent::MouseBotton btn = tgl::MouseEvent::MouseBotton::NoButton;
	switch (event->buttons()) {
	case Qt::LeftButton: btn = tgl::MouseEvent::MouseBotton::LeftButton; break;
	case Qt::MiddleButton: btn = tgl::MouseEvent::MouseBotton::MiddleButton; break;
	case Qt::RightButton: btn = tgl::MouseEvent::MouseBotton::RightButton; break;
	default:
		btn = tgl::MouseEvent::MouseBotton::NoButton;
		break;
	}

	me->setReleaseEvent(event->x(), event->y(), btn);
	executeGraphicsViewMouseReleaseEvent(me);
}

void QtGLGraphicsDriver::wheelEvent(QWheelEvent *event)
{
	auto we = getGraphicsViewWheelEvent();
	we->setWheelEvent(event->x(), event->y(), event->delta());
	executeGraphicsViewWheelEvent(we);
}

void QtGLGraphicsDriver::keyPressEvent(QKeyEvent* event)
{
	auto ke = getGraphicsViewKeyEvent();
	ke->setKeyPressEvent(keymap(event->key()));
	executeGraphicsViewKeyPressEvent(ke);
}

Key QtGLGraphicsDriver::keymap(int key)
{
	switch (key) {
	case Qt::Key_Space: return Key::Key_Space; break;
	case Qt::Key_Apostrophe: return Key::Key_Unknown; break;
	case Qt::Key_Comma: return Key::Key_Comma; break;
	case Qt::Key_Minus: return Key::Key_Minus; break;
	case Qt::Key_Period: return Key::Key_Period; break;
	case Qt::Key_Slash: return Key::Key_Slash; break;
	case Qt::Key_0: return Key::Key_0; break;
	case Qt::Key_1: return Key::Key_1; break;
	case Qt::Key_2: return Key::Key_2; break;
	case Qt::Key_3: return Key::Key_3; break;
	case Qt::Key_4: return Key::Key_4; break;
	case Qt::Key_5: return Key::Key_5; break;
	case Qt::Key_6: return Key::Key_6; break;
	case Qt::Key_7: return Key::Key_7; break;
	case Qt::Key_8: return Key::Key_8; break;
	case Qt::Key_9: return Key::Key_9; break;
	case Qt::Key_Semicolon: return Key::Key_Semicolon; break;
	case Qt::Key_Equal: return Key::Key_Equal; break;
	case Qt::Key_A: return Key::Key_A; break;
	case Qt::Key_B: return Key::Key_B; break;
	case Qt::Key_C: return Key::Key_C; break;
	case Qt::Key_D: return Key::Key_D; break;
	case Qt::Key_E: return Key::Key_E; break;
	case Qt::Key_F: return Key::Key_F; break;
	case Qt::Key_G: return Key::Key_G; break;
	case Qt::Key_H: return Key::Key_H; break;
	case Qt::Key_I: return Key::Key_I; break;
	case Qt::Key_J: return Key::Key_J; break;
	case Qt::Key_K: return Key::Key_K; break;
	case Qt::Key_L: return Key::Key_L; break;
	case Qt::Key_M: return Key::Key_M; break;
	case Qt::Key_N: return Key::Key_N; break;
	case Qt::Key_O: return Key::Key_O; break;
	case Qt::Key_P: return Key::Key_P; break;
	case Qt::Key_Q: return Key::Key_Q; break;
	case Qt::Key_R: return Key::Key_R; break;
	case Qt::Key_S: return Key::Key_S; break;
	case Qt::Key_T: return Key::Key_T; break;
	case Qt::Key_U: return Key::Key_U; break;
	case Qt::Key_W: return Key::Key_W; break;
	case Qt::Key_X: return Key::Key_X; break;
	case Qt::Key_Y: return Key::Key_Y; break;
	case Qt::Key_Z: return Key::Key_Z; break;
	case Qt::Key_Escape: return Key::Key_Escape; break;
	case Qt::Key_Enter: return Key::Key_Enter; break;
	case Qt::Key_Tab: return Key::Key_Tab; break;
	case Qt::Key_Insert: return Key::Key_Insert; break;
	case Qt::Key_Delete: return Key::Key_Delete; break;
	case Qt::Key_Right: return Key::Key_Right; break;
	case Qt::Key_Left: return Key::Key_Left; break;
	case Qt::Key_Down: return Key::Key_Down; break;
	case Qt::Key_Up: return Key::Key_Up; break;
	case Qt::Key_PageDown: return Key::Key_PageDown; break;
	case Qt::Key_PageUp: return Key::Key_PageUp; break;
	case Qt::Key_Home: return Key::Key_Home; break;
	case Qt::Key_End: return Key::Key_End; break;

	case Qt::Key_F1: return Key::Key_F1; break;
	case Qt::Key_F2: return Key::Key_F2; break;
	case Qt::Key_F3: return Key::Key_F3; break;
	case Qt::Key_F4: return Key::Key_F4; break;
	case Qt::Key_F5: return Key::Key_F5; break;
	case Qt::Key_F6: return Key::Key_F6; break;
	case Qt::Key_F7: return Key::Key_F7; break;
	case Qt::Key_F8: return Key::Key_F8; break;
	case Qt::Key_F9: return Key::Key_F9; break;
	case Qt::Key_F10: return Key::Key_F10; break;
	case Qt::Key_F11: return Key::Key_F11; break;
	case Qt::Key_F12: return Key::Key_F12; break;
	case Qt::Key_Shift: return Key::Key_Shift; break;
	case Qt::Key_Control: return Key::Key_Control; break;
	case Qt::Key_Alt: return Key::Key_Alt; break;
	default:
		return Key::Key_Unknown;
		break;
	}

	return Key::Key_Unknown;
}
} /* namespace tgl */
