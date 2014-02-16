/*
 * ButtonPanel.cpp
 */

#include "ButtonPanel.h"

namespace tgl
{
namespace gui
{

ButtonPanel::ButtonPanel() : DockWidget()
{
//	setSize(120, 380);
	setWindowTitle("ButtonPanel");
}

ButtonPanel::ButtonPanel(const std::string& title) : DockWidget(title)
{
//	setSize(120, 380);
	setWindowTitle(title);
}

ButtonPanel::~ButtonPanel()
{

}

PushButtonPtr ButtonPanel::addButton(const std::string& name)
{
	PushButtonPtr ptr = findButton(name);
	if (!ptr) {	// not find
		PushButtonPtr button(new PushButton(name));
		buttons_[name] = button;

		button->setCheckable(true);

		addWidget(button);
		ptr = button;
	}
	return ptr;
}

PushButtonPtr ButtonPanel::button(const std::string& name)
{
	return addButton(name);
}

PushButtonPtr ButtonPanel::findButton(const std::string& name) const
{
	auto itr = buttons_.find(name);
	return itr != buttons_.end() ? itr->second : nullptr;
}

void ButtonPanel::removeButton(const std::string& name)
{
	auto itr = buttons_.find(name);
	if (itr != buttons_.end()) {
		removeWidget(itr->second);
		buttons_.erase(itr);
	}
}

void ButtonPanel::clearButtons()
{
	for (auto itr = buttons_.begin(); itr != buttons_.end(); ++itr) {
		removeWidget(itr->second);
	}

	buttons_.clear();
}

}
}
