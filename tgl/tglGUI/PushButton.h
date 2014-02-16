/*
 * PushButton.h
 */

#ifndef TGL_GUI_PUSHBUTTON_H_
#define TGL_GUI_PUSHBUTTON_H_

#include <array>
#include "AbstractButton.h"

namespace tgl
{
namespace gui
{

class PushButton;
typedef std::shared_ptr<PushButton> PushButtonPtr;

class PushButton : public AbstractButton
{
public:
	PushButton();
	PushButton(const std::string& text);
	virtual ~PushButton();

	void setCheckedColor(double r, double g, double b, double a = 1.0);
	const std::array<double, 4> checkedColor() const { return checkedColor_; }

protected:

	virtual void render2DScene(tgl::Renderer2D* r);

	std::array<double, 4> checkedColor_;

};

}
}

#endif /* TGL_GUI_PUSHBUTTON_H_ */
