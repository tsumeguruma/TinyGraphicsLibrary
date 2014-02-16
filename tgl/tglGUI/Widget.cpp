/*
 * Widget.cpp
 */

#include "Widget.h"

namespace tgl
{
namespace gui
{

Widget::Widget()
{
	enabled_ = true;

	width_ = 0;
	height_ = 0;

	x_ = 0;
	y_ = 0;
}

Widget::~Widget()
{

}

void Widget::setSize(int width, int height)
{
	width_ = width;
	height_ = height;
}

void Widget::setPos(int x, int y)
{
	x_ = x;
	y_ = y;
}

void Widget::addWidget(WidgetPtr widget)
{
	addChild(widget);
}

void Widget::removeWidget(WidgetPtr widget)
{
	removeChild(widget);
}

}
}
