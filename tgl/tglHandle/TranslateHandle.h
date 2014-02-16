/*
 * TranslateHandle.h
 */

#ifndef TGL_HANDLE_TRANSLATEHANDLE_H_
#define TGL_HANDLE_TRANSLATEHANDLE_H_

#include <memory>
#include <array>
#include <boost/signals2.hpp>
#include "tglCore/GraphicsItem.h"
#include "tglUtil/SE3.h"

namespace tgl {

namespace handle
{

class Translate1DHandle;
class Translate2DHandle;
class Translate3DHandle;
class TranslateHandle;
typedef std::shared_ptr<Translate1DHandle> Translate1DHandlePtr;
typedef std::shared_ptr<Translate2DHandle> Translate2DHandlePtr;
typedef std::shared_ptr<Translate3DHandle> Translate3DHandlePtr;
typedef std::shared_ptr<TranslateHandle> TranslateHandlePtr;

class TranslateHandle : public GraphicsItem {
public:
	TranslateHandle();
	virtual ~TranslateHandle();

	void set(SE3Ptr se3);
	void setScale(double scale);
	void setAutoScale(bool on);

	const SE3Ptr se3() const { return se3_; }

	boost::signals2::signal<void ()>& sigStateChanged() { return sigStateChanged_; }

protected:

	virtual void renderScene(tgl::Renderer3D* r);

	typedef std::shared_ptr<Translate1DHandle> Tranbslate1DHandlePtr;
	Tranbslate1DHandlePtr xHandle_;
	Tranbslate1DHandlePtr yHandle_;
	Tranbslate1DHandlePtr zHandle_;

	typedef std::shared_ptr<Translate2DHandle> Tranbslate2DHandlePtr;
	Tranbslate2DHandlePtr xyHandle_;
	Tranbslate2DHandlePtr yzHandle_;
	Tranbslate2DHandlePtr zxHandle_;

	typedef std::shared_ptr<Translate3DHandle> Tranbslate3DHandlePtr;
	Tranbslate3DHandlePtr xyzHandle_;

	SE3Ptr se3_;
	double scale_;
	bool autoScale_;

	boost::signals2::signal<void ()> sigStateChanged_;

	void slotPositionChanged(Eigen::Vector3d) {
		// emit
		sigStateChanged_();
	}

	void slotRotationChanged(Eigen::Matrix3d) {
		// emit
		sigStateChanged_();
	}
};

class Translate1DHandle : public GraphicsItem {
public:
	Translate1DHandle();
	virtual ~Translate1DHandle();

	void set(SE3Ptr se3);
	void setScale(double scale) { scale_ = scale; }

	void setAxis(double x, double y, double z);

	const SE3Ptr se3() const { return se3_; }

	void setColor(double r, double g, double b, double a = 1.0) {
		color_ = {{r, g, b, a}};
	}

	void setPickingColor(double r, double g, double b, double a = 1.0) {
		pickingColor_ = {{r, g, b, a}};
	}

	void setDragingColor(double r, double g, double b, double a = 1.0) {
		draggingColor_ = {{r, g, b, a}};
	}

	void setHoverColor(double r, double g, double b, double a = 1.0) {
		hoverColor_ = {{r, g, b, a}};
	}

	boost::signals2::signal<void (Eigen::Vector3d)>& sigPositionChanged() { return sigPositionChanged_; }

protected:

	// Hover event
	virtual void hoverEnterEvent(tgl::GraphicsItemHoverEvent* e);
	virtual void hoverLeaveEvent(tgl::GraphicsItemHoverEvent* e);
	virtual void hoverMoveEvent(tgl::GraphicsItemHoverEvent* e);

	virtual void renderPickingOverlayScene(tgl::Renderer3D* r);
	virtual void renderOverlayScene(tgl::Renderer3D* r);

	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseMoveEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseReleaseEvent(tgl::GraphicsItemMouseEvent* e);


	SE3Ptr se3_;
	double scale_;
	bool autoScale_;

	double* drawColor_;
	std::array<double, 4> color_;
	std::array<double, 4> hoverColor_;
	std::array<double, 4> pickingColor_;
	std::array<double, 4> draggingColor_;

	Eigen::Vector3d axis_;

	bool hoverd_;
	bool dragged_;

