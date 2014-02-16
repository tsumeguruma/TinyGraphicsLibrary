/*
 * QtGLGraphicsDriver.h
 */

#ifndef QTGLGRAPHICSDRIVER_H_
#define QTGLGRAPHICSDRIVER_H_

#include <memory>
#include <atomic>
#include <QtOpenGL>
#include "tglCore/Common.h"
#include "tglCore/GraphicsDriver.h"
#include "tglCore/InputEvent.h"

namespace tgl {

class QtGLGraphicsDriver;
typedef std::shared_ptr<QtGLGraphicsDriver> QtGLGraphicsDriverPtr;

class QtGLGraphicsDriver : public QGLWidget, public tgl::GraphicsDriver {
	Q_OBJECT
public:
	QtGLGraphicsDriver(QWidget* parent = nullptr);
	virtual ~QtGLGraphicsDriver();

protected:
	virtual void initialize(tgl::GraphicsView* view);
	virtual void execute();
	virtual void terminate();

	virtual void setWindowSize(int width, int height);
	virtual void setWindowTitle(const std::string& title);
	virtual void setFrameRate(int fps);

	virtual const std::string& windowTitle() const;
	virtual int width() const;
	virtual int height() const;
	virtual int frameRate() const;

	// qt event
	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int width, int height);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

	virtual void keyPressEvent(QKeyEvent* event);

	Key keymap(int key);

	int frameRate_;

	QTimer timer_;
	std::atomic<bool> requireTerminate_;
};

} /* namespace tgl */

#endif /* QTGLGRAPHICSDRIVER_H_ */
