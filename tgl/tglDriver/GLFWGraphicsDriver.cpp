/*
 * GLFWGraphicDriver.cpp
 */

#include <GLFW/glfw3.h>
#include "tglCore/GraphicsView.h"
#include "GLFWGraphicsDriver.h"

#include <iostream>
using namespace std;

namespace tgl {

std::atomic<bool> GLFWGraphicsDriver::disableInitializeGLFW_(false);
std::atomic<bool> GLFWGraphicsDriver::disableTerminateGLFW_(false);
std::once_flag GLFWGraphicsDriver::init_once_flag_;
std::atomic<int> GLFWGraphicsDriver::terminateCount_(0);
std::mutex GLFWGraphicsDriver::staticMutex_;

GLFWGraphicsDriver::GLFWGraphicsDriver() {
	glfwWindow_ = nullptr;
	width_ = 640;
	height_ = 480;

	windowTitle_ = "GraphicsView";

	requireResizeEvent_ = false;
	requireMousePressEvent_ = false;
	requireMouseMoveEvent_ = false;
	requireMouseReleaseEvent_ = false;
	requireMouseWheelEvent_ = false;
	requireMouseKeyPressEvent_ = false;

	isMousePressed_ = false;
	pressedMouseButton_ = tgl::MouseEvent::MouseBotton::NoButton;
	mouseCursorX_ = 0;
	mouseCursorY_ = 0;

	++terminateCount_;
}

GLFWGraphicsDriver::~GLFWGraphicsDriver() {

	cout << "~GLFWGraphicsDriver" << endl;

	terminate();

	try {
		if (thread_ && thread_->joinable()) {
			thread_->join();
		}
	} catch (...) {

	}

	if (glfwWindow_) {
		glfwDestroyWindow(glfwWindow_);
	}

	--terminateCount_;
	if (terminateCount_ == 0) {
		if (!disableTerminateGLFW_.load()) {
			glfwTerminate();
		}
	}

	if (terminateCount_ < 0) {
		terminateCount_ = 0;
	}
}

void GLFWGraphicsDriver::disableInitializeGLFW(bool disable)
{
	disableInitializeGLFW_ = disable;
}

void GLFWGraphicsDriver::disableTerminateGLFW(bool disable)
{
	disableTerminateGLFW_ = disable;
}

void GLFWGraphicsDriver::setWindowSize(int width, int height) {
	width_ = width;
	height_ = height;

	if (glfwWindow_) {
		glfwSetWindowSize(glfwWindow_, width_, height_);
	}
}

void GLFWGraphicsDriver::setWindowTitle(const std::string& title) {
	windowTitle_ = title;

	if (glfwWindow_) {
		glfwSetWindowTitle(glfwWindow_, windowTitle_.c_str());
	}
}

void GLFWGraphicsDriver::setFrameRate(int fps) {
	frameRate_ = fps;
}

void GLFWGraphicsDriver::initialize(tgl::GraphicsView* view) {
	if (!view) return;

	std::unique_lock<std::mutex> lock(staticMutex_);

	std::call_once(init_once_flag_, [&](){
		if (!disableInitializeGLFW_.load()) {
			glfwInit();
		}
	});

	GraphicsDriver::initialize(view);
}

void GLFWGraphicsDriver::execute() {
	if (!graphicsView()) return;

	std::unique_lock<std::mutex> lock(staticMutex_);

	std::call_once(init_once_flag_, [&](){
		if (!disableInitializeGLFW_.load()) {
			glfwInit();
		}
	});

	std::atomic<bool> threadBegin(false);

	thread_ = std::move(std::unique_ptr<std::thread>(new std::thread([&](){

		glfwWindow_ = glfwCreateWindow(width_, height_, windowTitle_.c_str(), NULL, NULL);

		glfwMakeContextCurrent(glfwWindow_);

		// set this pointer
		glfwSetWindowUserPointer(glfwWindow_, this);

		// set callback
		glfwSetMouseButtonCallback(glfwWindow_, mouseButtonEvent);
		glfwSetCursorEnterCallback(glfwWindow_, cursorEnterEvent);
		glfwSetCursorPosCallback(glfwWindow_, cursorPosEvent);
		glfwSetScrollCallback(glfwWindow_, scrollEvent);
		glfwSetKeyCallback(glfwWindow_, keyEvent);

		glfwSetWindowSizeCallback(glfwWindow_, resizeEvent);

		threadBegin = true;

		resizeGL(width_, height_);

		executeGraphicsViewInitializeEvent();

		while (!glfwWindowShouldClose(glfwWindow_)) {

			// handle events
			handleEvents();

			/* Render here */
			executeGraphicsViewRenderEvent();

			/* Swap front and back buffers */
			glfwSwapBuffers(glfwWindow_);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glfwDestroyWindow(glfwWindow_);
		glfwWindow_ = nullptr;

		cout << "thread end" << endl;
	})));

	// GLFWの初期化が終わるまで待機
	while (!threadBegin.load()) {

	}
}

void GLFWGraphicsDriver::terminate() {
	std::unique_lock<std::mutex> lock(staticMutex_);
	if (glfwWindow_) {
		glfwSetWindowShouldClose(glfwWindow_, GL_TRUE);
	}
}

void GLFWGraphicsDriver::resizeGL(int x, int y) {
	width_ = x;
	height_ = y;
	requireResizeEvent_ = true;
}

void GLFWGraphicsDriver::mouseButtonEvent(GLFWwindow *window, int button, int action, int mods) {
	GLFWGraphicsDriver* driver = static_cast<GLFWGraphicsDriver*>(glfwGetWindowUserPointer(window));
	if (!driver) return;

	tgl::MouseEvent::MouseBotton btn = tgl::MouseEvent::MouseBotton::NoButton;
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT: btn = tgl::MouseEvent::MouseBotton::LeftButton; break;
	case GLFW_MOUSE_BUTTON_MIDDLE: btn = tgl::MouseEvent::MouseBotton::MiddleButton; break;
	case GLFW_MOUSE_BUTTON_RIGHT: btn = tgl::MouseEvent::MouseBotton::RightButton; break;
	default:
		btn = tgl::MouseEvent::MouseBotton::NoButton;
		break;
	}

	auto event = driver->getGraphicsViewMouseEvent();

	if (action == GLFW_PRESS) {
		driver->isMousePressed_ = true;
		driver->pressedMouseButton_ = btn;
		event->setPressEvent((int)driver->mouseCursorX_, (int)driver->mouseCursorY_, btn);

		driver->requireMousePressEvent_ = true;
	} else if (action == GLFW_RELEASE) {
		driver->isMousePressed_ = false;
		driver->pressedMouseButton_ = tgl::MouseEvent::MouseBotton::NoButton;
		event->setReleaseEvent((int)driver->mouseCursorX_, (int)driver->mouseCursorY_, btn);

		driver->requireMouseReleaseEvent_ = true;
	}
}

void GLFWGraphicsDriver::cursorPosEvent(GLFWwindow *window, double x, double y) {
	GLFWGraphicsDriver* driver = static_cast<GLFWGraphicsDriver*>(glfwGetWindowUserPointer(window));
	if (!driver) return;

	driver->mouseCursorX_ = x;
	driver->mouseCursorY_ = y;

	auto event = driver->getGraphicsViewMouseEvent();

	event->setMoveEvent((int)driver->mouseCursorX_, (int)driver->mouseCursorY_, driver->pressedMouseButton_);

	driver->requireMouseMoveEvent_ = true;
}

void GLFWGraphicsDriver::cursorEnterEvent(GLFWwindow *window, int enter) {
	GLFWGraphicsDriver* driver = static_cast<GLFWGraphicsDriver*>(glfwGetWindowUserPointer(window));
	if (!driver) return;

//		cout << "cursorEnterEvent" << endl;


}

void GLFWGraphicsDriver::scrollEvent(GLFWwindow *window, double x, double y) {
	GLFWGraphicsDriver* driver = static_cast<GLFWGraphicsDriver*>(glfwGetWindowUserPointer(window));
	if (!driver) return;

	auto event = driver->getGraphicsViewWheelEvent();

	int delta = 80*y;
	event->setWheelEvent(x, y, delta);

	driver->requireMouseWheelEvent_ = true;
}

void GLFWGraphicsDriver::keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
	GLFWGraphicsDriver* driver = static_cast<GLFWGraphicsDriver*>(glfwGetWindowUserPointer(window));
	if (!driver) return;