	Eigen::Vector3d cp_;
	Eigen::Matrix3d R_;

	Eigen::Vector3d dcp_;

	boost::signals2::signal<void (Eigen::Vector3d)> sigPositionChanged_;
};

class Translate2DHandle : public GraphicsItem
{
public:

	enum PlaneType {
		XYPlane,
		YZPlane,
		ZXPlane,
	};

	Translate2DHandle();
	virtual ~Translate2DHandle();

	void set(SE3Ptr se3);
	void setPlaneType(PlaneType type);
	void setScale(double scale) { scale_ = scale; }

	const SE3Ptr se3() const { return se3_; }

	void setColor(double r, double g, double b, double a = 1.0) {
		color_ = {{r, g, b, a}};
	}

	void setPickingColor(double r, double g, double b, double a = 1.0) {
		pickingColor_ = {{r, g, b, a}};
	}

	void setDragingColor(double r, double g, double b, double a = 1.0) {
		draggingColor_ = {{r, g, b, a}};
	}

	void setHoverColor(double r, double g, double b, double a = 1.0) {
		hoverColor_ = {{r, g, b, a}};
	}

	boost::signals2::signal<void (Eigen::Vector3d)>& sigPositionChanged() { return sigPositionChanged_; }

protected:

	// Hover event
	virtual void hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/);
	virtual void hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/);
	virtual void hoverMoveEvent(tgl::GraphicsItemHoverEvent* /*e*/);

	virtual void renderOverlayScene(tgl::Renderer3D* r) ;

	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseMoveEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseReleaseEvent(tgl::GraphicsItemMouseEvent* /*e*/);

	SE3Ptr se3_;

	PlaneType planeType_;

	double scale_;

	double* drawColor_;
	std::array<double, 4> color_;
	std::array<double, 4> hoverColor_;
	std::array<double, 4> pickingColor_;
	std::array<double, 4> draggingColor_;

	bool hoverd_;
	bool dragged_;

	double length_;

	Eigen::Vector3d axis1_;
	Eigen::Vector3d axis2_;

	Eigen::Vector3d preHp_;

	boost::signals2::signal<void (Eigen::Vector3d)> sigPositionChanged_;

};

class Translate3DHandle : public GraphicsItem
{
public:
	Translate3DHandle();
	virtual ~Translate3DHandle();

	void set(SE3Ptr se3);
	void setScale(double scale) { scale_ = scale; }

	const SE3Ptr se3() const { return se3_; }

	void setColor(double r, double g, double b, double a = 1.0) {
		color_ = {{r, g, b, a}};
	}

	void setPickingColor(double r, double g, double b, double a = 1.0) {
		pickingColor_ = {{r, g, b, a}};
	}

	void setDragingColor(double r, double g, double b, double a = 1.0) {
		draggingColor_ = {{r, g, b, a}};
	}

	void setHoverColor(double r, double g, double b, double a = 1.0) {
		hoverColor_ = {{r, g, b, a}};
	}

	boost::signals2::signal<void (Eigen::Vector3d)>& sigPositionChanged() { return sigPositionChanged_; }

protected:

	// Hover event
	virtual void hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/);
	virtual void hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/);
	virtual void hoverMoveEvent(tgl::GraphicsItemHoverEvent* /*e*/);

	virtual void renderOverlayScene(tgl::Renderer3D* r);

	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseMoveEvent(tgl::GraphicsItemMouseEvent* e);
	virtual void mouseReleaseEvent(tgl::GraphicsItemMouseEvent* /*e*/);

	SE3Ptr se3_;

	double scale_;

	double* drawColor_;
	std::array<double, 4> color_;
	std::array<double, 4> hoverColor_;
	std::array<double, 4> pickingColor_;
	std::array<double, 4> draggingColor_;

	bool hoverd_;
	bool dragged_;

	double length_;

	Eigen::Vector3d axis1_;
	Eigen::Vector3d axis2_;

	Eigen::Vector3d preHp_;
	Eigen::Matrix3d cR_;
	Eigen::Vector3d dhp_;

	boost::signals2::signal<void (Eigen::Vector3d)> sigPositionChanged_;

};

} /* namespace handle */
} /* namespace tgl */

#endif /* TGL_HANDLE_TRANSLATEHANDLE_H_ */
