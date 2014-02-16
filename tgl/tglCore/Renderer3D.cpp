/*
 * Renderer3D.cpp
 */

#include <cmath>
#include "GraphicsView.h"
#include "Renderer3D.h"

#include <iostream>

using namespace std;

namespace tgl {

Renderer3D::Renderer3D(GraphicsView* view) {
	graphicsView_ = view;

	sphereQuality_ = 16;
	capsuleQuality_ = 24;
	cylinderQuality_ = 24;
	circleQuality_ = 24;

	pointSize_ = 10;
	lineWidth_ = 1;

	drawMode_ = Solid;

	cullFace_ = true;

	color_ = {{1.0, 1.0, 1.0, 1.0}};
	textColor_ = {{1.0, 1.0, 1.0}};
}

Renderer3D::~Renderer3D() {

}

void Renderer3D::setPointSize(double size)
{
	pointSize_ = size;
	glPointSize(pointSize_);
}

void Renderer3D::setLineWidth(double width)
{
	lineWidth_ = width;
	glLineWidth(lineWidth_);
}

void Renderer3D::setCylinderQuality(int n)
{
	capsuleQuality_ = n;
}

void Renderer3D::setCircleQuality(int n)
{
	circleQuality_ = n;
}

void Renderer3D::setCapsuleQuality(int n)
{
	capsuleQuality_ = n;
}

void Renderer3D::setSphereQuality(int n)
{
	sphereQuality_ = n;
}

void Renderer3D::setCullFace(bool on)
{
	cullFace_ = on;
}

void Renderer3D::setColor(double r, double g, double b, double a)
{
	color_ = {{r, g, b, a}};
	glColor4d(color_[0], color_[1], color_[2], color_[3]);
	setMaterial(color_[0], color_[1], color_[2], color_[3]);

	if (color_[3] < 0.99) {
		glDisable(GL_DEPTH_TEST);
	} else {
		glEnable(GL_DEPTH_TEST);
	}
}

void Renderer3D::setTextColor(double r, double g, double b)
{
	textColor_ = {{r, g, b}};
}

void Renderer3D::setDrawMode(DrawMode mode)
{
	drawMode_ = mode;
}

void Renderer3D::setMaterial(const double diffuseColor[3], const double emissiveColor[3], const double specularColor[3], double alpha, double ambientIntensity, double shininess)
{
	if (!diffuseColor || !emissiveColor || !specularColor) {
		cerr << "error Renderer3D::setMaterial : diffuseColor is null" << endl;
		return;
	}

	GLfloat light_ambient[4],light_diffuse[4],light_specular[4], light_emission[4];

	light_ambient[0] = ambientIntensity*diffuseColor[0];
	light_ambient[1] = ambientIntensity*diffuseColor[1];
	light_ambient[2] = ambientIntensity*diffuseColor[2];
	light_ambient[3] = alpha;
	light_diffuse[0] = diffuseColor[0];
	light_diffuse[1] = diffuseColor[1];
	light_diffuse[2] = diffuseColor[2];
	light_diffuse[3] = alpha;
	light_specular[0] = specularColor[0];
	light_specular[1] = specularColor[1];
	light_specular[2] = specularColor[2];
	light_specular[3] = alpha;
	light_emission[0] = emissiveColor[0];
	light_emission[1] = emissiveColor[1];
	light_emission[2] = emissiveColor[2];
	light_emission[3] = alpha;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void Renderer3D::setMaterial(double r, double g, double b, double alpha)
{
	GLfloat light_ambient[4],light_diffuse[4],light_specular[4];
	light_ambient[0] = r*0.3f;
	light_ambient[1] = g*0.3f;
	light_ambient[2] = b*0.3f;
	light_ambient[3] = alpha;
	light_diffuse[0] = r*0.7f;
	light_diffuse[1] = g*0.7f;
	light_diffuse[2] = b*0.7f;
	light_diffuse[3] = alpha;
	light_specular[0] = r*0.2f;
	light_specular[1] = g*0.2f;
	light_specular[2] = b*0.2f;
	light_specular[3] = alpha;
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
	glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 5.0f);
}

// ---- draw functions ----------

void Renderer3D::drawPoint(const double pos[3])
{
	glDisable(GL_LIGHTING);

	glBegin(GL_POINTS);
	glVertex3f(pos[0], pos[1], pos[2]);
	glEnd();

	glEnable(GL_LIGHTING);
}

void Renderer3D::drawLine(const double pos1[3], const double pos2[3])
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glVertex3f(pos1[0],pos1[1],pos1[2]);
	glVertex3f(pos2[0],pos2[1],pos2[2]);
	glEnd();

