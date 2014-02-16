/*
 * DockWidget.h
 */

#ifndef TGL_GUI_DOCKWIDGET_H_
#define TGL_GUI_DOCKWIDGET_H_

#include <boost/signals2.hpp>

#include "Widget.h"
#include "PushButton.h"

namespace tgl
{
namespace gui
{

class DockWidget;
typedef std::shared_ptr<DockWidget> DockWidgetPtr;

class DockWidget : public Widget
{
public:
	DockWidget();
	DockWidget(const std::string& windowTitle);
	virtual ~DockWidget();

	void setWindowTitle(const std::string& windowTitle) { windowTitle_ = windowTitle; }
	const std::string& windowTitle() const { return windowTitle_; }

	void setEnableTitleBar(bool enable);
	bool isEnabledTitleBar() const { return enableTitleBar_; }

	virtual void clearWidgets();

	virtual void removeWidget(WidgetPtr widget);
	virtual void addWidget(WidgetPtr widget);

	size_t numWidgets() const { return addedWidgets_.size(); }

	virtual void titleBarButtonToggled(bool toggle);
	virtual void titleBarButtonPressed();

protected:

	class TitleBarButton : public PushButton {
	public:
		virtual void render2DScene(tgl::Renderer2D* r);
		virtual void renderPicking2DScene(tgl::Renderer2D* r) { render2DScene(r); }
	};

	void init();

	// render
	virtual void render2DScene(tgl::Renderer2D* r);
	virtual void renderTextScene(tgl::TextRenderer* r);

	// Mouse event
	virtual void mouseDoubleClickEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseMoveEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseReleaseEvent(tgl::GraphicsItemMouseEvent* e);

	std::string windowTitle_;

	std::vector<WidgetPtr> addedWidgets_;

	PushButtonPtr titleBarButton_;

	bool enableTitleBar_;
	bool showWidgets_;

	int titleBarHeight_;
};

}
}

#endif /* TGL_GUI_DOCKWIDGET_H_ */
