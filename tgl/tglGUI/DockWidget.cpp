/*
 * DockWidget.cpp
 */

#include <algorithm>

#include "tglCore/Renderer2D.h"
#include "tglCore/Renderer3D.h"
#include "tglCore/TextRenderer.h"
#include "tglCore/GraphicsItemEvent.h"
#include "DockWidget.h"

#include <iostream>
using namespace std;

namespace tgl
{
namespace gui
{

DockWidget::DockWidget()
{
	init();

	setWindowTitle("DockWidget");
	setSize(180, 380);
}

DockWidget::DockWidget(const std::string& windowTitle)
{
	init();

	setWindowTitle(windowTitle);
	setSize(180, 380);
}

DockWidget::~DockWidget()
{
	clearWidgets();
}

void DockWidget::init()
{
	titleBarButton_ = PushButtonPtr(new PushButton);
	Widget::addWidget(titleBarButton_);
	titleBarButton_->setText("");
	titleBarButton_->setSize(10, 10);
	titleBarButton_->setCheckable(true);
	titleBarButton_->setChecked(true);

	titleBarButton_->sigToggled().connect(boost::bind(&DockWidget::titleBarButtonToggled, this, _1));
	titleBarButton_->sigPressed().connect(boost::bind(&DockWidget::titleBarButtonPressed, this));

	enableTitleBar_ = true;
	titleBarHeight_ = 20;
	showWidgets_ = true;
}

void DockWidget::titleBarButtonToggled(bool toggle)
{
//	cout << "toggle : " << toggle << endl;
	showWidgets_ = toggle;

}

void DockWidget::titleBarButtonPressed()
{
//	cout << "pressed" << endl;
}

void DockWidget::clearWidgets()
{
	for (auto widget : addedWidgets_) {
		Widget::removeWidget(widget);
	}

	addedWidgets_.clear();
}

void DockWidget::removeWidget(WidgetPtr widget)
{
//	addedWidgets_.erase(std::remove(addedWidgets_.begin(), addedWidgets_.end(), widget), addedWidgets_.end());

	const auto remvitr = std::remove(addedWidgets_.begin(), addedWidgets_.end(), widget);
	for (auto itr = remvitr; itr != addedWidgets_.end(); ++itr) {
		Widget::removeWidget(*itr);
	}

	addedWidgets_.erase(remvitr, addedWidgets_.end());
}

void DockWidget::addWidget(WidgetPtr widget)
{
	Widget::addWidget(widget);
	addedWidgets_.push_back(widget);
}

void DockWidget::render2DScene(tgl::Renderer2D* r)
{
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
	int hy = 0;

	r->setRectMode(tgl::Renderer2D::Mode::Corner);
	r->setStrokeWeight(1);
	r->setStrokeColor(0.3, 0.3, 0.3);

	// draw title bar
	if (isEnabledTitleBar()) {
		titleBarButton_->setVisible(true);
		titleBarButton_->setPos(w - 20, titleBarHeight_/4);

		double tc[] = {0.1, 0.1, 0.1, 0.8};
		r->setFillColor(tc[0], tc[1], tc[2], tc[3]);
		r->drawRect(x, y, w, titleBarHeight_);

		y += titleBarHeight_;
		hy = titleBarHeight_;
	} else {
		titleBarButton_->setVisible(false);
	}

	// draw widgets container
	if (!showWidgets_) {
		for (size_t i = 0; i < this->addedWidgets_.size(); ++i) {
			WidgetPtr child = addedWidgets_[i];
			if (child) {
				child->setVisible(false);
			}
		}

		return;

	} else {
		for (size_t i = 0; i < this->addedWidgets_.size(); ++i) {
			WidgetPtr child = addedWidgets_[i];
			if (child) {
				child->setVisible(true);
			}
		}
	}

	int padx = 4;
	int pady = 4;
	int xx = x + padx;
	int yy = hy + pady;
	w = this->width();
	h = pady;

	for (size_t i = 0; i < this->addedWidgets_.size(); ++i) {
		WidgetPtr child = addedWidgets_[i];
		if (child) {
			int cw = this->width() - padx*2;
			child->setSize(cw, child->height());
			child->setPos(padx, yy);
			yy += pady + child->height();
			h += child->height() + pady;
		}
	}

	if (isEnabledTitleBar()) {
		r->setFillColor(1,1,1, 0.4);

		r->setStrokeColor(0.3, 0.3, 0.3);
		r->setStrokeWeight(1);

		r->drawRect(x,y,w,h);
	}
}

void DockWidget::renderTextScene(tgl::TextRenderer* r)
{
	if (!isEnabledTitleBar() || windowTitle_.empty()) return;

	int x = this->x();
	int y = this->y();
	Widget* parent = dynamic_cast<Widget*>(parentItem());
	if (parent) {
		x += parent->x();
		y += parent->y();
	}

	x += 20;
	y += titleBarHeight_ - 5;

	r->setTextColor(1,1,1);
	r->drawText(x, y, windowTitle_);
}

void DockWidget::mouseDoubleClickEvent(tgl::GraphicsItemMouseEvent* e)
{

}

void DockWidget::mousePressEvent(tgl::GraphicsItemMouseEvent* e)
{

}

void DockWidget::mouseMoveEvent(tgl::GraphicsItemMouseEvent* e)
{
	setPos(this->x() + e->dx(), this->y() + e->dy());
}

void DockWidget::mouseReleaseEvent(tgl::GraphicsItemMouseEvent* e)
{

}

void DockWidget::TitleBarButton::render2DScene(tgl::Renderer2D* r)
{
	int x = this->x();
	int y = this->y();
	Widget* parent = dynamic_cast<Widget*>(parentItem());
	if (parent) {
		x += parent->x();
		y += parent->y();
	}

	r->setFillColor(0.9, 0.9, 0.9);
	r->drawCircle(x, y, 8);
}

}
}