	glEnable(GL_LIGHTING);
}

void Renderer3D::drawSolidBox(double sx, double sy, double sz)
{
	float lx = sx*0.5f;
	float ly = sy*0.5f;
	float lz = sz*0.5f;

	// sides
	glBegin (GL_TRIANGLE_STRIP);
	glNormal3f (-1,0,0);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,-ly,lz);
	glVertex3f (-lx,ly,-lz);
	glVertex3f (-lx,ly,lz);
	glNormal3f (0,1,0);
	glVertex3f (lx,ly,-lz);
	glVertex3f (lx,ly,lz);
	glNormal3f (1,0,0);
	glVertex3f (lx,-ly,-lz);
	glVertex3f (lx,-ly,lz);
	glNormal3f (0,-1,0);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,-ly,lz);
	glEnd();

	// top face
	glBegin (GL_TRIANGLE_FAN);
	glNormal3f (0,0,1);
	glVertex3f (-lx,-ly,lz);
	glVertex3f (lx,-ly,lz);
	glVertex3f (lx,ly,lz);
	glVertex3f (-lx,ly,lz);
	glEnd();

	// bottom face
	glBegin (GL_TRIANGLE_FAN);
	glNormal3f (0,0,-1);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,ly,-lz);
	glVertex3f (lx,ly,-lz);
	glVertex3f (lx,-ly,-lz);
	glEnd();
}

void Renderer3D::drawWireBox(double sx, double sy, double sz)
{
	float lx = sx*0.5f;
	float ly = sy*0.5f;
	float lz = sz*0.5f;

	// sides
	glBegin (GL_LINES);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,-ly,lz);
	glEnd();

	glBegin (GL_LINES);
	glVertex3f (-lx,ly,-lz);
	glVertex3f (-lx,ly,lz);
	glEnd();

	glBegin (GL_LINES);
	glVertex3f (lx,ly,-lz);
	glVertex3f (lx,ly,lz);
	glEnd();

	glBegin (GL_LINES);
	glVertex3f (lx,-ly,-lz);
	glVertex3f (lx,-ly,lz);
	glEnd();

	glBegin (GL_LINES);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,-ly,lz);
	glEnd();

	// top face
	glBegin (GL_LINE_LOOP);
	glVertex3f (-lx,-ly,lz);
	glVertex3f (lx,-ly,lz);
	glVertex3f (lx,ly,lz);
	glVertex3f (-lx,ly,lz);
	glEnd();

	// bottom face
	glBegin (GL_LINE_LOOP);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,ly,-lz);
	glVertex3f (lx,ly,-lz);
	glVertex3f (lx,-ly,-lz);
	glEnd();
}

void Renderer3D::drawBox(const double pos[3], const double R[9], const double sides[3])
{
	if (cullFace_) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	glPushMatrix();
	transform(pos, R);

	switch (drawMode_) {
		case Solid: {
			glEnable(GL_LIGHTING);
			drawSolidBox(sides[0], sides[1], sides[2]);
			break;
		}
		case Wire: {
			glDisable(GL_LIGHTING);
			drawWireBox(sides[0], sides[1], sides[2]);
			break;
		}
	}

	glPopMatrix();
}

/*
 * Compute lookup table of cos and sin values forming a cirle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */

void Renderer3D::calcCircleTable(std::vector<double>& sint, std::vector<double>& cost, int n)
{
	size_t size = abs(n);

    const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

    sint.resize(size+1);
    cost.resize(size+1);

    sint[0] = 0.0;
    cost[0] = 1.0;

    for (int i = 1; i < size; ++i) {
        sint[i] = sin(angle*i);
        cost[i] = cos(angle*i);
    }

    sint[size] = sint[0];
    cost[size] = cost[0];
}

void Renderer3D::drawSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
	std::vector<double>& sint1 = sint1_;
	std::vector<double>& cost1 = cost1_;

	std::vector<double>& sint2 = sint2_;
	std::vector<double>& cost2 = cost2_;

	if (sint1.size() != (slices+1) || cost1.size() != (slices+1)) {
		Renderer3D::calcCircleTable(sint1, cost1, -slices);
	}

	if (sint2.size() != (stacks*2+1) || cost2.size() != (stacks*2+1)) {
		Renderer3D::calcCircleTable(sint2, cost2, stacks*2);

	}

	/* The top stack is covered with a triangle fan */

	double z0 = 1.0;
	double z1 = cost2[(stacks>0)?1:0];
	double r0 = 0.0;
	double r1 = sint2[(stacks>0)?1:0];

	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0,0,1);
	glVertex3d(0,0,radius);

	for (int j = slices; j >= 0; j--) {
		glNormal3d(cost1[j]*r1, sint1[j]*r1, z1);
		glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
	}

	glEnd();

	/* Cover each stack with a quad strip, except the top and bottom stacks */

	for (int i = 1; i < stacks-1; i++)	{
		z0 = z1; z1 = cost2[i+1];
		r0 = r1; r1 = sint2[i+1];

		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j <= slices; j++) {
			glNormal3d(cost1[j]*r1, sint1[j]*r1, z1);
			glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
			glNormal3d(cost1[j]*r0, sint1[j]*r0, z0);
			glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
		}

		glEnd();
	}

	/* The bottom stack is covered with a triangle fan */

	z0 = z1;
	r0 = r1;

	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0,0,-1);
	glVertex3d(0,0,-radius);

	for (int j = 0; j <= slices; j++) {
		glNormal3d(cost1[j]*r0, sint1[j]*r0, z0);
		glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
	}

	glEnd();

}

