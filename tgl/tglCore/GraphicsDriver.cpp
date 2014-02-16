/*
 * GraphicsDriver.cpp
 */

#include "GraphicsView.h"
#include "GraphicsDriver.h"

namespace tgl {

GraphicsDriver::GraphicsDriver() {
	view_ = nullptr;
}

GraphicsDriver::~GraphicsDriver() {

}

void GraphicsDriver::executeGraphicsViewInitializeEvent()
{
	if (view_) view_->initializeEvent();
}

void GraphicsDriver::executeGraphicsViewRenderEvent()
{
	if (view_) view_->renderEvent();
}

void GraphicsDriver::executeGraphicsViewResizeEvent(int width, int height)
{
	if (view_) view_->resizeEvent(width, height);
}

void GraphicsDriver::executeGraphicsViewMousePressEvent(MouseEvent* e)
{
	if (view_) view_->mousePressEvent(e);
}

void GraphicsDriver::executeGraphicsViewMouseMoveEvent(MouseEvent* e)
{
	if (view_) view_->mouseMoveEvent(e);
}

void GraphicsDriver::executeGraphicsViewMouseReleaseEvent(MouseEvent* e)
{
	if (view_) view_->mouseReleaseEvent(e);
}

void GraphicsDriver::executeGraphicsViewWheelEvent(WheelEvent* e)
{
	if (view_) view_->wheelEvent(e);
}

void GraphicsDriver::executeGraphicsViewKeyPressEvent(KeyEvent* e)
{
	if (view_) view_->keyPressEvent(e);
}

MouseEvent* GraphicsDriver::getGraphicsViewMouseEvent()
{
	return view_ ? view_->mouseEvent_.get() : nullptr;
}

WheelEvent* GraphicsDriver::getGraphicsViewWheelEvent()
{
	return view_ ? view_->wheelEvent_.get() : nullptr;
}

KeyEvent* GraphicsDriver::getGraphicsViewKeyEvent()
{
	return view_ ? view_->keyEvent_.get() : nullptr;
}

} /* namespace tgl */
