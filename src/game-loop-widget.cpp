#include <three-dim-util/widgets/game-loop-widget.hpp>
#include <three-dim-util/gl-wrapper.hpp>
#include <three-dim-util/matrix.hpp>
#include <memory>
#include <QMouseEvent>
#include <QString>
#include <QTimer>

namespace threedimutil
{
    GameLoopWidget::GameLoopWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        format.setSamples(num_samples_);
        this->setFormat(format);
        
        time_point_ = std::chrono::steady_clock::now();
        
        timer_ = std::make_shared<QTimer>(this);
        connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update()));
    }
    
    void GameLoopWidget::startTimer()
    {
        timer_->start();
    }
    
    void GameLoopWidget::stopTimer()
    {
        timer_->stop();
    }
    
    bool GameLoopWidget::isTimerActive() const
    {
        return timer_->isActive();
    }
    
    void GameLoopWidget::update()
    {
        QOpenGLWidget::update();
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
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::mouseMoveEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera_.MoveTrackball(x_dev, y_dev);
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::mouseReleaseEvent(QMouseEvent*)
    {
        camera_.EndTrackball();
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::wheelEvent(QWheelEvent* event)
    {
        camera_.BeginTrackball(0, 0, threedimutil::Camera::Mode::Zoom);
        camera_.MoveTrackball (0, event->delta());
        camera_.EndTrackball  ();
        if (!timer_->isActive()) { update(); }
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
    
    long GameLoopWidget::getElapsedTimeInMilliseconds() const
    {
        const auto current_time_point = std::chrono::steady_clock::now();
        const auto elapsed_duration   = std::chrono::duration_cast<std::chrono::milliseconds>(current_time_point - time_point_);
        
        return elapsed_duration.count();
    }
    
    void GameLoopWidget::saveImage(const std::string& output_file_path)
    {
        grab().save(QString::fromStdString(output_file_path));
    }    
}