void Renderer3D::drawWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
	/* Adjust z and radius as stacks and slices are drawn. */

	double r;
	double x,y,z;

	/* Pre-computed circle */

	std::vector<double>& sint1 = sint1_;
	std::vector<double>& cost1 = cost1_;

	std::vector<double>& sint2 = sint2_;
	std::vector<double>& cost2 = cost2_;

	if (sint1.size() != (slices+1) || cost1.size() != (slices+1)) {
		Renderer3D::calcCircleTable(sint1, cost1, -slices);
	}

	if (sint2.size() != (stacks*2+1) || cost2.size() != (stacks*2+1)) {
		Renderer3D::calcCircleTable(sint2, cost2, stacks*2);

	}

	/* Draw a line loop for each stack */

	for (int i = 1; i < stacks; i++) {
		z = cost2[i];
		r = sint2[i];

		glBegin(GL_LINE_LOOP);

		for (int j = 0; j <= slices; j++) {
			x = cost1[j];
			y = sint1[j];

			glNormal3d(x,y,z);
			glVertex3d(x*r*radius,y*r*radius,z*radius);
		}

		glEnd();
	}

	/* Draw a line loop for each slice */
	for (int i = 0; i < slices; i++) {
		glBegin(GL_LINE_STRIP);

		for (int j = 0; j <= stacks; j++) {
			x = cost1[i]*sint2[j];
			y = sint1[i]*sint2[j];
			z = cost2[j];

			glNormal3d(x,y,z);
			glVertex3d(x*radius,y*radius,z*radius);
		}

		glEnd();
	}
}

void Renderer3D::drawSphere(const double pos[3], const double R[9], double r)
{
	glPushMatrix();
	transform(pos, R);
	switch (drawMode_) {
		case Solid:
			glEnable(GL_LIGHTING);
			drawSolidSphere(r, sphereQuality_, sphereQuality_);
			break;
		case Wire:
			glDisable(GL_LIGHTING);
			drawWireSphere(r, sphereQuality_, sphereQuality_);
			break;
	}
	glPopMatrix();
}


/*
 * Draws a solid cone
 */
void Renderer3D::drawSolidCone( GLdouble base, GLdouble height, GLint slices, GLint stacks )
{
    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
    const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

    /* Scaling factors for vertex normals */

    const double cosn = ( height / sqrt ( height * height + base * base ));
    const double sinn = ( base   / sqrt ( height * height + base * base ));

    /* Pre-computed circle */

	std::vector<double>& sint = circleCircleTable_.sint;
	std::vector<double>& cost = circleCircleTable_.cost;

	if (sint.size() != (slices+1) || cost.size() != (slices+1)) {
		Renderer3D::calcCircleTable(sint, cost, -slices);
	}

	/* Cover the circular base with a triangle fan... */

    double z0 = 0.0;
    double z1 = zStep;

    double r0 = base;
    double r1 = r0 - rStep;

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(0.0,0.0, z0 );

        for (int j=0; j<=slices; j++)
            glVertex3d(cost[j]*r0, sint[j]*r0, z0);

    glEnd();

    /* Cover each stack with a quad strip, except the top stack */

    for(int i=0; i<stacks-1; i++ )
    {
        glBegin(GL_QUAD_STRIP);

            for(int j=0; j<=slices; j++)
            {
                glNormal3d(cost[j]*cosn, sint[j]*cosn, sinn);
                glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
                glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
            }

            z0 = z1; z1 += zStep;
            r0 = r1; r1 -= rStep;

        glEnd();
    }

    /* The top stack is covered with individual triangles */

    glBegin(GL_TRIANGLES);

        glNormal3d(cost[0]*cosn, sint[0]*cosn, sinn);

        for (int j=0; j<slices; j++)
        {
            glVertex3d(cost[j+0]*r0,   sint[j+0]*r0,   z0    );
            glVertex3d(0,              0,              height);
            glNormal3d(cost[j+1]*cosn, sint[j+1]*cosn, sinn  );
            glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0    );
        }

    glEnd();
}

