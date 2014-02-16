/*
 * TranslateHandle.cpp
 */

#include "tglCore/GraphicsView.h"
#include "tglUtil/Intersection.h"
#include "tglUtil/EigenUtil.h"
#include "TranslateHandle.h"

#include <iostream>
using namespace std;

namespace tgl {

namespace handle
{
TranslateHandle::TranslateHandle() {
	se3_ = std::make_shared<SE3>();

	scale_ = 1.0;
	autoScale_ = true;

	xHandle_ = Tranbslate1DHandlePtr(new Translate1DHandle);
	yHandle_ = Tranbslate1DHandlePtr(new Translate1DHandle);
	zHandle_ = Tranbslate1DHandlePtr(new Translate1DHandle);

	xHandle_->setAxis(1,0,0);
	yHandle_->setAxis(0,1,0);
	zHandle_->setAxis(0,0,1);

	xHandle_->setColor(1,0,0);
	yHandle_->setColor(0,0,1);
	zHandle_->setColor(0,1,0);

	// set hover color
	xHandle_->setHoverColor(0.0,0.8,1);
	yHandle_->setHoverColor(0.0,0.8,1);
	zHandle_->setHoverColor(0.0,0.8,1);

	addChild(xHandle_);
	addChild(yHandle_);
	addChild(zHandle_);

	// set 2D Handle
	xyHandle_ = Tranbslate2DHandlePtr(new Translate2DHandle);
	yzHandle_ = Tranbslate2DHandlePtr(new Translate2DHandle);
	zxHandle_ = Tranbslate2DHandlePtr(new Translate2DHandle);

	xyHandle_->setPlaneType(Translate2DHandle::XYPlane);
	yzHandle_->setPlaneType(Translate2DHandle::YZPlane);
	zxHandle_->setPlaneType(Translate2DHandle::ZXPlane);

	xyHandle_->setColor(1,1,0);
	yzHandle_->setColor(1,1,0);
	zxHandle_->setColor(1,1,0);

	// set hover color
	xyHandle_->setHoverColor(0.0,0.8,1);
	yzHandle_->setHoverColor(0.0,0.8,1);
	zxHandle_->setHoverColor(0.0,0.8,1);

	addChild(xyHandle_);
	addChild(yzHandle_);
	addChild(zxHandle_);

	// set 3D handle
	xyzHandle_ = Tranbslate3DHandlePtr(new tgl::handle::Translate3DHandle);
	xyzHandle_->setColor(1,1,0);

	xyzHandle_->setHoverColor(0.0,0.8,1);

	addChild(xyzHandle_);

	// set signal slot
	xHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));
	yHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));
	zHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));

	xyHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));
	yzHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));
	zxHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));

	xyzHandle_->sigPositionChanged().connect(boost::bind(&TranslateHandle::slotPositionChanged, this, _1));

	// set se3
	xHandle_->set(se3_);
	yHandle_->set(se3_);
	zHandle_->set(se3_);

	xyHandle_->set(se3_);
	yzHandle_->set(se3_);
	zxHandle_->set(se3_);

	xyzHandle_->set(se3_);
}

TranslateHandle::~TranslateHandle()
{


}

void TranslateHandle::set(SE3Ptr se3) {
	se3_ = se3;
	xHandle_->set(se3);
	yHandle_->set(se3);
	zHandle_->set(se3);

	xyHandle_->set(se3);
	yzHandle_->set(se3);
	zxHandle_->set(se3);

	xyzHandle_->set(se3);
}

void TranslateHandle::setScale(double scale) {
	scale_ = scale;
	xHandle_->setScale(scale);
	yHandle_->setScale(scale);
	zHandle_->setScale(scale);

	xyHandle_->setScale(scale);
	yzHandle_->setScale(scale);
	zxHandle_->setScale(scale);

	xyzHandle_->setScale(scale);
}

void TranslateHandle::setAutoScale(bool on) {
	autoScale_ = on;
}