	if (action == GLFW_PRESS) {
		auto event = driver->getGraphicsViewKeyEvent();
		event->setKeyPressEvent(GLFWGraphicsDriver::keymap(key));
		driver->requireMouseKeyPressEvent_ = true;
	} else if (action == GLFW_RELEASE) {

	} else if (action == GLFW_REPEAT) {

	}

}

void GLFWGraphicsDriver::resizeEvent(GLFWwindow *window, int width, int height) {
	GLFWGraphicsDriver* driver = static_cast<GLFWGraphicsDriver*>(glfwGetWindowUserPointer(window));
	if (!driver) return;

	driver->resizeGL(width, height);
}

void GLFWGraphicsDriver::handleEvents()
{
	// handle resize event
	if (requireResizeEvent_) {
		executeGraphicsViewResizeEvent(width_, height_);
		requireResizeEvent_ = false;
	}

	if (requireMousePressEvent_) {
		executeGraphicsViewMousePressEvent(getGraphicsViewMouseEvent());
		requireMousePressEvent_ = false;
	}

	if (requireMouseMoveEvent_) {
		executeGraphicsViewMouseMoveEvent(getGraphicsViewMouseEvent());
		requireMouseMoveEvent_ = false;
	}

	if (requireMouseReleaseEvent_) {
		executeGraphicsViewMouseReleaseEvent(getGraphicsViewMouseEvent());
		requireMouseReleaseEvent_ = false;
	}

	if (requireMouseWheelEvent_) {
		executeGraphicsViewWheelEvent(getGraphicsViewWheelEvent());
		requireMouseWheelEvent_ = false;
	}

	if (requireMouseKeyPressEvent_) {
		executeGraphicsViewKeyPressEvent(getGraphicsViewKeyEvent());
		requireMouseKeyPressEvent_ = false;
	}
}

