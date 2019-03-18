#include <three-dim-util/opengl2/widgets/trackball-widget.hpp>
#include <three-dim-util/opengl2/gl.hpp>
#include <three-dim-util/opengl2/gl-wrappers.hpp>
#include <three-dim-util/matrix.hpp>
#include <sstream>
#include <iomanip>
#include <QMouseEvent>
#include <QString>

namespace threedimutil
{
    TrackballWidget::TrackballWidget(QWidget *parent) : AbstractWidget(parent)
    {
    }
    
    void TrackballWidget::mousePressEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera().BeginTrackball(x_dev, y_dev, threedimutil::Camera::Mode::Rotate);
        update();
    }
    
    void TrackballWidget::mouseMoveEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera().MoveTrackball(x_dev, y_dev);
        update();
    }
    
    void TrackballWidget::mouseReleaseEvent(QMouseEvent*)
    {
        camera().EndTrackball();
        update();
    }
    
    void TrackballWidget::wheelEvent(QWheelEvent* event)
    {
        camera().BeginTrackball(0, 0, threedimutil::Camera::Mode::Zoom);
        camera().MoveTrackball (0, event->delta());
        camera().EndTrackball  ();
        update();
    }
    
    void TrackballWidget::setProjectionMatrix()
    {
        const double aspect = static_cast<double>(width()) / static_cast<double>(height());
        this->glMatrixMode(GL_PROJECTION);
        this->glLoadIdentity();
        threedimutil::mult_matrix(threedimutil::make_perspective(camera().vertical_angle_of_view(), aspect, near_clip(), far_clip()));
    }
    
    void TrackballWidget::setModelViewMatrix()
    {
        this->glMatrixMode(GL_MODELVIEW);
        this->glLoadIdentity();
        threedimutil::mult_matrix(threedimutil::make_look_at(camera()));
    }
    
    void TrackballWidget::saveImageSequence(const std::string &output_directory_path,
                                            const std::string &prefix,
                                            int num_digits)
    {
        assert(num_digits >= 2);
        constexpr int    num_frames = 300;
        constexpr int    times      = 2;
        constexpr double theta      = static_cast<double>(times) * 2.0 * M_PI / static_cast<double>(num_frames);
        for (int i = 0; i < num_frames; ++ i)
        {
            const std::string num = [&]()
            {
                std::ostringstream sout;
                sout << std::setfill('0') << std::setw(num_digits) << i;
                return sout.str();
            }();
            saveImage(output_directory_path + "/" + prefix + num + ".png");
            camera().RotateAroundTarget(theta);
        }
    }
}