void TranslateHandle::renderScene(tgl::Renderer3D* /*r*/)
{
	// set auto scale
	if (autoScale_) {
		Eigen::Vector3d dist(se3_->position() - this->graphicsWindow()->camera()->position());
		double d = dist.norm();
		if (d < 0.0001) d = 0.0001;
//		double scale = scale_ * 0.1*d;
		double scale = scale_ * 0.16*d;

		double w = this->viewHeight() / 480.0;	// h : 480の時基準
		double windowScale = 1.0 / w;
		scale *= windowScale;

		xHandle_->setScale(scale);
		yHandle_->setScale(scale);
		zHandle_->setScale(scale);

		xyHandle_->setScale(scale);
		yzHandle_->setScale(scale);
		zxHandle_->setScale(scale);

		xyzHandle_->setScale(scale);
	}
}

// Translate1DHandle
Translate1DHandle::Translate1DHandle()
{
	se3_ = std::make_shared<SE3>();
	drawColor_ = color_.data();

	scale_ = 1.0;

	hoverd_ = false;
	dragged_ = false;
}

Translate1DHandle::~Translate1DHandle()
{

}

void Translate1DHandle::set(SE3Ptr se3)
{
	se3_ = se3;
}

void Translate1DHandle::setAxis(double x, double y, double z)
{
	axis_ << x, y, z;
	Eigen::Vector3d n(axis_.normalized());
	R_ = rotFromRpy(double(-M_PI_2*n(1)), double(M_PI_2*n(0)), double(M_PI_2*n(2)));
}

// Hover event
void Translate1DHandle::hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = hoverColor_.data();
	hoverd_ = true;
}

void Translate1DHandle::hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = color_.data();
	hoverd_ = false;
}

void Translate1DHandle::hoverMoveEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = hoverColor_.data();
}

void Translate1DHandle::renderPickingOverlayScene(tgl::Renderer3D* r)
{
	if (!se3_) return;

	double b = 0.2;
	double l = scale_ * b;
	double ra = l * 0.3;
	Eigen::Vector3d p;
	p = se3_->position() + (scale_ * (1.0 - b) * axis_);
	cp_ = p;

	r->setColor(drawColor_[0], drawColor_[1], drawColor_[2], drawColor_[3]);
	Eigen::Matrix3d tR = R_.transpose();
	r->drawCone(p.data(), tR.data(), l, ra);

}

void Translate1DHandle::renderOverlayScene(tgl::Renderer3D* r)
{
	if (!se3_) return;

	double b = 0.2;
	double l = scale_ * b;
	double ra = l * 0.3;
	Eigen::Vector3d p = se3_->position() + (scale_ * (1.0 - b) * axis_);
	cp_ = p;

	r->setColor(1,1,1);
	r->setLineWidth(2);
	r->drawLine(se3_->position().data(), p.data());

	double a = 1.0;
	if (hoverd_ || dragged_) {
		drawColor_ = hoverColor_.data();
		r->disableLighting();
	} else {
		drawColor_ = color_.data();
	}

	r->setColor(drawColor_[0], drawColor_[1], drawColor_[2], a);

	Eigen::Matrix3d tR = R_.transpose();
	r->drawCone(p.data(), tR.data(), l, ra);
	r->enableLighting();

}

void Translate1DHandle::mousePressEvent(tgl::GraphicsItemMouseEvent* e)
{
	dragged_ = true;
	if (!se3_) return;

	const int x = e->x();
	const int y = e->y();

	const Eigen::Vector3d& line1_p1 = se3_->position();
	const Eigen::Vector3d& line1_p2 = cp_;
	Eigen::Vector3d line2_p1 = graphicsWindow()->camera()->position();
	Eigen::Vector3d line2_p2 = graphicsWindow()->camera()->unProject2D(x, y);
	Eigen::Vector3d np1, np2;
	if (calcIntersectionLineAndLine(np1, np2, line1_p1, line1_p2, line2_p1, line2_p2)) {
		dcp_ = cp_ - np1;
	}
}

void Translate1DHandle::mouseMoveEvent(tgl::GraphicsItemMouseEvent* e)
{
	if (!se3_) return;
	const int x = e->x();
	const int y = e->y();

	const Eigen::Vector3d& line1_p1 = se3_->position();
	const Eigen::Vector3d& line1_p2 = cp_;
	Eigen::Vector3d line2_p1 = graphicsWindow()->camera()->position();
	Eigen::Vector3d line2_p2 = graphicsWindow()->camera()->unProject2D(x, y);
	Eigen::Vector3d np1, np2;
	if (calcIntersectionLineAndLine(np1, np2, line1_p1, line1_p2, line2_p1, line2_p2)) {
		Eigen::Vector3d dp;
		dp = np1 - cp_;

//			se3_->p += dp + dcp_;
		se3_->setPosition(Eigen::Vector3d(se3_->position() + dp + dcp_));

		// emit
		sigPositionChanged_(se3_->position());
	}

}

