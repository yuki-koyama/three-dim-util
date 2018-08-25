#include <three-dim-util/glut-wrapper.hpp>
#include <three-dim-util/gl.hpp>
#include <three-dim-util/gl-wrapper.hpp>
#include <three-dim-util/draw-functions.hpp>
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
    
    void drawEllipse(double r_x, double r_y, double trans_x, double trans_y)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(trans_x, trans_y, 0.0);
        glScaled(r_x, r_y, 1.0);
        draw_circle(1.0);
        glPopMatrix();
    }
}