/*
 * Draws a wire cone
 */
void Renderer3D::drawWireCone( GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
    /* Step in z and radius as stacks are drawn. */

    double z = 0.0;
    double r = base;

    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
    const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

    /* Scaling factors for vertex normals */

    const double cosn = ( height / sqrt ( height * height + base * base ));
    const double sinn = ( base   / sqrt ( height * height + base * base ));

    /* Pre-computed circle */
	std::vector<double>& sint = circleCircleTable_.sint;
	std::vector<double>& cost = circleCircleTable_.cost;

	if (sint.size() != (slices+1) || cost.size() != (slices+1)) {
		Renderer3D::calcCircleTable(sint, cost, -slices);
	}

    /* Draw the stacks... */

    for (int i=0; i<stacks; i++)
    {
        glBegin(GL_LINE_LOOP);

            for(int j=0; j<slices; j++ )
            {
                glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn);
                glVertex3d(cost[j]*r,    sint[j]*r,    z   );
            }

        glEnd();

        z += zStep;
        r -= rStep;
    }

    /* Draw the slices */

    r = base;

    glBegin(GL_LINES);

        for (int j=0; j<slices; j++)
        {
            glNormal3d(cost[j]*cosn, sint[j]*cosn, sinn  );
            glVertex3d(cost[j]*r,    sint[j]*r,    0.0   );
            glVertex3d(0.0,          0.0,          height);
        }

    glEnd();
}



/*
 * Draws a solid cylinder
 */
void Renderer3D::drawSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
    int i,j;

    /* Step in z and radius as stacks are drawn. */

    double z0,z1;
    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

    /* Pre-computed circle */

	std::vector<double>& sint = cylinderCircleTable_.sint;
	std::vector<double>& cost = cylinderCircleTable_.cost;

	const int size = slices+1;
	if ((int)sint.size() != size || (int)cost.size() != size) {
		Renderer3D::calcCircleTable(sint, cost, -slices);
	}

    /* Cover the base and top */

    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0.0, 0.0, -1.0 );
        glVertex3d(0.0, 0.0,  0.0 );
        for (j=0; j<=slices; j++)
          glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0.0, 0.0, 1.0   );
        glVertex3d(0.0, 0.0, height);
        for (j=slices; j>=0; j--)
          glVertex3d(cost[j]*radius, sint[j]*radius, height);
    glEnd();

    /* Do the stacks */

    z0 = 0.0;
    z1 = zStep;

    for (i=1; i<=stacks; i++)
    {
        if (i==stacks)
            z1 = height;

        glBegin(GL_QUAD_STRIP);
            for (j=0; j<=slices; j++ )
            {
                glNormal3d(cost[j],        sint[j],        0.0 );
                glVertex3d(cost[j]*radius, sint[j]*radius, z0  );
                glVertex3d(cost[j]*radius, sint[j]*radius, z1  );
            }
        glEnd();

        z0 = z1; z1 += zStep;
    }
}

/*
 * Draws a wire cylinder
 */
void Renderer3D::drawWireCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
    int i,j;

    /* Step in z and radius as stacks are drawn. */

    double z = 0.0;
    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

    /* Pre-computed circle */
	std::vector<double>& sint = cylinderCircleTable_.sint;
	std::vector<double>& cost = cylinderCircleTable_.cost;

	const int size = slices+1;
	if ((int)sint.size() != size || (int)cost.size() != size) {
		Renderer3D::calcCircleTable(sint, cost, -slices);
	}

    /* Draw the stacks... */

    for (i=0; i<=stacks; i++)
    {
        if (i==stacks)
            z = height;

        glBegin(GL_LINE_LOOP);

            for( j=0; j<slices; j++ )
            {
                glNormal3d(cost[j],        sint[j],        0.0);
                glVertex3d(cost[j]*radius, sint[j]*radius, z  );
            }

        glEnd();

        z += zStep;
    }

    /* Draw the slices */

    glBegin(GL_LINES);

        for (j=0; j<slices; j++)
        {
            glNormal3d(cost[j],        sint[j],        0.0   );
            glVertex3d(cost[j]*radius, sint[j]*radius, 0.0   );
            glVertex3d(cost[j]*radius, sint[j]*radius, height);
        }

    glEnd();
}

// draw a capped cylinder of length l and radius r, aligned along the x axis
//static int capped_cylinder_quality = 3;

