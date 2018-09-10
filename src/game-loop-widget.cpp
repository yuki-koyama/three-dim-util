#include <three-dim-util/game-loop-widget.hpp>
#include <three-dim-util/gl-wrapper.hpp>
#include <three-dim-util/matrix.hpp>
#include <sstream>
#include <iomanip>
#include <QMouseEvent>
#include <QString>

namespace threedimutil
{
    GameLoopWidget::GameLoopWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        format.setSamples(num_samples_);
        this->setFormat(format);
    }
    
    void GameLoopWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        threedimutil::clear_color_3d(background_color_);
        
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        
        glEnable(GL_DEPTH_TEST);
    }
    
    void GameLoopWidget::resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h);
    }
    
    void GameLoopWidget::mousePressEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera_.BeginTrackball(x_dev, y_dev, threedimutil::Camera::Mode::Rotate);
        update();
    }
    
    void GameLoopWidget::mouseMoveEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera_.MoveTrackball(x_dev, y_dev);
        update();
    }
    
    void GameLoopWidget::mouseReleaseEvent(QMouseEvent*)
    {
        camera_.EndTrackball();
        update();
    }
    
    void GameLoopWidget::wheelEvent(QWheelEvent* event)
    {
        camera_.BeginTrackball(0, 0, threedimutil::Camera::Mode::Zoom);
        camera_.MoveTrackball (0, event->delta());
        camera_.EndTrackball  ();
        update();
    }
    
    void GameLoopWidget::setProjectionMatrix()
    {
        const double aspect = static_cast<double>(width()) / static_cast<double>(height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        threedimutil::mult_matrix(threedimutil::make_perspective(camera_.vertical_angle_of_view(), aspect, near_clip_, far_clip_));
    }
    
    void GameLoopWidget::setModelViewMatrix()
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        threedimutil::mult_matrix(threedimutil::make_look_at(camera_));
    }
    
    void GameLoopWidget::saveImage(const std::string& output_file_path)
    {
        grab().save(QString::fromStdString(output_file_path));
    }
    
    void GameLoopWidget::saveImageSequence(const std::string &output_directory_path,
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
            camera_.RotateAroundTarget(theta);
        }
    }
}
