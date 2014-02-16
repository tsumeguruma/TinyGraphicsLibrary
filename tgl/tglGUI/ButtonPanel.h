/*
 * ButtonPanel.h
 */

#ifndef TGL_GUI_BUTTONPANEL_H_
#define TGL_GUI_BUTTONPANEL_H_

#include <string>
#include <map>

#include "DockWidget.h"
#include "PushButton.h"

namespace tgl
{
namespace gui
{

class ButtonPanel;
typedef std::shared_ptr<ButtonPanel> ButtonPanelPtr;

class ButtonPanel : public DockWidget
{
public:
	ButtonPanel();
	ButtonPanel(const std::string& title);
	virtual ~ButtonPanel();

	virtual PushButtonPtr addButton(const std::string& name);
	PushButtonPtr button(const std::string& name);
	PushButtonPtr findButton(const std::string& name) const;

	const std::map<std::string, PushButtonPtr>& buttons() const { return buttons_; }

	virtual void removeButton(const std::string& name);

	virtual void clearButtons();

protected:
	std::map<std::string, PushButtonPtr> buttons_;

};

}
}

#endif /* TGL_GUI_BUTTONPANEL_H_ */