void Renderer3D::drawCapsule (float l, float r)
{
  int i,j;
  float tmp,nx,ny,nz,start_nx,start_ny,a,ca,sa;
  // number of sides to the cylinder (divisible by 4):
//  const int n = capped_cylinder_quality*4;
  const int n = ((int)capsuleQuality_/4) * 4;

  l *= 0.5;
  a = float(M_PI*2.0)/float(n);
  sa = (float) sin(a);
  ca = (float) cos(a);

  // draw cylinder body
  ny=1; nz=0;		  // normal vector = (0,ny,nz)
  glBegin (GL_TRIANGLE_STRIP);
  for (i=0; i<=n; i++) {
    glNormal3d (ny,nz,0);
    glVertex3d (ny*r,nz*r,l);
    glNormal3d (ny,nz,0);
    glVertex3d (ny*r,nz*r,-l);
    // rotate ny,nz
    tmp = ca*ny - sa*nz;
    nz = sa*ny + ca*nz;
    ny = tmp;
  }
  glEnd();

  // draw first cylinder cap
  start_nx = 0;
  start_ny = 1;
  for (j=0; j<(n/4); j++) {
    // get start_n2 = rotated start_n
    float start_nx2 =  ca*start_nx + sa*start_ny;
    float start_ny2 = -sa*start_nx + ca*start_ny;
    // get n=start_n and n2=start_n2
    nx = start_nx; ny = start_ny; nz = 0;
    float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
    glBegin (GL_TRIANGLE_STRIP);
    for (i=0; i<=n; i++) {
      glNormal3d (ny2,nz2,nx2);
      glVertex3d (ny2*r,nz2*r,l+nx2*r);
      glNormal3d (ny,nz,nx);
      glVertex3d (ny*r,nz*r,l+nx*r);
      // rotate n,n2
      tmp = ca*ny - sa*nz;
      nz = sa*ny + ca*nz;
      ny = tmp;
      tmp = ca*ny2- sa*nz2;
      nz2 = sa*ny2 + ca*nz2;
      ny2 = tmp;
    }
    glEnd();
    start_nx = start_nx2;
    start_ny = start_ny2;
  }

  // draw second cylinder cap
  start_nx = 0;
  start_ny = 1;
  for (j=0; j<(n/4); j++) {
    // get start_n2 = rotated start_n
    float start_nx2 = ca*start_nx - sa*start_ny;
    float start_ny2 = sa*start_nx + ca*start_ny;
    // get n=start_n and n2=start_n2
    nx = start_nx; ny = start_ny; nz = 0;
    float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
    glBegin (GL_TRIANGLE_STRIP);
    for (i=0; i<=n; i++) {
      glNormal3d (ny,nz,nx);
      glVertex3d (ny*r,nz*r,-l+nx*r);
      glNormal3d (ny2,nz2,nx2);
      glVertex3d (ny2*r,nz2*r,-l+nx2*r);
      // rotate n,n2
      tmp = ca*ny - sa*nz;
      nz = sa*ny + ca*nz;
      ny = tmp;
      tmp = ca*ny2- sa*nz2;
      nz2 = sa*ny2 + ca*nz2;
      ny2 = tmp;
    }
    glEnd();
    start_nx = start_nx2;
    start_ny = start_ny2;
  }
}

