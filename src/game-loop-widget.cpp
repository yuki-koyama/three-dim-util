#include <three-dim-util/widgets/game-loop-widget.hpp>
#include <three-dim-util/opengl2/gl-wrappers.hpp>
#include <three-dim-util/matrix.hpp>
#include <QMouseEvent>
#include <QString>
#include <QTimer>

namespace threedimutil
{
    GameLoopWidget::GameLoopWidget(QWidget *parent) : threedimutil::AbstractWidget(parent)
    {
        time_point_ = std::chrono::steady_clock::now();
        timer_      = std::make_shared<QTimer>(this);
        
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
    
    void GameLoopWidget::mousePressEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera().BeginTrackball(x_dev, y_dev, threedimutil::Camera::Mode::Rotate);
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::mouseMoveEvent(QMouseEvent* event)
    {
        const int x_dev = devicePixelRatio() * event->x();
        const int y_dev = devicePixelRatio() * event->y();
        
        camera().MoveTrackball(x_dev, y_dev);
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::mouseReleaseEvent(QMouseEvent*)
    {
        camera().EndTrackball();
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::wheelEvent(QWheelEvent* event)
    {
        camera().BeginTrackball(0, 0, threedimutil::Camera::Mode::Zoom);
        camera().MoveTrackball (0, event->delta());
        camera().EndTrackball  ();
        if (!timer_->isActive()) { update(); }
    }
    
    void GameLoopWidget::setProjectionMatrix()
    {
        const double aspect = static_cast<double>(width()) / static_cast<double>(height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        threedimutil::mult_matrix(threedimutil::make_perspective(camera().vertical_angle_of_view(), aspect, near_clip(), far_clip()));
    }
    
    void GameLoopWidget::setModelViewMatrix()
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        threedimutil::mult_matrix(threedimutil::make_look_at(camera()));
    }
    
    long GameLoopWidget::getElapsedTimeInMilliseconds() const
    {
        const auto current_time_point = std::chrono::steady_clock::now();
        const auto elapsed_duration   = std::chrono::duration_cast<std::chrono::milliseconds>(current_time_point - time_point_);
        
        return elapsed_duration.count();
    }
}
