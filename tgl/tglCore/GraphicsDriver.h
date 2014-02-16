/*
 * GraphicsDriver.h
 */

#ifndef GRAPHICSDRIVER_H_
#define GRAPHICSDRIVER_H_

#include <memory>
#include <string>

namespace tgl {

class GraphicsView;
class MouseEvent;
class WheelEvent;
class KeyEvent;

class GraphicsDriver;
typedef std::unique_ptr<GraphicsDriver> GraphicsDriverPtr;

class GraphicsDriver {
	friend class GraphicsView;
public:
	GraphicsDriver();
	virtual ~GraphicsDriver();

protected:

	virtual void initialize(GraphicsView* view) {
		view_ = view;
	}

	virtual void execute() = 0;

	virtual void terminate() = 0;

	virtual void setWindowSize(int width, int height) = 0;
	virtual void setWindowTitle(const std::string& title) = 0;
	virtual void setFrameRate(int fps) = 0;

	virtual const std::string& windowTitle() const = 0;
	virtual int width() const = 0;
	virtual int height() const = 0;
	virtual int frameRate() const = 0;

	void executeGraphicsViewInitializeEvent();
	void executeGraphicsViewRenderEvent();
	void executeGraphicsViewResizeEvent(int width, int height);

	void executeGraphicsViewMousePressEvent(MouseEvent* e);
	void executeGraphicsViewMouseMoveEvent(MouseEvent* e);
	void executeGraphicsViewMouseReleaseEvent(MouseEvent* e);
	void executeGraphicsViewWheelEvent(WheelEvent* e);
	void executeGraphicsViewKeyPressEvent(KeyEvent* e);

	MouseEvent* getGraphicsViewMouseEvent();
	WheelEvent* getGraphicsViewWheelEvent();
	KeyEvent* getGraphicsViewKeyEvent();

	GraphicsView* graphicsView() const { return view_; }

private:

	GraphicsView* view_;

};

} /* namespace tgl */

#endif /* GRAPHICSDRIVER_H_ */
