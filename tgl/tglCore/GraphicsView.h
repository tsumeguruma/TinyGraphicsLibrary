/*
 * GraphicsView.h
 */

#ifndef TGL_CORE_GRAPHICSVIEW_H_
#define TGL_CORE_GRAPHICSVIEW_H_

#include <map>
#include <vector>
#include <array>
#include <string>
#include <atomic>
#include <unordered_map>
#include <boost/any.hpp>

#include <GL/gl.h>
#include <GL/glu.h>

#include "InputEvent.h"
#include "GraphicsItem.h"
#include "GraphicsItemEvent.h"
#include "Renderer3D.h"
#include "Renderer2D.h"
#include "TextRenderer.h"
#include "Camera.h"
#include "Light.h"

#include "StandardCamera.h"
#include "SphericalCamera.h"

namespace tgl {

class GraphicsDriver;

class GraphicsView {
	friend class GraphicsDriver;
public:
	GraphicsView(std::unique_ptr<GraphicsDriver> driver);
	virtual ~GraphicsView();

	virtual void initialize();
	virtual void execute();
	virtual void terminate();

	void setWindowTitle(const std::string& title);
	const std::string& windowTitle() const;

	void setWindowSize(int width, int height);
	int width() const;
	int height() const;

	void setFrameRate(int fps);
	int frameRate() const;

	// Camera
	void setCamera(CameraPtr camera);
	CameraPtr camera() const { return camera_; }

	// Light
	void addLight(LightPtr light);
	void removeLight(LightPtr light);
	LightPtr light(size_t index = 0) const;
	size_t numLights() const { return lightList_.size(); }

	// GraphicsItem
	void addGraphicsItem(GraphicsItemPtr item);
	void removeGraphicsItem(GraphicsItemPtr item);
	int numGraphicsItems() const { return graphicsItems_.size(); }
	GraphicsItemPtr graphicsItem(int i) { return graphicsItems_[i]; }

	//  settings
	void setBackgroundColor(double r, double g, double b, double a = 1.0);

	typedef std::unordered_map<std::string, boost::any> ExtentionsType;
	ExtentionsType& extensions() { return extensions_; }
	const ExtentionsType& extensions() const { return extensions_; }


protected:

	// execute pre/post process
	virtual void executePrevProcess() {};
	virtual void executePostProcess() {};

	// render scene
	virtual void renderScene(tgl::Renderer3D* /*r*/) {};
	virtual void renderOverlayScene(tgl::Renderer3D* /*r*/) {};
	virtual void render2DScene(tgl::Renderer2D* /*r*/) {};
	virtual void renderTextScene(tgl::TextRenderer* /*r*/) {};

	virtual void initializeEvent();
	virtual void resizeEvent(int width, int height);
	virtual void renderEvent();

	virtual void mousePressEvent(MouseEvent* e);
	virtual void mouseMoveEvent(MouseEvent* e);
	virtual void mouseReleaseEvent(MouseEvent* e);
	virtual void wheelEvent(WheelEvent* e);

	virtual void keyPressEvent(KeyEvent* e);

	// post event
	virtual void mousePressPostEvent(MouseEvent*) {}
	virtual void mouseMovePostEvent(MouseEvent*) {}
	virtual void mouseReleasePostEvent(MouseEvent*) {}

	// handle GraphicsItem event
	void graphicsItemMousePressEvent(MouseEvent* e);
	void graphicsItemMouseMoveEvent(MouseEvent* e);
	void graphicsItemMouseReleaseEvent(MouseEvent* e);

	void renderSceneOfGrahicsItems();
	void renderOverlaySceneOfGrahicsItems();
	void render2DSceneOfGrahicsItems();
	void renderTextSceneOfGrahicsItems();

	// picking event
	GraphicsItemList pickingUpSceneGrahicsItems(int x, int y);
	GraphicsItemList pickingUpOverlaySceneGrahicsItems(int x, int y);
	GraphicsItemList pickingUp2DSceneGrahicsItems(int x, int y);
	GraphicsItemList pickingUpGrahicsItems(int x, int y);
	GraphicsItemList selectHitsGrahicsItems(GLuint hits, GLuint* buf, const std::map<int, GraphicsItemPtr>& indexToGraphicsItemMap);

	void traverseGraphicsItems(GraphicsItemList& items);

	std::unique_ptr<Renderer3D> renderer3D_;
	std::unique_ptr<Renderer2D> renderer2D_;
	std::unique_ptr<TextRenderer> textRenderer_;

	CameraPtr camera_;
	std::vector<LightPtr> lightList_;

	int frameRate_;

	float perspectiveFovy_;		// 縦の視野角を”度”単位
	float perspectiveZnear_;		// 一番近いZ位置
	float perspectiveZfar_;		// 一番遠いZ位置

	std::array<double, 4> backgroundColor_;
	std::array<GLint, 4> viewport_;

	// event
	std::unique_ptr<MouseEvent> mouseEvent_;
	std::unique_ptr<WheelEvent> wheelEvent_;
	std::unique_ptr<KeyEvent> keyEvent_;

	// GraphicsItem
	GraphicsItemList graphicsItems_;

	GraphicsItemPtr mouseGraphicsItem_;
	GraphicsItemPtr pickedGraphicsItem_;
	GraphicsItemPtr hoveredGraphicsItem_;

	std::unique_ptr<GraphicsItemMouseEvent> graphicsItemMouseEvent_;
	std::unique_ptr<GraphicsItemHoverEvent> graphicsItemHoverEvent_;
	std::unique_ptr<GraphicsItemSelectEvent> graphicsItemSelectEvent_;

	std::vector<GraphicsItemPtr> traversedItems_;

	// extensions
	ExtentionsType extensions_;

	// driver
	std::atomic<bool> initialized_;
	std::unique_ptr<GraphicsDriver> driver_;
};

} /* namespace tgl */

#endif /* TGL_CORE_GRAPHICSVIEW_H_ */
