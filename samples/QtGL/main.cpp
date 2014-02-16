/*
 * main.cpp
 */

#include <iostream>
#include <sstream>
#include <QApplication>
#include "tglCore/GraphicsView.h"
#include "tglHandle/TranslateHandle.h"
#include "tglGUI/ButtonPanel.h"
#include "tglDriver/QtGLGraphicsDriver.h"

using namespace std;

class Item : public tgl::GraphicsItem
{
public:
	Item() {
		color_ = {{1,0,0,1}};
		pressCount_ = 0;
		setName("Item");

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> rand(-5.0,5.0);

		p << rand(mt), rand(mt), rand(mt);
	}
	virtual ~Item() {}

protected:
	virtual void renderScene(tgl::Renderer3D* r) {
		double R[] = {1,0,0, 0,1,0, 0,0,1};

		r->setColor(color_[0], color_[1], color_[2]);

		switch (pressCount_) {
		case 0: r->drawSphere(p.data(), R, 1); break;
		case 1: r->drawCylinder(p.data(), R, 1.0, 0.5); break;
		case 2: r->drawCapsule(p.data(), R, 1.0, 0.5); break;
		case 3: r->drawCone(p.data(), R, 1.0, 0.5); break;
		default: pressCount_ = 0; break;
		}
	}

	// Hover event
	virtual void hoverEnterEvent(tgl::GraphicsItemHoverEvent* /*e*/) {
		color_ = {{1,1,0,1}};
	}

	virtual void hoverLeaveEvent(tgl::GraphicsItemHoverEvent* /*e*/) {
		color_ = {{1,0,0,1}};
	}

	// Mouse event
	virtual void mousePressEvent(tgl::GraphicsItemMouseEvent* e) {
		++pressCount_;
	}

	std::array<double, 4> color_;
	Eigen::Vector3d p;
	int pressCount_;
};

class View : public tgl::GraphicsView
{
public:
	View(tgl::GraphicsDriverPtr driver) : tgl::GraphicsView(std::move(driver)) {

		wire_ = false;

		// set TranslateHandle
		translateHandle_ = std::make_shared<tgl::handle::TranslateHandle>();
		translateHandle_->setName("TranslateHandle");
		addGraphicsItem(translateHandle_);

		// set ButtonPanel
		buttonPanel_ = tgl::gui::ButtonPanelPtr(new tgl::gui::ButtonPanel);
		buttonPanel_->addButton("Button1");
		buttonPanel_->addButton("Button2");

		buttonPanel_->button("Button2")->sigToggled().connect([this](bool toggled){
			this->button2Toggled(toggled);
		});

		addGraphicsItem(buttonPanel_);

		// add Item
		addGraphicsItem(tgl::GraphicsItemPtr(new Item));
		addGraphicsItem(tgl::GraphicsItemPtr(new Item));
		addGraphicsItem(tgl::GraphicsItemPtr(new Item));
		addGraphicsItem(tgl::GraphicsItemPtr(new Item));
	}

	virtual ~View() {}

protected:

	virtual void renderScene(tgl::Renderer3D* r) {
		Eigen::Vector3d p(Eigen::Vector3d::Zero());
		Eigen::Matrix3d R(Eigen::Matrix3d::Identity());
		double sides[] = {1.0, 1.0, 1.0};

		r->setColor(0, 0, 0);

		r->setLineWidth(1);
		r->drawGrid(10, 10);

		r->setLineWidth(2);
		glDisable(GL_DEPTH_TEST);
		r->drawAxis(p.data(), R.data(), 10);

		if (buttonPanel_->button("Button1")->isChecked()) {
			r->setColor(1.0, 0.4, 0.4);
		} else {
			r->setColor(0.4, 0.4, 1.0);
		}
		p = translateHandle_->se3()->position();
		R = translateHandle_->se3()->attitude();

		if (wire_) {
			r->setDrawMode(tgl::Renderer3D::DrawMode::Wire);
		} else {
			r->setDrawMode(tgl::Renderer3D::DrawMode::Solid);
		}

		r->drawBox(p.data(), R.data(), sides);
	}

	virtual void render2DScene(tgl::Renderer2D* r) {
		r->setFillColor(1,0,0);
		r->setStrokeColor(0.6,0.6,0.6);
		r->setStrokeWeight(5);
		r->drawCircle(50, 100, 10);

		r->setFillColor(0,1,0);
		r->setStrokeColor(1,1,1);
		r->setStrokeWeight(2);
		r->drawRing(50, 150, 20, 10);

		r->setFillColor(0, 0, 1, 0.4);
		r->setRectMode(tgl::Renderer2D::Mode::Center);
		r->drawRect(50, 200, 30, 30);
	}

	virtual void renderTextScene(tgl::TextRenderer* r) {
		Eigen::Vector3d p = translateHandle_->se3()->position();
		std::stringstream ss;
		ss << "(" << p(0) << ", " << p(1) << ", " << p(2) << ")";
		r->setTextColor(1,1,1);
		r->drawText(10, r->viewHeight() - 10, ss.str());
	}

	void button2Toggled(bool toggle) {
		cout << "Button2 toggled : " << (toggle ? "on" : "off") << endl;
		wire_ = toggle;
	}

	tgl::handle::TranslateHandlePtr translateHandle_;
	tgl::gui::ButtonPanelPtr buttonPanel_;
	bool wire_;
};

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	tgl::GraphicsDriverPtr driver(new tgl::QtGLGraphicsDriver);
	auto view = std::make_shared<View>(std::move(driver));
	view->setWindowTitle("View");
	view->initialize();
	view->execute();

	tgl::GraphicsDriverPtr driver2(new tgl::QtGLGraphicsDriver);
	auto view2 = std::make_shared<View>(std::move(driver2));
	view2->setWindowTitle("View2");
	view2->initialize();
	view2->execute();

	return app.exec();
}