void Translate1DHandle::mouseReleaseEvent(tgl::GraphicsItemMouseEvent* /*e*/)
{
	drawColor_ = color_.data();
	dragged_ = false;
}

//
Translate2DHandle::Translate2DHandle()
{
	se3_ = std::make_shared<SE3>();
	drawColor_ = color_.data();
	scale_ = 1.0;

	axis1_ << 1,0,0;
	axis2_ << 0,1,0;

	hoverd_ = false;
	dragged_ = false;

	length_ = 0.2;
}

Translate2DHandle::~Translate2DHandle()
{

}

void Translate2DHandle::set(SE3Ptr se3)
{
	se3_ = se3;
}

void Translate2DHandle::setPlaneType(PlaneType type)
{
	planeType_ = type;
	if (planeType_ == XYPlane) {
		axis1_ << 1,0,0;
		axis2_ << 0,1,0;
	} else if (planeType_ == YZPlane) {
		axis1_ << 0,1,0;
		axis2_ << 0,0,1;
	} else if (planeType_ == ZXPlane) {
		axis1_ << 0,0,1;
		axis2_ << 1,0,0;
	}
}

// Hover event
void Translate2DHandle::hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = hoverColor_.data();
	hoverd_ = true;
}
void Translate2DHandle::hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = color_.data();
	hoverd_ = false;
}

void Translate2DHandle::hoverMoveEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = hoverColor_.data();
}

void Translate2DHandle::renderOverlayScene(tgl::Renderer3D* r)
{
	if (!se3_) return;

	Eigen::Vector3d p1, p2, p3;

	double length = scale_ * length_;

	p1 = se3_->position() + length*axis1_;
	p2 = se3_->position() + length*axis2_;
	p3 = se3_->position() + length*(axis1_ + axis2_);

	r->setLineWidth(2);
	r->setColor(drawColor_[0], drawColor_[1], drawColor_[2], drawColor_[3]);
	r->drawLine(p1.data(), p3.data());
	r->drawLine(p2.data(), p3.data());

	double a = 0;
	if (hoverd_ || dragged_) {
		a = 0.4;
		drawColor_ = hoverColor_.data();
	}
	r->setColor(drawColor_[0], drawColor_[1], drawColor_[2], a);

	glDisable(GL_CULL_FACE);
	glBegin(GL_POLYGON); // ポリゴンの描画
	glVertex3dv(se3_->position().data());
	glVertex3dv(p1.data());
	glVertex3dv(p3.data());
	glVertex3dv(p2.data());
	glEnd(); // ポリゴンの描画終了
}

void Translate2DHandle::mousePressEvent(tgl::GraphicsItemMouseEvent* e)
{
	dragged_ = true;
	if (!se3_) return;

	const int x = e->x();
	const int y = e->y();

	double length = scale_ * length_;

	const Eigen::Vector3d& p0 = se3_->position();
	Eigen::Vector3d p1, p2, p3;
	p1 = se3_->position() + length*axis1_;
	p2 = se3_->position() + length*axis2_;
	p3 = se3_->position() + length*(axis1_ + axis2_);
//		p3 = p1 + p2;

	Eigen::Vector3d cp(graphicsWindow()->camera()->position());
	Eigen::Vector3d wp(graphicsWindow()->camera()->unProject2D(x, y));

	preHp_ = calcIntersectionLineAndPlane(cp, wp, p1, p2, p3);
	preHp_ -= p0;
}

void Translate2DHandle::mouseMoveEvent(tgl::GraphicsItemMouseEvent* e)
{
	if (!se3_) return;

	const int x = e->x();
	const int y = e->y();

	double length = scale_ * length_;

	Eigen::Vector3d p1, p2, p3;
	p1 = se3_->position() + length*axis1_;
	p2 = se3_->position() + length*axis2_;
	p3 = se3_->position() + length*(axis1_ + axis2_);

	Eigen::Vector3d cp(graphicsWindow()->camera()->position());
	Eigen::Vector3d wp(graphicsWindow()->camera()->unProject2D(x, y));

	Eigen::Vector3d hp;
	hp = calcIntersectionLineAndPlane(cp, wp, p1, p2, p3);

	Eigen::Vector3d dhp;
	dhp = hp - preHp_;

	se3_->setPosition(dhp);

	// emit
	sigPositionChanged_(se3_->position());
}