void Renderer3D::drawCylinder(double l, double r, double zoffset, bool drawCap)
{
	int i;
	double tmp, ny, nz, ca, sa, a;
//	const int n = 24; 	// number of sides to the cylinder (divisible by 4)
	const int n = cylinderQuality_;

	l *= 0.5;
	a = (M_PI * 2.0)/n;
	sa = sin(a);
	ca = cos(a);

	glShadeModel(GL_SMOOTH);
	  // draw cylinder body
	  ny=1; nz=0;		  // normal vector = (0,ny,nz)
	  glBegin (GL_TRIANGLE_STRIP);
	  for (i=0; i<=n; i++) {
	    glNormal3d (ny,nz,0);
	    glVertex3d (ny*r,nz*r,l+zoffset);
	    glNormal3d (ny,nz,0);
	    glVertex3d (ny*r,nz*r,-l+zoffset);
	    // rotate ny,nz
	    tmp = ca*ny - sa*nz;
	    nz = sa*ny + ca*nz;
	    ny = tmp;
	  }
	  glEnd();

//	  // 裏面
//	  {
//		  ny=1; nz=0;		  // normal vector = (0,ny,nz)
//		  glBegin (GL_TRIANGLE_STRIP);
//		  for (i=0; i<=n; i++) {
//			glNormal3d (ny,nz,0);
//		    glVertex3d (ny*r,nz*r,-l+zoffset);
//		    glNormal3d (ny,nz,0);
//		    glVertex3d (ny*r,nz*r,l+zoffset);
//		    // rotate ny,nz
//		    tmp = ca*ny - sa*nz;
//		    nz = sa*ny + ca*nz;
//		    ny = tmp;
//		  }
//		  glEnd();
//	  }

	  if (drawCap) {

	  // draw top cap
	  glShadeModel (GL_FLAT);
	  ny=1; nz=0;		  // normal vector = (0,ny,nz)
	  glBegin (GL_TRIANGLE_FAN);
	  glNormal3d (0,0,1);
	  glVertex3d (0,0,l+zoffset);
	  for (i=0; i<=n; i++) {
	    if (i==1 || i==n/2+1)
	      setMaterial(color_[0]*0.75f,color_[1]*0.75f,color_[2]*0.75f,color_[3]);
	    glNormal3d (0,0,1);
	    glVertex3d (ny*r,nz*r,l+zoffset);
	    if (i==1 || i==n/2+1)
	      setMaterial (color_[0],color_[1],color_[2],color_[3]);

	    // rotate ny,nz
	    tmp = ca*ny - sa*nz;
	    nz = sa*ny + ca*nz;
	    ny = tmp;
	  }
	  glEnd();

	  // draw bottom cap
	  ny=1; nz=0;		  // normal vector = (0,ny,nz)
	  glBegin (GL_TRIANGLE_FAN);
	  glNormal3d (0,0,-1);
	  glVertex3d (0,0,-l+zoffset);
	  for (i=0; i<=n; i++) {
	    if (i==1 || i==n/2+1)
		setMaterial (color_[0]*0.75f,color_[1]*0.75f,color_[2]*0.75f,color_[3]);
	    glNormal3d (0,0,-1);
	    glVertex3d (ny*r,nz*r,-l+zoffset);
	    if (i==1 || i==n/2+1)
		setMaterial (color_[0],color_[1],color_[2],color_[3]);

	    // rotate ny,nz
	    tmp = ca*ny + sa*nz;
	    nz = -sa*ny + ca*nz;
	    ny = tmp;
	  }
	  glEnd();

	  }

	  glShadeModel(GL_SMOOTH);
}

void Renderer3D::drawCylinder(const double pos[3], const double R[9], double length, double radius, bool drawCap)
{
	if (cullFace_) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	glPushMatrix();
	transform(pos, R);
//	drawCylinder(length, radius, 0.0, drawCap);

	switch (drawMode_) {
		case Solid:
			glEnable(GL_LIGHTING);
			drawCylinder(length, radius, 0.0, drawCap);
			break;
		case Wire:
			glDisable(GL_LIGHTING);
			glPushMatrix();
			translate(0, 0, -length/2);
			drawWireCylinder(radius, length, cylinderQuality_, 2);
			glPopMatrix();
			break;
	}

	glPopMatrix();

	glEnable(GL_CULL_FACE);
}

void Renderer3D::drawCapsule(const double pos[3], const double R[9], double length, double radius)
{
	glEnable(GL_CULL_FACE);
	glPushMatrix();
	transform(pos, R);
	drawCapsule(length, radius);
	glPopMatrix();
}

void Renderer3D::drawCone(const double pos[3], const double R[9], double length, double radius)
{
	glPushMatrix();
	transform(pos, R);
	switch (drawMode_) {
		case Solid: drawSolidCone(radius, length, circleQuality_, 1); break;
		case Wire:
			glDisable(GL_LIGHTING);
			drawWireCone(radius, length, circleQuality_, 1);
			glEnable(GL_LIGHTING);
			break;
	}
	glPopMatrix();
}

