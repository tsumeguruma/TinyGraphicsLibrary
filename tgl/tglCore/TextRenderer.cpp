/*
 * TextRenderer.cpp
 */

#include <boost/filesystem.hpp>
#include <FTGL/ftgl.h>
#include "GraphicsView.h"
#include "TextRenderer.h"

#include <iostream>
using namespace std;

namespace tgl {

TextRenderer::TextRenderer(GraphicsView* view)
	: graphicsView_(view)
{
	textColor_[0] = 0;
	textColor_[1] = 0;
	textColor_[2] = 0;
	textColor_[3] = 1;
	textSize_ = 16;

	align_ = Align::Left;
	valign_ = VAlign::Bottom;
	loadFont(defaultFontPath);
}

TextRenderer::~TextRenderer() {

}


void TextRenderer::setTextColor(double r, double b, double g, double a)
{
	textColor_[0] = r;
	textColor_[1] = g;
	textColor_[2] = b;
	textColor_[3] = a;
}

void TextRenderer::setTextAlign(Align align, VAlign valign)
{
	align_ = align;
	valign_ = valign;
}

void TextRenderer::setTextSize(int size)
{
	textSize_ = size;
	if (!font_) return;
	font_->FaceSize(size);
}

void TextRenderer::loadFont(const std::string& path)
{
	boost::filesystem::path fpath(path);
	if (boost::filesystem::exists(fpath)) {
		fontPath_ = path;
		font_ = std::move(FTFontPtr(new FTExtrudeFont(fontPath_.c_str())));
		font_->FaceSize(textSize_);
	}
}

void TextRenderer::drawText(int x, int y, const std::string& text)
{
	if (!font_) {
		std::cerr << "error : Renderer2D::drawText font is null" << std::endl;
		return;
	}

	// text align の設定
	FTBBox bb = font_->BBox(text.c_str());
	const int w = abs(bb.Lower().X() - bb.Upper().X());
	const int h = abs(bb.Lower().Y() - bb.Upper().Y());
	int sx, sy;

	switch (align_) {
	case Align::Left: sx = 0; break;
	case Align::Center: sx = w/2; break;
	case Align::Right: sx = w; break;
	default: sx = 0; break;
	}

	switch (valign_) {
	case VAlign::Top: sy = h; break;
	case VAlign::Bottom: sy = 0; break;
	case VAlign::Center: sy = h/2; break;
	default: sy = 0; break;
	}

	glColor4dv(textColor_);

	glEnable(GL_POLYGON_SMOOTH);

	glPushMatrix();
	glTranslatef(x + sx, y + sy, 0);
	glScalef(1,-1,1);

	font_->Render(text.c_str());

	glPopMatrix();

	glDisable(GL_POLYGON_SMOOTH);
}

void TextRenderer::drawText(double x, double y, double z, const std::string& text)
{
	if (graphicsView_) {
		Eigen::Vector3d pos = graphicsView_->camera()->project2D(x, y, z);
		drawText(pos(0), pos(1), text);
	}
}

int TextRenderer::viewWidth() const
{
	return graphicsView_ ? graphicsView_->width() : 0;
}

int TextRenderer::viewHeight() const
{
	return graphicsView_ ? graphicsView_->height() : 0;
}

} /* namespace tgl */
