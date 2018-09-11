#ifndef game_loop_widget_hpp
#define game_loop_widget_hpp

#include <three-dim-util/camera.hpp>
#include <string>
#include <chrono>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QTimer;

namespace threedimutil
{
    class GameLoopWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit GameLoopWidget(QWidget *parent = nullptr);
        
        void saveImage(const std::string& output_file_path);
        
        threedimutil::Camera& camera() { return camera_; }
        const threedimutil::Camera& camera() const { return camera_; }
        
        double& near_clip() { return near_clip_; }
        double near_clip() const { return near_clip_; }
        double& far_clip() { return far_clip_; }
        double far_clip() const { return far_clip_; }
        
        void startTimer();
        void stopTimer();
        bool isTimerActive() const;
        
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        
        // This method is intended to be overridden in user-defined classes
        void paintGL() {}
        
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent*);
        void wheelEvent(QWheelEvent* event);
        
        void setProjectionMatrix();
        void setModelViewMatrix();
        
    protected slots:
        virtual void update();
        long getElapsedTimeInMilliseconds() const;

    private:
        const int num_samples_ = 8;
        const Eigen::Vector3d background_color_ { 0.9, 0.9, 0.9 };
        
        threedimutil::Camera camera_;
        double near_clip_ = 0.05;
        double far_clip_  = 20.0;
        
        std::shared_ptr<QTimer> timer_;
        
        std::chrono::steady_clock::time_point time_point_;
    };
}

#endif /* game_loop_widget_hpp */
