/*
 * Widget.h
 */

#ifndef TGL_GUI_WIDGET_H_
#define TGL_GUI_WIDGET_H_

#include "tglCore/GraphicsItem.h"

namespace tgl
{
namespace gui
{

class Widget;
typedef std::shared_ptr<Widget> WidgetPtr;

class Widget : public GraphicsItem
{
public:
	Widget();
	virtual ~Widget();

	void setEnabled(bool on) { enabled_ = on; }

	bool isEnabled() const { return enabled_; }

	void setSize(int width, int height);
	void setPos(int x, int y);

	int width() const { return width_; }
	int height() const { return height_; }

	int x() const { return x_; }
	int y() const { return y_; }

	virtual void addWidget(WidgetPtr widget);
	virtual void removeWidget(WidgetPtr widget);

protected:
	bool enabled_;

	int width_;
	int height_;

	int x_;
	int y_;
};

}
}

#endif /* TGL_GUI_WIDGET_H_ */
