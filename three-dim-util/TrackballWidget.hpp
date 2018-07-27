#ifndef TrackballWidget_hpp
#define TrackballWidget_hpp

#include <string>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <three-dim-util/camera.hpp>

namespace threedimutil
{
    class TrackballWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
    public:
        TrackballWidget(QWidget *parent = nullptr);
        
        void initializeGL();
        void resizeGL(int w, int h);
        
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent*);
        void wheelEvent(QWheelEvent* event);
        
        void saveImage(const std::string& output_file_path);
        void saveImageSequence(const std::string& output_directory_path, const std::string& prefix);
        
        threedimutil::Camera& camera() { return camera_; }
        
    protected:
        void setProjectionMatrix();
        void setModelViewMatrix();

    private:
        const int num_samples_ = 8;
        const Eigen::Vector3d background_color_ { 0.9, 0.9, 0.9 };
        
        threedimutil::Camera camera_;
    };
}

#endif /* TrackballWidget_hpp */
