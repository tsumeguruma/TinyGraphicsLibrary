/*
 * GLFWGraphicsDriver.h
 */

#ifndef TGL_GLFWGRAPHICSDRIVER_H_
#define TGL_GLFWGRAPHICSDRIVER_H_

#include <thread>
#include <mutex>
#include <atomic>
#include "tglCore/Common.h"
#include "tglCore/GraphicsDriver.h"
#include "tglCore/InputEvent.h"

class GLFWwindow;

namespace tgl {

class GLFWGraphicsDriver;
typedef std::shared_ptr<GLFWGraphicsDriver> GLFWGraphicsDriverPtr;

class GLFWGraphicsDriver : public tgl::GraphicsDriver {
public:
	GLFWGraphicsDriver();
	virtual ~GLFWGraphicsDriver();

	static void disableInitializeGLFW(bool disable = true);
	static void disableTerminateGLFW(bool disable = true);

protected:
	virtual void initialize(tgl::GraphicsView* view);
	virtual void execute();
	virtual void terminate();

	virtual void setWindowSize(int width, int height);
	virtual void setWindowTitle(const std::string& title);
	virtual void setFrameRate(int fps);

	virtual const std::string& windowTitle() const { return windowTitle_; }
	virtual int width() const { return width_; }
	virtual int height() const { return height_; }
	virtual int frameRate() const { return frameRate_; }

	// event
	static void mouseButtonEvent(GLFWwindow *window, int button, int action, int mods);
	static void cursorPosEvent(GLFWwindow *window, double x, double y);
	static void cursorEnterEvent(GLFWwindow *window, int enter);
	static void scrollEvent(GLFWwindow *window, double x, double y);
	static void keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void resizeEvent(GLFWwindow *window, int width, int height);

	void resizeGL(int x, int y);

	void handleEvents();

	static Key keymap(int key);

	GLFWwindow* glfwWindow_;

	bool isMousePressed_;
	tgl::MouseEvent::MouseBotton pressedMouseButton_;
	double mouseCursorX_;
	double mouseCursorY_;

	std::atomic<bool> requireResizeEvent_;
	std::atomic<bool> requireMousePressEvent_;
	std::atomic<bool> requireMouseMoveEvent_;
	std::atomic<bool> requireMouseReleaseEvent_;
	std::atomic<bool> requireMouseWheelEvent_;
	std::atomic<bool> requireMouseKeyPressEvent_;

	std::string windowTitle_;
	int width_;
	int height_;

	int frameRate_;

	static std::atomic<bool> disableInitializeGLFW_;
	static std::atomic<bool> disableTerminateGLFW_;
	static std::once_flag init_once_flag_;
	static std::atomic<int> terminateCount_;
	static std::mutex staticMutex_;

	std::unique_ptr<std::thread> thread_;
};

} /* namespace tgl */

#endif /* GLFWGRAPHICDRIVER_H_ */
