/*
 * AbstractButton.cpp
 */

#include "tglCore/GraphicsItemEvent.h"
#include "AbstractButton.h"

namespace tgl
{
namespace gui
{

AbstractButton::AbstractButton()
{
	checkable_ = false;
	checked_ = false;
	down_ = false;
}

AbstractButton::~AbstractButton()
{

}

bool AbstractButton::isCheckable() const
{
	return checkable_;
}

bool AbstractButton::isChecked() const
{
	return checked_;
}

bool AbstractButton::isDown() const
{
	return down_;
}

void AbstractButton::setCheckable(bool on)
{
	checkable_ = on;
}

void AbstractButton::setDown(bool down)
{
	down_ = down;
}

void AbstractButton::setText(const std::string& text)
{
	text_ = text;
}

const std::string& AbstractButton::text() const
{
	return text_;
}

// slots
void AbstractButton::click()
{

}

void AbstractButton::setChecked(bool checked)
{
	checked_ = checked;
}

void AbstractButton::toggle()
{

}

// Mouse event
void AbstractButton::mouseDoubleClickEvent(tgl::GraphicsItemMouseEvent* e)
{

}

void AbstractButton::mousePressEvent(tgl::GraphicsItemMouseEvent* e)
{
	if (isCheckable()) {

	} else {
		checked_ = true;
		setDown(isChecked());
	}

	// emit
	pressed_();
}

void AbstractButton::mouseMoveEvent(tgl::GraphicsItemMouseEvent* e)
{

}

void AbstractButton::mouseReleaseEvent(tgl::GraphicsItemMouseEvent* e)
{

	if (isCheckable()) {
		checked_ = !checked_;

		setDown(isChecked());
		// emit
		toggled_(isChecked());
	} else {
		checked_ = false;
		setDown(isChecked());
	}

	// emit
	clicked_(isChecked());

	// emit
	released_();
}

}
}