void Renderer3D::drawRing(const double pos[3], const double R[9], double length, double outer_radius, double inner_radius)
{
	glPushMatrix();
	transform(pos, R);

	glBegin(GL_QUAD_STRIP); // ポリゴンの描画

	glNormal3f(0, 0, 1);

	// 上
	for (int i = 0; i <= circleQuality_; i++) {
		// 座標を計算
		double rate = (double)i / circleQuality_;
		double c = cos(2.0 * M_PI * rate);
		double s = sin(2.0 * M_PI * rate);

		double ox = outer_radius * c;
		double oy = outer_radius * s;

		double ix = inner_radius * c;
		double iy = inner_radius * s;

		glVertex3d(ix, iy, length/2); // 頂点座標を指定
		glVertex3d(ox, oy, length/2); // 頂点座標を指定
	}

	glNormal3f(0, 0, -1);

	// 下
	for (int i = 0; i <= circleQuality_; i++) {
		// 座標を計算
		double rate = -(double)i / circleQuality_;
		double c = cos(2.0 * M_PI * rate);
		double s = sin(2.0 * M_PI * rate);

		double ox = outer_radius * c;
		double oy = outer_radius * s;

		double ix = inner_radius * c;
		double iy = inner_radius * s;

		glVertex3d(ix, iy, -length/2); // 頂点座標を指定
		glVertex3d(ox, oy, -length/2); // 頂点座標を指定
	}

	// 外周
	for (int i = 0; i <= circleQuality_; i++) {
		// 座標を計算
		double rate = (double)i / circleQuality_;
		double c = cos(2.0 * M_PI * rate);
		double s = sin(2.0 * M_PI * rate);

		double ox = outer_radius * c;
		double oy = outer_radius * s;

		glEnable(GL_NORMALIZE);
		glNormal3f(ox, oy, 0);
		glDisable(GL_NORMALIZE);

		glVertex3d(ox, oy, length/2); // 頂点座標を指定
		glVertex3d(ox, oy, -length/2); // 頂点座標を指定
	}


	// 内周
	for (int i = 0; i <= circleQuality_; i++) {
		// 座標を計算
		double rate = (double)i / circleQuality_;
		double c = cos(2.0 * M_PI * rate);
		double s = sin(2.0 * M_PI * rate);

		double ix = inner_radius * c;
		double iy = inner_radius * s;

		glEnable(GL_NORMALIZE);
		glNormal3f(-ix, -iy, 0);
		glDisable(GL_NORMALIZE);

		glVertex3d(ix, iy, -length/2); // 頂点座標を指定
		glVertex3d(ix, iy, length/2); // 頂点座標を指定
	}


	glEnd(); // ポリゴンの描画終了

	glPopMatrix();
}

void Renderer3D::drawCircle(const double pos[3], const double R[9], double r)
{
	glPushMatrix();
	transform(pos, R);

	glDisable(GL_CULL_FACE);

//	if (cullFace_) {
//		glEnable(GL_CULL_FACE);
//	} else {
//		glDisable(GL_CULL_FACE);
//	}

	glBegin(GL_POLYGON); // ポリゴンの描画

	glNormal3f(0, 0, 1);

	// 円を描画
	for (int i = 0; i < circleQuality_; i++) {
		// 座標を計算
		double rate = (double)i / circleQuality_;
		double x = r * cos(2.0 * M_PI * rate);
		double y = r * sin(2.0 * M_PI * rate);
		glVertex3d(x, y, 0.0); // 頂点座標を指定
	}
	glEnd(); // ポリゴンの描画終了

	glEnable(GL_CULL_FACE);

	glPopMatrix();
}

void Renderer3D::drawRingCircle(const double pos[3], const double R[9], double outer_radius, double inner_radius)
{
	glPushMatrix();
	transform(pos, R);

	if (cullFace_) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	glBegin(GL_QUAD_STRIP); // ポリゴンの描画

	glNormal3f(0, 0, 1);

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
	}
	glEnd(); // ポリゴンの描画終了

	glEnable(GL_CULL_FACE);

	glPopMatrix();
}

void Renderer3D::drawRect(double x1, double y1, double x2, double y2)
{
	glRectd(x1, y1, x2, y2);
}

void Renderer3D::drawRect(const double pos[3], const double R[9], double w, double h)
{
	glPushMatrix();
	transform(pos, R);
	glRectd(-w/2.0, -h/2.0, w/2.0, h/2.0);
	glPopMatrix();
}

void Renderer3D::drawArrow(const double pos1[3], const double pos2[3], double r)
{

}

void Renderer3D::drawArrowCone(const double pos1[3], const double pos2[3], double r)
{
	double d[] = {pos1[0] - pos2[0], pos1[1] - pos2[1], pos1[2] - pos2[2]};
	double length = sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
}


void Renderer3D::drawAxis(const double pos[3], const double R[9], double length)
{
	glDisable(GL_LIGHTING);

	glPushMatrix();
	transform(pos, R);

	// X軸 赤
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(length, 0.0, 0.0);
	glEnd();

	// Y軸 青
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, length, 0.0);
	glEnd();

	// Z軸 緑
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, length);
	glEnd();

	glPopMatrix();

	// restore
	setColor(color_[0], color_[1], color_[2]);
	glEnable(GL_LIGHTING);
}

void Renderer3D::drawGrid(double w, int div)
{
	const double gridmax = w;
	double gridSize = w / (div);
	int count = gridmax / gridSize;

	glDisable(GL_LIGHTING);

	// xy
	glBegin(GL_LINES);
	for (int i = -count; i <= count; ++i) {
		glVertex3f((float)gridSize*i, (float)-gridmax, 0.0);
		glVertex3f((float)gridSize*i, (float)gridmax, 0.0);
		glVertex3f((float)-gridmax, (float)gridSize*i, 0.0);
		glVertex3f((float)gridmax, (float)gridSize*i, 0.0);
	}
	glEnd();

	// restore
	glEnable(GL_LIGHTING);
}

