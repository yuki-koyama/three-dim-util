#include <three-dim-util/glut-wrapper.hpp>
#include <three-dim-util/gl.hpp>
#include <three-dim-util/gl-wrapper.hpp>
#include <list>
#include <Eigen/Geometry>

namespace threedimutil
{
    using Eigen::Vector3d;
    using Eigen::Matrix4d;
        
    void drawSphere(double r, const Vector3d& p, int res)
    {
        glPushMatrix();
        translate(p);
        drawSphere(r, res);
        glPopMatrix();
    }
    
    void drawSphere(double r, int res)
    {
        glutSolidSphere(r, res, res);
    }
    
    void drawCylinder(double r, double h, int res, bool center)
    {
        drawCylinder(r, r, h, res, center);
    }
    
    void drawCylinder(double r1, double r2, double h, int res, bool center)
    {
        glPushMatrix();
        if (center) glTranslatef(0.0f, 0.0f, - static_cast<GLfloat>(h / 2.0));
        GLUquadricObj* qobj = gluNewQuadric();
        gluCylinder(qobj, r1, r2, h, res, 1);
        glScalef(-1.0f, 1.0f, -1.0f);
        gluDisk(qobj, 0.0, r1, res, 1);
        glScalef(-1.0f, 1.0f, -1.0f);
        glTranslatef(0.0f, 0.0f, static_cast<GLfloat>(h));
        gluDisk(qobj, 0.0, r2, res, 1);
        gluDeleteQuadric(qobj);
        glPopMatrix();
    }
    
    void drawCylinder(double r, const Vector3d& p1, const Vector3d& p2, int res)
    {
        Vector3d   t   = p2 - p1;
        Vector3d   t0  = Vector3d(0.0, 0.0, 1.0);
        double     c   = t.dot(t0) / t.norm();
        double     q   = acos(c);
        Vector3d   ax  = t0.cross(t).normalized();
        double     h   = t.norm();
        
        Eigen::Affine3d rot(Eigen::AngleAxis<double>(q, ax));
        if (fabs(c - 1.0) < 1e-16) rot = Matrix4d::Identity();
        
        glPushMatrix();
        translate(p1);
        glMultMatrixd(rot.data()); // rot.data() is column-major
        drawCylinder(r, h, res, false);
        glPopMatrix();
    }
    
    void drawCube(double size, bool center)
    {
        assert (center);
        
        glutSolidCube(size);
    }
    
    void drawCube(double x, double y, double z, bool center)
    {
        assert (center);
        glPushMatrix();
        glScaled(x, y, z);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    
    void drawCube(const Vector3d& t, double x, double y, double z, bool center)
    {
        assert (center);
        glPushMatrix();
        translate(t);
        glScaled(x, y, z);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    
    void drawFrame(double length, double width)
    {
        const double l = length;
        const double m = 0.0;
        glLineWidth(width);
        glBegin(GL_LINES);
        glColor3d (0.9, 0.0, 0.0);
        glVertex3d(  m, 0.0, 0.0);
        glVertex3d(  l, 0.0, 0.0);
        glColor3d (0.0, 0.9, 0.0);
        glVertex3d(0.0,   m, 0.0);
        glVertex3d(0.0,   l, 0.0);
        glColor3d (0.0, 0.0, 0.9);
        glVertex3d(0.0, 0.0,   m);
        glVertex3d(0.0, 0.0,   l);
        glEnd();
    }
    
    void drawRectangle(const Eigen::Vector2d& t, double x, double y)
    {
        drawRectangle(t(0), t(1), x, y);
    }
    
    void drawRectangle(double trans_x, double trans_y, double size_x, double size_y)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(trans_x, trans_y, 0.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(+ 0.5 * size_x, + 0.5 * size_y);
        glVertex2d(+ 0.5 * size_x, - 0.5 * size_y);
        glVertex2d(- 0.5 * size_x, - 0.5 * size_y);
        glVertex2d(- 0.5 * size_x, + 0.5 * size_y);
        glEnd();
        glPopMatrix();
    }
    
    void drawCircle(double r)
    {
        const int res = 60;
        GLUquadricObj* qobj = gluNewQuadric();
        gluDisk(qobj, 0.0, r, res, 1);
        gluDeleteQuadric(qobj);
    }
    
    void drawCircle(double r, double trans_x, double trans_y)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(trans_x, trans_y, 0.0);
        drawCircle(r);
        glPopMatrix();
    }
    
    void drawEllipse(double r_x, double r_y, double trans_x, double trans_y)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(trans_x, trans_y, 0.0);
        glScaled(r_x, r_y, 1.0);
        drawCircle(1.0);
        glPopMatrix();
    }
}
