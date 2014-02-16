/*
 * InputEvent.h
 */

#ifndef TGL_CORE_INPUTEVENT_H_
#define TGL_CORE_INPUTEVENT_H_

#include "Common.h"

namespace tgl {

class KeyEvent
{
public:
	KeyEvent() : key_(Key::Key_Unknown) {}
	KeyEvent(Key key) : key_(key) {}

	virtual ~KeyEvent() {}

	void setKeyPressEvent(Key key) { key_ = key; }

	Key key() const { return key_; }

protected:
	Key key_;
};

class MouseEvent
{
public:

	enum class MouseBotton {
		NoButton,
		LeftButton,
		RightButton,
		MiddleButton,
	};

	MouseEvent() : x_(0), y_(0), dx_(0), dy_(0), button_(MouseBotton::NoButton) {}
	MouseEvent(int x, int y, MouseBotton button) :
		x_(x), y_(y), dx_(0), dy_(0), button_(button)
	{}

	virtual ~MouseEvent() {}

	virtual void setPressEvent(int x, int y, MouseBotton button) {
		x_ = x;
		y_ = y;
		button_ = button;
	}

	virtual void setMoveEvent(int x, int y, MouseBotton button) {
		dx_ = x - x_;
		dy_ = y - y_;
		x_ = x;
		y_ = y;
		button_ = button;
	}

	virtual void setReleaseEvent(int x, int y, MouseBotton button) {
		x_ = x;
		y_ = y;
		dx_ = 0;
		dy_ = 0;
		button_ = button;
	}

	MouseBotton button() const { return button_; }
	int x() const { return x_; }
	int y() const { return y_; }
	int dx() const { return dx_; }
	int dy() const { return dy_; }

protected:
	int x_;
	int y_;

	int dx_;
	int dy_;

	MouseBotton button_;
};

class WheelEvent
{
public:
	enum Orientation {
		Horizontal,
		Vertical,
	};

	WheelEvent() : x_(0), y_(0), delta_(0), orientation_(Vertical) {}
	WheelEvent(int x, int y, int delta, Orientation orientation = Vertical) :
		x_(x), y_(y), delta_(delta), orientation_(orientation)
	{}

	virtual ~WheelEvent() {}

	void setWheelEvent(int x, int y, int delta, Orientation orientation = Vertical) {
		x_ = x, y_ = y, delta_ = delta, orientation_ = orientation;
	}

	Orientation orientation() const { return orientation_; }
	int delta() const { return delta_; }
	int x() const { return x_; }
	int y() const { return y_; }

protected:
	int x_;
	int y_;
	int delta_;
	Orientation orientation_;
};

} /* namespace tgl */

#endif /* TGL_CORE_INPUTEVENT_H_ */
