#ifndef TrackballWidget_hpp
#define TrackballWidget_hpp

#include <three-dim-util/camera.hpp>
#include <string>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

namespace threedimutil
{
    class TrackballWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit TrackballWidget(QWidget *parent = nullptr);
        
        void saveImage(const std::string& output_file_path);
        void saveImageSequence(const std::string& output_directory_path,
                               const std::string& prefix,
                               int num_digits = 6);
        
        threedimutil::Camera& camera() { return camera_; }
        const threedimutil::Camera& camera() const { return camera_; }
        
        double& near_clip() { return near_clip_; }
        double near_clip() const { return near_clip_; }
        double& far_clip() { return far_clip_; }
        double far_clip() const { return far_clip_; }
        
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        
        // This method should be overridden in user-defined classes
        virtual void paintGL() = 0;
        
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent*);
        void wheelEvent(QWheelEvent* event);
        
        void setProjectionMatrix();
        void setModelViewMatrix();
        
    private:
        const int num_samples_ = 8;
        const Eigen::Vector3d background_color_ { 0.9, 0.9, 0.9 };
        
        threedimutil::Camera camera_;
        double near_clip_ = 0.05;
        double far_clip_  = 20.0;
    };
}

#endif /* TrackballWidget_hpp */
