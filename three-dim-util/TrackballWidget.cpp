#include "TrackballWidget.hpp"
#include <sstream>
#include <iomanip>
#include <three-dim-util/gl-wrapper.hpp>
#include <three-dim-util/glu-wrapper.hpp>
#include <QMouseEvent>
#include <QString>

namespace threedimutil
{
    void TrackballWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        threedimutil::clear_color_3d(background_color_);
        
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        
        glEnable(GL_DEPTH_TEST);
    }
    
    void TrackballWidget::resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h);
    }
    
    TrackballWidget::TrackballWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        format.setSamples(num_samples_);
        this->setFormat(format);
    }
    
    void TrackballWidget::mousePressEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera_.BeginTrackball(x_dev, y_dev, threedimutil::Camera::Mode::Rotate);
        update();
    }
    
    void TrackballWidget::mouseMoveEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera_.MoveTrackball(x_dev, y_dev);
        update();
    }
    
    void TrackballWidget::mouseReleaseEvent(QMouseEvent*)
    {
        camera_.EndTrackball();
        update();
    }
    
    void TrackballWidget::wheelEvent(QWheelEvent* event)
    {
        camera_.BeginTrackball(0, 0, threedimutil::Camera::Mode::Zoom);
        camera_.MoveTrackball (0, event->delta());
        camera_.EndTrackball  ();
        update();
    }
    
    void TrackballWidget::setProjectionMatrix()
    {
        const double aspect = static_cast<double>(this->width()) / static_cast<double>(this->height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(camera_.vertical_angle_of_view_, aspect, 0.05, 20.0);
    }
    
    void TrackballWidget::setModelViewMatrix()
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        threedimutil::look_at(camera_.position_, camera_.target_, camera_.up_);
    }
    
    void TrackballWidget::saveImage(const std::string& output_file_path)
    {
        grab().save(QString::fromStdString(output_file_path));
    }
    
    void TrackballWidget::saveImageSequence(const std::string &output_directory_path, const std::string &prefix)
    {
        constexpr int num_frames   = 400;
        constexpr int speed_factor = 5;
        camera_.BeginTrackball(0, 0, threedimutil::Camera::Mode::Rotate);
        for (int i = 0; i < num_frames; ++ i)
        {
            const std::string num = [&]()
            {
                std::ostringstream sout;
                sout << std::setfill('0') << std::setw(5) << i;
                return sout.str();
            }();
            saveImage(output_directory_path + "/" + prefix + num + ".png");
            camera_.MoveTrackball(i * speed_factor, 0);
        }
        camera_.EndTrackball();
    }
}