void Translate2DHandle::mouseReleaseEvent(tgl::GraphicsItemMouseEvent* /*e*/)
{
	dragged_ = false;
}

// Translate3DHandle
Translate3DHandle::Translate3DHandle()
{
	se3_ = std::make_shared<SE3>();
	drawColor_ = color_.data();
	scale_ = 1.0;

	hoverd_ = false;
	dragged_ = false;

	length_ = 0.15;

	axis1_ << 1,0,0;
	axis2_ << 0,-1,0;

	cR_.setIdentity();
}

Translate3DHandle::~Translate3DHandle()
{

}

void Translate3DHandle::set(SE3Ptr se3)
{
	se3_ = se3;
}

// Hover event
void Translate3DHandle::hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = hoverColor_.data();
	hoverd_ = true;
}
void Translate3DHandle::hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = color_.data();
	hoverd_ = false;
}

void Translate3DHandle::hoverMoveEvent(tgl::GraphicsItemHoverEvent* /*e*/)
{
	drawColor_ = hoverColor_.data();
}

void Translate3DHandle::renderOverlayScene(tgl::Renderer3D* r)
{
	if (!se3_) return;

	double length = scale_ * length_;

	const double R0[] = {1,0,0, 0,1,0, 0,0,1};
	const double sides[] = {length, length, length};
	double a = 0;
	if (hoverd_ || dragged_) {
		drawColor_ = hoverColor_.data();
	} else {
		drawColor_ = color_.data();
	}

	a = drawColor_[3];
	r->setColor(drawColor_[0], drawColor_[1], drawColor_[2], a);
	r->drawBox(se3_->position().data(), R0, sides);
}


void Translate3DHandle::mousePressEvent(tgl::GraphicsItemMouseEvent* e)
{
	dragged_ = true;
	if (!se3_) return;

#if 0	// bug
	const int x = e->x();
	const int y = e->y();

	Eigen::Matrix3d cR(graphicsWindow()->camera()->inverseRotation());

	Eigen::Vector3d axis1, axis2;
	axis1 = cR * axis1_;
	axis2 = cR * axis2_;

	double length = scale_ * length_;

	Eigen::Vector3d p1, p2, p3;
	p1 = se3_->position() + length*axis1;
	p2 = se3_->position() + length*axis2;
	p3 = se3_->position() + length*(axis1 + axis2);

	Eigen::Vector3d cp(graphicsWindow()->camera()->position());
	Eigen::Vector3d wp(graphicsWindow()->camera()->unProject2D(x, y));

	preHp_ = calcIntersectionLineAndPlane(cp, wp, p1, p2, p3);
	dhp_ = preHp_ - se3_->position();
#endif
}

void Translate3DHandle::mouseMoveEvent(tgl::GraphicsItemMouseEvent* e)
{
	dragged_ = true;
	if (!se3_) return;

#if 0	// bug

	const int x = e->x();
	const int y = e->y();

	Eigen::Matrix3d cR(graphicsWindow()->camera()->inverseRotation());
//	Eigen::Matrix3d cR(graphicsWindow()->camera()->rotation());

	Eigen::Vector3d axis1, axis2;
	axis1 = cR * axis1_;
	axis2 = cR * axis2_;

	double length = scale_ * length_;

	Eigen::Vector3d p1, p2, p3;
	p1 = se3_->position() + length*axis1;
	p2 = se3_->position() + length*axis2;
	p3 = se3_->position() + length*(axis1 + axis2);

	Eigen::Vector3d cp(graphicsWindow()->camera()->position());
	Eigen::Vector3d wp(graphicsWindow()->camera()->unProject2D(x, y));

	Eigen::Vector3d hp;
	hp = calcIntersectionLineAndPlane(cp, wp, p1, p2, p3);

	se3_->setPosition(Eigen::Vector3d(hp - dhp_));

	// emit
	sigPositionChanged_(se3_->position());
#endif
}

void Translate3DHandle::mouseReleaseEvent(tgl::GraphicsItemMouseEvent* /*e*/)
{
	dragged_ = false;
}

}
} /* namespace tgl */
