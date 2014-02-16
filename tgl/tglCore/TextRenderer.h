/*
 * TextRenderer.h
 */

#ifndef TGL_CORE_TEXTRENDERER_H_
#define TGL_CORE_TEXTRENDERER_H_

#include <string>
#include <memory>
#include <GL/gl.h>
#include "Common.h"

class FTFont;

namespace tgl {

class GraphicsView;

class TextRenderer {
public:

	TextRenderer(GraphicsView* view);
	virtual ~TextRenderer();

	void setTextColor(double r, double b, double g, double a = 1.0);
	void setTextAlign(Align align, VAlign valign);
	void setTextSize(int size);
	void loadFont(const std::string& path);

	int textSize() const { return textSize_; }

	void drawText(int x, int y, const std::string& text);
	void drawText(double x, double y, double z, const std::string& text);

	int viewWidth() const;
	int viewHeight() const;

private:
	GraphicsView* graphicsView_;

	double textColor_[4];

	Align align_;
	VAlign valign_;

	typedef std::unique_ptr<FTFont> FTFontPtr;
	FTFontPtr font_;
	std::string fontPath_;
	int textSize_;
};

} /* namespace tgl */

#endif /* TGL_CORE_TEXTRENDERER_H_ */
