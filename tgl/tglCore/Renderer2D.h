/*
 * Renderer2D.h
 */

#ifndef TGL_CORE_RENDERER2D_H_
#define TGL_CORE_RENDERER2D_H_

#include <string>
#include <memory>
#include <GL/gl.h>

class FTFont;

namespace tgl {

class GraphicsView;

class Renderer2D {
public:

	enum class Mode {
		Center,
		Corner,
	};

	enum class Align {
		Left,
		Center,
		Right,
	};

	enum class VAlign {
		Top,
		Bottom,
		Center,
		Baseline,
	};

	Renderer2D(GraphicsView* view);
	virtual ~Renderer2D();

	// Primitive shapes
	void drawPoint(int x, int y);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void drawRect(int x, int y, int width, int height);
	void drawEllipse(int x, int y, int width, int height);
	void drawCircle(int x, int y, int radius);
	void drawRing(int x, int y, int outer_radius, int inner_radius);

	void drawBezier(int x, int y, int cx1, int cy1, int cx2, int cy2, int x2, int y2);

	void setSmooth(bool on);
	void setRectMode(Mode mode);
	void setEllipseMode(Mode mode);

	void setFillColor(double r, double g, double b, double a = 1.0);
	void setNoFill();

	void setStrokeColor(double r, double g, double b, double a = 1.0);
	void setNoStroke();

	void setStrokeWeight(int w);
	void setPointSize(int s);

	void setCircleQuality(int q) { circleQuality_ = q; }
	int circleQuality() const { return circleQuality_; }

	// Text
	void setTextAlign(Align align, VAlign vAlign);
	void setTextSize(int size);
	void setTextColor(double r, double g, double b, double a = 1.0);
	int textSize() const;
	void loadFont(const std::string& path);
	void drawText(int x, int y, const std::string& str);

	// transforms
	void pushMatrix();
	void popMatrix();

	void translate(int x, int y);
	void rotate(double degree);
	void scale(double scale);
	void scale(double scaleX, double scaleY);

	int viewWidth() const;
	int viewHeight() const;

private:
	void setColor(double color[4]);
	GraphicsView* graphicsView_;

	double fillColor_[4];
	double strokeColor_[4];
	bool fill_;
	bool stroke_;

	int circleQuality_;

	Mode rectMode_;
	Mode ellipseMode_;

	// text
	Align align_;
	VAlign valign_;

	double textColor_[4];

	typedef std::unique_ptr<FTFont> FTFontPtr;
	FTFontPtr font_;
	std::string fontPath_;
	int textSize_;

	int strokeWeight_;
	int pointSize_;
};

} /* namespace tgl */

#endif /* TGL_CORE_RENDERER2D_H_ */
