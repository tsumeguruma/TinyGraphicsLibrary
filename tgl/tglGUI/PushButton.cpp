/*
 * PushButton.cpp
 */

#include "tglCore/Renderer2D.h"
#include "PushButton.h"

#include <iostream>
using namespace std;

namespace tgl
{
namespace gui
{

PushButton::PushButton()
{
	setSize(100, 18);
	checkedColor_ = {{1.0, 0.8, 0.8, 1.0}};
}

PushButton::PushButton(const std::string& text)
{
	setSize(100, 18);
	setText(text);
	checkedColor_ = {{1.0, 0.8, 0.8, 1.0}};
}

PushButton::~PushButton()
{

}

void PushButton::setCheckedColor(double r, double g, double b, double a)
{
	checkedColor_ = {{r, g, b, a}};
}

void PushButton::render2DScene(tgl::Renderer2D* r)
{
	if (!isEnabled() || !isVisible()) return;

	int x, y, w, h;
	x = this->x();
	y = this->y();
	Widget* parent = dynamic_cast<Widget*>(parentItem());
	if (parent) {
		x += parent->x();
		y += parent->y();
	}

	w = this->width();
	h = this->height();

	r->setStrokeColor(0.3, 0.3, 0.3);
	r->setStrokeWeight(1);

	if (isDown() || isChecked()) {
		r->setFillColor(checkedColor_[0], checkedColor_[1], checkedColor_[2], checkedColor_[3]);
	} else {
		r->setFillColor(1,1,1);
	}
	r->drawRect(x,y,w,h);

	// render text
	r->setTextColor(0,0,0);

	r->setTextSize(this->height()-2);
	r->setTextAlign(Renderer2D::Align::Left, Renderer2D::VAlign::Center);
	r->drawText(x+4, y + h/2, text());
}

}
}