Key GLFWGraphicsDriver::keymap(int key)
{
	switch (key) {
	case GLFW_KEY_SPACE: return Key::Key_Space; break;
	case GLFW_KEY_APOSTROPHE: return Key::Key_Unknown; break;
	case GLFW_KEY_COMMA: return Key::Key_Comma; break;
	case GLFW_KEY_MINUS: return Key::Key_Minus; break;
	case GLFW_KEY_PERIOD: return Key::Key_Period; break;
	case GLFW_KEY_SLASH: return Key::Key_Slash; break;
	case GLFW_KEY_0: return Key::Key_0; break;
	case GLFW_KEY_1: return Key::Key_1; break;
	case GLFW_KEY_2: return Key::Key_2; break;
	case GLFW_KEY_3: return Key::Key_3; break;
	case GLFW_KEY_4: return Key::Key_4; break;
	case GLFW_KEY_5: return Key::Key_5; break;
	case GLFW_KEY_6: return Key::Key_6; break;
	case GLFW_KEY_7: return Key::Key_7; break;
	case GLFW_KEY_8: return Key::Key_8; break;
	case GLFW_KEY_9: return Key::Key_9; break;
	case GLFW_KEY_SEMICOLON: return Key::Key_Semicolon; break;
	case GLFW_KEY_EQUAL: return Key::Key_Equal; break;
	case GLFW_KEY_A: return Key::Key_A; break;
	case GLFW_KEY_B: return Key::Key_B; break;
	case GLFW_KEY_C: return Key::Key_C; break;
	case GLFW_KEY_D: return Key::Key_D; break;
	case GLFW_KEY_E: return Key::Key_E; break;
	case GLFW_KEY_F: return Key::Key_F; break;
	case GLFW_KEY_G: return Key::Key_G; break;
	case GLFW_KEY_H: return Key::Key_H; break;
	case GLFW_KEY_I: return Key::Key_I; break;
	case GLFW_KEY_J: return Key::Key_J; break;
	case GLFW_KEY_K: return Key::Key_K; break;
	case GLFW_KEY_L: return Key::Key_L; break;
	case GLFW_KEY_M: return Key::Key_M; break;
	case GLFW_KEY_N: return Key::Key_N; break;
	case GLFW_KEY_O: return Key::Key_O; break;
	case GLFW_KEY_P: return Key::Key_P; break;
	case GLFW_KEY_Q: return Key::Key_Q; break;
	case GLFW_KEY_R: return Key::Key_R; break;
	case GLFW_KEY_S: return Key::Key_S; break;
	case GLFW_KEY_T: return Key::Key_T; break;
	case GLFW_KEY_U: return Key::Key_U; break;
	case GLFW_KEY_W: return Key::Key_W; break;
	case GLFW_KEY_X: return Key::Key_X; break;
	case GLFW_KEY_Y: return Key::Key_Y; break;
	case GLFW_KEY_Z: return Key::Key_Z; break;
	case GLFW_KEY_ESCAPE: return Key::Key_Escape; break;
	case GLFW_KEY_ENTER: return Key::Key_Enter; break;
	case GLFW_KEY_TAB: return Key::Key_Tab; break;
	case GLFW_KEY_INSERT: return Key::Key_Insert; break;
	case GLFW_KEY_DELETE: return Key::Key_Delete; break;
	case GLFW_KEY_RIGHT: return Key::Key_Right; break;
	case GLFW_KEY_LEFT: return Key::Key_Left; break;
	case GLFW_KEY_DOWN: return Key::Key_Down; break;
	case GLFW_KEY_UP: return Key::Key_Up; break;
	case GLFW_KEY_PAGE_DOWN: return Key::Key_PageDown; break;
	case GLFW_KEY_PAGE_UP: return Key::Key_PageUp; break;
	case GLFW_KEY_HOME: return Key::Key_Home; break;
	case GLFW_KEY_END: return Key::Key_End; break;

	case GLFW_KEY_F1: return Key::Key_F1; break;
	case GLFW_KEY_F2: return Key::Key_F2; break;
	case GLFW_KEY_F3: return Key::Key_F3; break;
	case GLFW_KEY_F4: return Key::Key_F4; break;
	case GLFW_KEY_F5: return Key::Key_F5; break;
	case GLFW_KEY_F6: return Key::Key_F6; break;
	case GLFW_KEY_F7: return Key::Key_F7; break;
	case GLFW_KEY_F8: return Key::Key_F8; break;
	case GLFW_KEY_F9: return Key::Key_F9; break;
	case GLFW_KEY_F10: return Key::Key_F10; break;
	case GLFW_KEY_F11: return Key::Key_F11; break;
	case GLFW_KEY_F12: return Key::Key_F12; break;
	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_RIGHT_SHIFT: return Key::Key_Shift; break;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL: return Key::Key_Control; break;
	case GLFW_KEY_LEFT_ALT:
	case GLFW_KEY_RIGHT_ALT: return Key::Key_Alt; break;
	default:
		return Key::Key_Unknown;
		break;
	}

	return Key::Key_Unknown;
}

} /* namespace tgl */
