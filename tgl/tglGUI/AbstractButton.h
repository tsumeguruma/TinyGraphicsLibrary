/*
 * AbstractButton.h
 */

#ifndef TGL_GUI_ABSTRACTBUTTON_H_
#define TGL_GUI_ABSTRACTBUTTON_H_

#include <string>
#include <boost/signals2.hpp>

#include "Widget.h"

namespace tgl
{
namespace gui
{

class AbstractButton;
typedef std::shared_ptr<AbstractButton> AbstractButtonPtr;

class AbstractButton : public Widget
{
public:
	AbstractButton();
	virtual ~AbstractButton();

	bool isCheckable() const;
	bool isChecked() const;
	bool isDown() const;

	virtual void setCheckable(bool on);
	virtual void setDown(bool down);
	virtual void setText(const std::string& text);

	const std::string& text() const;

	// slots
	virtual void click();
	virtual void setChecked(bool checked);
	virtual void toggle();

	// signals
	boost::signals2::signal<void (bool)>& sigClicked() { return clicked_; }
	boost::signals2::signal<void ()>& sigPressed() { return pressed_; }
	boost::signals2::signal<void ()>& sigReleased() { return released_; }
	boost::signals2::signal<void (bool)>& sigToggled() { return toggled_; }

protected:

	// Mouse event
	virtual void mouseDoubleClickEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseMoveEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseReleaseEvent(tgl::GraphicsItemMouseEvent* e);

	bool checkable_;
	bool checked_;
	bool down_;
	std::string text_;

	// signals
	boost::signals2::signal<void (bool)> clicked_;
	boost::signals2::signal<void ()> pressed_;
	boost::signals2::signal<void ()> released_;
	boost::signals2::signal<void (bool)> toggled_;
};

}
}

#endif /* TGL_GUI_ABSTRACTBUTTON_H_ */
