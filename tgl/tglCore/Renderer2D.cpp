/*
 * Renderer2D.cpp
 */

#include <boost/filesystem.hpp>
#include <FTGL/ftgl.h>
#include "GraphicsView.h"
#include "Renderer2D.h"

#include <iostream>
using namespace std;

namespace tgl {

Renderer2D::Renderer2D(GraphicsView* view) : graphicsView_(view) {
	rectMode_ = Mode::Center;

	fillColor_[0] = 1.0;
	fillColor_[1] = 1.0;
	fillColor_[2] = 1.0;
	fillColor_[3] = 1.0;

	strokeColor_[0] = 0;
	strokeColor_[1] = 0;
	strokeColor_[2] = 0;
	strokeColor_[3] = 1;

	textColor_[0] = 1;
	textColor_[1] = 1;
	textColor_[2] = 1;
	textColor_[3] = 1;

	circleQuality_ = 40;

	align_ = Align::Left;
	valign_ = VAlign::Bottom;
	textSize_ = 16;
	loadFont(defaultFontPath);
}

Renderer2D::~Renderer2D() {

}

// Primitive shapes
void Renderer2D::drawPoint(int x, int y)
{
	setColor(fillColor_);

	glBegin(GL_POINTS);
	glVertex3i(x, y, 0);
	glEnd();
}

void Renderer2D::drawLine(int x1, int y1, int x2, int y2)
{
	setColor(strokeColor_);

	glBegin(GL_LINES);
	glVertex3i(x1, y1, 0);
	glVertex3i(x2, y2, 0);
	glEnd();
}

void Renderer2D::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if (fill_) {
		setColor(fillColor_);

		glBegin(GL_POLYGON);
		glVertex3i(x1, y1, 0);
		glVertex3i(x2, y2, 0);
		glVertex3i(x3, y3, 0);
		glEnd();
	}

	if (stroke_) {
		setColor(strokeColor_);

		glBegin(GL_LINE_LOOP);
		glVertex3i(x1, y1, 0);
		glVertex3i(x2, y2, 0);
		glVertex3i(x3, y3, 0);
		glEnd();
	}
}

void Renderer2D::drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	if (fill_) {
		setColor(fillColor_);

		glBegin(GL_POLYGON);
		glVertex3i(x1, y1, 0);
		glVertex3i(x2, y2, 0);
		glVertex3i(x3, y3, 0);
		glVertex3i(x4, y4, 0);
		glEnd();
	}

	if (stroke_) {
		setColor(strokeColor_);

		glBegin(GL_LINE_LOOP);
		glVertex3i(x1, y1, 0);
		glVertex3i(x2, y2, 0);
		glVertex3i(x3, y3, 0);
		glVertex3i(x4, y4, 0);
		glEnd();
	}
}

void Renderer2D::drawRect(int x, int y, int w, int h)
{
	if (rectMode_ == Mode::Center) {
		int hw = w / 2;
		int hh = h / 2;
		drawQuad(x-hw, y-hh, x+hw, y-hh, x+hw, y+hh, x-hw, y+hh);
	} else if (rectMode_ == Mode::Corner) {
		drawQuad(x, y, x+w, y, x+w, y+h, x, y+h);
	} else {
		drawQuad(x, y, x+w, y, x+w, y+h, x, y+h);
	}

//	drawQuad(x, y, x+w, y, x+w, y+h, x, y+h);
}

