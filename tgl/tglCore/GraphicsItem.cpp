/*
 * GraphicsItem.cpp
 */

#include <algorithm>
#include "GraphicsView.h"
#include "GraphicsItem.h"

namespace tgl {

GraphicsItem::GraphicsItem() {

	parent_ = nullptr;
	graphicsView_ = nullptr;
	visible_ = true;
}

GraphicsItem::~GraphicsItem() {
	for (auto ptr : children_) {
		ptr->parent_ = nullptr;
		ptr->graphicsView_ = nullptr;
	}
}

void GraphicsItem::addChild(GraphicsItemPtr item)
{
	if (item->parentItem()) {
		item->parentItem()->removeChild(item);
	}
	item->parent_ = this;
	item->graphicsView_ = this->graphicsView_;
	children_.push_back(item);
}

void GraphicsItem::removeChild(GraphicsItemPtr item)
{
	auto itr = std::find(children_.begin(), children_.end(), item);
	if (itr != children_.end()) {
		(*itr)->parent_ = nullptr;
		(*itr)->graphicsView_ = nullptr;
		children_.erase(itr);
	}
}

void GraphicsItem::setVisible(bool visible)
{
	visible_ = visible;
	for (auto ptr : children_) {
		ptr->setVisible(visible);
	}
}

int GraphicsItem::viewWidth() const
{
	return graphicsView_ ? graphicsView_->width() : 0;
}

int GraphicsItem::viewHeight() const
{
	return graphicsView_ ? graphicsView_->height() : 0;
}

void GraphicsItem::traverse(GraphicsItemPtr item, std::function<void (GraphicsItemPtr)> func)
{
	if (!item) return;

	func(item);
	for (auto ptr : item->children_) {
		traverse(ptr, func);
	}
}

void GraphicsItem::traverseReverse(GraphicsItemPtr item, std::function<void (GraphicsItemPtr)> func)
{
	if (!item) return;

	for (auto ptr : item->children_) {
		traverse(ptr, func);
	}
	func(item);
}

} /* namespace tgl */