void Renderer3D::drawPoints(const double* p, int np)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for (int i = 0; i < np; ++i) {
		glVertex3f(p[i], p[i+1], p[i+2]);
	}
	glEnd();
}

void Renderer3D::drawLines(const double* lines, int numlines)
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	for (int i = 0; i < numlines-1; ++i) {
		int l1 = (i%numlines)*3;
		int l2 = ((i+1)%numlines)*3;
		glVertex3f(lines[l1],lines[l1+1],lines[l1+2]);
		glVertex3f(lines[l2],lines[l2+1],lines[l2+2]);
	}

	glEnd();

	glEnable(GL_LIGHTING);
}

void Renderer3D::drawLineStrip(const double* lines, int numlines)
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numlines-1; ++i) {
		int l1 = (i%numlines)*3;
		int l2 = ((i+1)%numlines)*3;
		glVertex3f(lines[l1],lines[l1+1],lines[l1+2]);
		glVertex3f(lines[l2],lines[l2+1],lines[l2+2]);
	}

	glEnd();

	glEnable(GL_LIGHTING);
}

void Renderer3D::drawLineLoop(const double* lines, int numlines)
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numlines-1; ++i) {
		int l1 = (i%numlines)*3;
		int l2 = ((i+1)%numlines)*3;
		glVertex3f(lines[l1],lines[l1+1],lines[l1+2]);
		glVertex3f(lines[l2],lines[l2+1],lines[l2+2]);
	}

	glEnd();

	glEnable(GL_LIGHTING);
}

void Renderer3D::drawPoint(double x, double y, double z)
{
	glDisable(GL_LIGHTING);

	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();

	glEnable(GL_LIGHTING);
}
void Renderer3D::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();

	glEnable(GL_LIGHTING);
}

// transform

void Renderer3D::pushMatrix()
{
	glPushMatrix();
}

void Renderer3D::popMatrix()
{
	glPopMatrix();
}

// 平行、回転移動
void Renderer3D::transform(const double pos[3], const double R[9])
{
	GLdouble matrix[16];
	matrix[0] = R[0];
	matrix[1] = R[3];
	matrix[2] = R[6];
	matrix[3] = 0.0;
	matrix[4] = R[1];
	matrix[5] = R[4];
	matrix[6] = R[7];
	matrix[7] = 0.0;
	matrix[8] = R[2];
	matrix[9] = R[5];
	matrix[10] = R[8];
	matrix[11] = 0.0;
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
	matrix[15] = 1.0;
	glMultMatrixd(matrix);
}

void Renderer3D::translate(const double t[3])
{
	glTranslated(t[0],t[1],t[2]);
}

void Renderer3D::translate(double x, double y, double z)
{
	glTranslated(x,y,z);
}

void Renderer3D::rotate(const double R[9])
{
	GLdouble matrix[16];
	matrix[0] = R[0];
	matrix[1] = R[3];
	matrix[2] = R[6];
	matrix[3] = 0.0;
	matrix[4] = R[1];
	matrix[5] = R[4];
	matrix[6] = R[7];
	matrix[7] = 0.0;
	matrix[8] = R[2];
	matrix[9] = R[5];
	matrix[10] = R[8];
	matrix[11] = 0.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = 0.0;
	matrix[15] = 1.0;
	glMultMatrixd(matrix);
}

void Renderer3D::rotateRpy(double r, double p, double y)
{
	glRotated(r, 1,0,0);
	glRotated(p, 0,1,0);
	glRotated(y, 0,0,1);
}

void Renderer3D::rotateX(double deg)
{
	glRotated(deg, 1,0,0);
}

void Renderer3D::rotateY(double deg)
{
	glRotated(deg, 0,1,0);
}

void Renderer3D::rotateZ(double deg)
{
	glRotated(deg, 0,0,1);
}

void Renderer3D::scale(double scale)
{
	glScaled(scale, scale, scale);
}

void Renderer3D::scale(const double s[3])
{
	glScaled(s[0], s[1], s[2]);
}

void Renderer3D::scale(double sx, double sy, double sz)
{
	glScaled(sx, sy, sz);
}

void Renderer3D::enableLighting()
{
	glEnable(GL_LIGHTING);
}

void Renderer3D::disableLighting()
{
	glDisable(GL_LIGHTING);
}

} /* namespace tgl */