void Renderer2D::drawEllipse(int x, int y, int width, int height)
{
	const double r = 1.0;

	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(width, height, 0);

	if (fill_) {
		setColor(fillColor_);

		glBegin(GL_POLYGON); // ポリゴンの描画
		// 円を描画
		for (int i = 0; i < circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double x = r * cos(2.0 * M_PI * rate);
			double y = r * sin(2.0 * M_PI * rate);
			glVertex3d(x, y, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了
	}

	if (stroke_) {
		setColor(strokeColor_);

		glBegin(GL_LINE_LOOP);
		// 円を描画
		for (int i = 0; i < circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double x = r * cos(2.0 * M_PI * rate);
			double y = r * sin(2.0 * M_PI * rate);
			glVertex3d(x, y, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了
	}

	glPopMatrix();
}

void Renderer2D::drawCircle(int x, int y, int radius)
{
	const double r = radius;

	glPushMatrix();
	glTranslatef(x, y, 0);

	if (fill_) {
		setColor(fillColor_);

		glBegin(GL_POLYGON); // ポリゴンの描画
		// 円を描画
		for (int i = 0; i < circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double x = r * cos(2.0 * M_PI * rate);
			double y = r * sin(2.0 * M_PI * rate);
			glVertex3d(x, y, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了
	}

	if (stroke_) {
		setColor(strokeColor_);

		glBegin(GL_LINE_LOOP);
		// 円を描画
		for (int i = 0; i < circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double x = r * cos(2.0 * M_PI * rate);
			double y = r * sin(2.0 * M_PI * rate);
			glVertex3d(x, y, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了
	}

	glPopMatrix();
}

void Renderer2D::drawRing(int x, int y, int outer_radius, int inner_radius)
{
	glPushMatrix();
	glTranslatef(x, y, 0);

	if (fill_) {
		setColor(fillColor_);

		glBegin(GL_QUAD_STRIP); // ポリゴンの描画
		// 円を描画
		for (int i = 0; i <= circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double c = cos(2.0 * M_PI * rate);
			double s = sin(2.0 * M_PI * rate);

			double ox = outer_radius * c;
			double oy = outer_radius * s;

			double ix = inner_radius * c;
			double iy = inner_radius * s;

			glVertex3d(ix, iy, 0.0); // 頂点座標を指定
			glVertex3d(ox, oy, 0.0); // 頂点座標を指定
//			glVertex3d(ix, iy, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了
	}

	if (stroke_) {
		setColor(strokeColor_);

		glBegin(GL_LINE_LOOP);
		// 円を描画
		for (int i = 0; i < circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double c = cos(2.0 * M_PI * rate);
			double s = sin(2.0 * M_PI * rate);

			double ix = inner_radius * c;
			double iy = inner_radius * s;

			glVertex3d(ix, iy, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了

		glBegin(GL_LINE_LOOP);
		// 円を描画
		for (int i = 0; i < circleQuality_; i++) {
			// 座標を計算
			double rate = (double)i / circleQuality_;
			double c = cos(2.0 * M_PI * rate);
			double s = sin(2.0 * M_PI * rate);

			double ox = outer_radius * c;
			double oy = outer_radius * s;

			glVertex3d(ox, oy, 0.0); // 頂点座標を指定
		}
		glEnd(); // ポリゴンの描画終了
	}

	glPopMatrix();
}

// Curves
void Renderer2D::drawBezier(int x, int y, int cx1, int cy1, int cx2, int cy2, int x2, int y2)
{

}

// Attributes
void Renderer2D::setSmooth(bool on)
{
	if (on) {
		glEnable(GL_LINE_SMOOTH);	// アンチエイリアス
		glEnable(GL_POINT_SMOOTH);
		//	glEnable(GL_POLYGON_SMOOTH);
	} else {
		glDisable(GL_LINE_SMOOTH);	// アンチエイリアス
		glDisable(GL_POINT_SMOOTH);
	//	glDisable(GL_POLYGON_SMOOTH);
	}
}

void Renderer2D::setRectMode(Mode mode)
{
	rectMode_ = mode;
}

void Renderer2D::setEllipseMode(Mode mode)
{
	ellipseMode_ = mode;
}

void Renderer2D::setFillColor(double r, double g, double b, double a)
{
	fillColor_[0] = r;
	fillColor_[1] = g;
	fillColor_[2] = b;
	fillColor_[3] = a;
	fill_ = true;

//	if (fillColor_[3] < 0.99) glDisable(GL_DEPTH_TEST);
//	else glEnable(GL_DEPTH_TEST);
}

void Renderer2D::setNoFill()
{
	fill_ = false;
}

void Renderer2D::setStrokeColor(double r, double g, double b, double a)
{
	strokeColor_[0] = r;
	strokeColor_[1] = g;
	strokeColor_[2] = b;
	strokeColor_[3] = a;
	stroke_ = true;
}

void Renderer2D::setNoStroke()
{
	stroke_ = false;
}

void Renderer2D::setStrokeWeight(int w)
{
	glLineWidth(w);
}

void Renderer2D::setPointSize(int s)
{
	glPointSize(s);
}

void Renderer2D::setTextAlign(Align align, VAlign vAlign)
{
	align_ = align;
	valign_ = vAlign;
}

void Renderer2D::setTextSize(int size)
{
	textSize_ = size;
	if (!font_) return;
	font_->FaceSize(size);
}

int Renderer2D::textSize() const
{
	return textSize_;
}

void Renderer2D::loadFont(const std::string& path)
{
	boost::filesystem::path fpath(path);
	if (boost::filesystem::exists(fpath)) {
		fontPath_ = path;
		font_ = FTFontPtr(new FTExtrudeFont(fontPath_.c_str()));
		font_->FaceSize(textSize_);
	}
}

void Renderer2D::setTextColor(double r, double g, double b, double a)
{
	textColor_[0] = r;
	textColor_[1] = g;
	textColor_[2] = b;
	textColor_[3] = a;
}

void Renderer2D::drawText(int x, int y, const std::string& str)
{
	if (!font_) {
		std::cerr << "error : Renderer2D::drawText font is null" << std::endl;
		return;
	}

	// text align の設定
	FTBBox bb = font_->BBox(str.c_str());
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

	setColor(textColor_);

	glEnable(GL_POLYGON_SMOOTH);

	glPushMatrix();
	glTranslatef(x + sx, y + sy, 0);
	glScalef(1,-1,1);

	font_->Render(str.c_str());

	glPopMatrix();

	glDisable(GL_POLYGON_SMOOTH);
}

void Renderer2D::pushMatrix()
{
	glPushMatrix();
}
void Renderer2D::popMatrix()
{
	glPopMatrix();
}

void Renderer2D::translate(int x, int y)
{
	glTranslated(x, y, 0);
}

void Renderer2D::rotate(double degree)
{
	glRotated(degree, 0,0,1);
}

void Renderer2D::scale(double scale)
{
	glScaled(scale, scale, 1.0);
}

void Renderer2D::scale(double scaleX, double scaleY)
{
	glScaled(scaleX, scaleY, 1.0);
}

void Renderer2D::setColor(double color[4])
{
	glColor4dv(color);
}

int Renderer2D::viewWidth() const
{
	return graphicsView_ ? graphicsView_->width() : 0;
}

int Renderer2D::viewHeight() const
{
	return graphicsView_ ? graphicsView_->height() : 0;
}


} /* namespace tgl */
