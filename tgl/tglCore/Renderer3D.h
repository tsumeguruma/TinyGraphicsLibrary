/*
 * Renderer3D.h
 */

#ifndef TGL_CORE_RENDERER3D_H_
#define TGL_CORE_RENDERER3D_H_

#include <array>
#include <GL/gl.h>

namespace tgl {

class GraphicsView;

class Renderer3D {
public:

	enum CoordinatePlane {
		XYPlane,
		YZPlane,
		XZPlane,
	};

	enum DrawMode {
		Solid,
		Wire
	};

	Renderer3D(GraphicsView* view);
	virtual ~Renderer3D();

    void setPointSize(double size);
    void setLineWidth(double width);

	void setCylinderQuality(int n);
	void setCircleQuality(int n);
	void setCapsuleQuality(int n);
	void setSphereQuality(int n);

	int cylinderQuality() const { return cylinderQuality_; }
	int circleQuality() const { return circleQuality_; }
	int capsuleQuality() const { return capsuleQuality_; }
	int sphereQuality() const { return sphereQuality_; }

	void setMaterial(const double diffuseColor[3], const double emissiveColor[3], const double specularColor[3], double alpha, double ambientIntensity = 0.3, double shininess = 5.0);
	void getMaterial(double diffuseColor[3], double emissiveColor[3], double specularColor[3], double& alpha, double& ambientIntensity, double& shininess);

	void setColor(double r, double g, double b, double a = 1.0);
	void setTextColor(double r, double g, double b);

	void getColor(double& r, double& g, double& b);
	void getColor(double& r, double& g, double& b, double& a);
	void getTextColor(double& r, double& g, double& b);

	void setDrawMode(DrawMode mode);
	DrawMode drawMode() const { return drawMode_; }

	void setCullFace(bool on);
	bool cullFace() const { return cullFace_; }

	// lighting
	void enableLighting();
	void disableLighting();

	// draw functions
	void drawPoint(const double pos[3]);
	void drawLine(const double pos1[3], const double pos2[3]);
	void drawBox(const double pos[3], const double R[9], const double sides[3]);
	void drawSphere(const double pos[3], const double R[9], double r);
	void drawCylinder(const double pos[3], const double R[9], double length, double radius, bool drawCap = true);
	void drawCapsule(const double pos[3], const double R[9], double length, double radius);
	void drawCone(const double pos[3], const double R[9], double length, double radius);
	void drawRing(const double pos[3], const double R[9], double length, double outer_radius, double inner_radius);
	void drawCircle(const double pos[3], const double R[9], double r);
	void drawRingCircle(const double pos[3], const double R[9], double outer_radius, double inner_radius);
	void drawRect(const double pos[3], const double R[9], double w, double h);
	void drawArrow(const double pos1[3], const double pos2[3], double r);
	void drawArrowCone(const double pos1[3], const double pos2[3], double r);
	void drawAxis(const double pos[3], const double R[9], double length);
	void drawGrid(double w, int div);

	void drawPoints(const double* p, int numpoints);
	void drawLines(const double* lines, int numlines);
	void drawLineStrip(const double* lines, int numlines);
	void drawLineLoop(const double* lines, int numlines);

	void drawPoint(double x, double y, double z);
	void drawLine(double x1, double y1, double z1, double x2, double y2, double z2);
	void drawRect(double x1, double y1, double x2, double y2);

	// transform functions
	void pushMatrix();
	void popMatrix();

	void translate(const double t[3]);
	void translate(double x, double y, double z);

	void rotate(const double R[9]);
	void rotateRpy(double r, double p, double y);
	void rotateX(double deg);
	void rotateY(double deg);
	void rotateZ(double deg);

	void transform(const double pos[3], const double R[9]);

	void scale(double scale);
	void scale(const double s[3]);
	void scale(double sx, double sy, double sz);

private:
	void setMaterial(double r, double g, double b, double alpha);

	void drawSolidBox(double sx, double sy, double sz);
	void drawWireBox(double sx, double sy, double sz);
	void drawSolidSphere(GLdouble radius, GLint slices, GLint stacks);
	void drawWireSphere(GLdouble radius, GLint slices, GLint stacks);
	void drawCapsule(float l, float r);
	void drawCylinder(double l, double r, double zoffset, bool drawCap = true);
	void drawSolidCone( GLdouble base, GLdouble height, GLint slices, GLint stacks );
	void drawWireCone( GLdouble base, GLdouble height, GLint slices, GLint stacks);
	void drawSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);
	void drawWireCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);

	static void calcCircleTable(std::vector<double>& sint, std::vector<double>& cost, int n);

	GraphicsView* graphicsView_;

	std::array<double, 4> color_;
	std::array<double, 3> textColor_;
	int sphereQuality_;
	int capsuleQuality_;
	int cylinderQuality_;
	int circleQuality_;
	double pointSize_;
	double lineWidth_;
	DrawMode drawMode_;
	bool cullFace_;

	std::vector<double> sint1_;
	std::vector<double> cost1_;
	std::vector<double> sint2_;
	std::vector<double> cost2_;

	struct CircleTable {
		std::vector<double> sint;
		std::vector<double> cost;
	};

	CircleTable sphereCircleTable1_;
	CircleTable sphereCircleTable2_;
	CircleTable capsuleCircleTable_;
	CircleTable cylinderCircleTable_;
	CircleTable circleCircleTable_;

};

} /* namespace tgl */

#endif /* TGL_CORE_RENDERER3D_H_ */
