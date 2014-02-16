/*
 * GraphicsItem.h
 */

#ifndef TGL_CORE_GRAPHICSITEM_H_
#define TGL_CORE_GRAPHICSITEM_H_

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace tgl {

class GraphicsView;
class Renderer3D;
class Renderer2D;
class TextRenderer;
class GraphicsItemHoverEvent;
class GraphicsItemMouseEvent;
class GraphicsItemSelectEvent;

class GraphicsItem;
typedef std::shared_ptr<GraphicsItem> GraphicsItemPtr;

typedef std::vector<GraphicsItemPtr> GraphicsItemList;

class GraphicsItem {
	friend class GraphicsView;
public:
	GraphicsItem();
	virtual ~GraphicsItem();

	void setName(const std::string& name) { name_ = name; }
	const std::string& name() const { return name_; }

	virtual void addChild(GraphicsItemPtr item);
	virtual void removeChild(GraphicsItemPtr item);

	// visible == false の時はpickingされない
	bool isVisible() const { return visible_; }
	void setVisible(bool visible);

	GraphicsItem* parentItem() const { return parent_; }

	GraphicsItemList& childItems()  { return children_; }
	const GraphicsItemList& childItems() const { return children_; }
	size_t numChildItems() const { return children_.size(); }
	GraphicsItemPtr childItem(size_t i) const { return children_[i]; }

	GraphicsView* graphicsWindow() const { return graphicsView_; }
	int viewWidth() const;
	int viewHeight() const;

	static void traverse(GraphicsItemPtr item, std::function<void (GraphicsItemPtr)> func);
	static void traverseReverse(GraphicsItemPtr item, std::function<void (GraphicsItemPtr)> func);

protected:
	// render scene
	virtual void renderScene(tgl::Renderer3D* /* r */) {}
	virtual void renderOverlayScene(tgl::Renderer3D* /* r */) {}

	virtual void render2DScene(tgl::Renderer2D* /* r */) {}
	virtual void renderTextScene(tgl::TextRenderer* /* r */) {}

	// picking scene
	virtual void renderPickingScene(tgl::Renderer3D* r) { renderScene(r); }
	virtual void renderPickingOverlayScene(tgl::Renderer3D* r) {renderOverlayScene(r); }

	virtual void renderPicking2DScene(tgl::Renderer2D* r) { render2DScene(r); }
	virtual void renderPickingTextScene(tgl::TextRenderer* r) { renderTextScene(r); }

	// Hover event
	virtual void hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/) {}
	virtual void hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/) {}
	virtual void hoverMoveEvent(tgl::GraphicsItemHoverEvent* /*e*/) {}

	// Mouse event
	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* /*e*/) {}
	virtual void mouseMoveEvent(tgl::GraphicsItemMouseEvent* /*e*/) {}
	virtual void mouseReleaseEvent(tgl::GraphicsItemMouseEvent* /*e*/) {}

	// Select event
	virtual void selectEnterEvent(tgl::GraphicsItemSelectEvent* /*e*/) {}
	virtual void selectLeaveEvent(tgl::GraphicsItemSelectEvent* /*e*/) {}

private:
	std::string name_;

	GraphicsItem* parent_;
	GraphicsItemList children_;

	GraphicsView* graphicsView_;
	bool visible_;
};

} /* namespace tgl */

#endif /* TGL_CORE_GRAPHICSITEM_H_ */
