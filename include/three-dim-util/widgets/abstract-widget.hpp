#ifndef abstract_widget_hpp
#define abstract_widget_hpp

#include <three-dim-util/camera.hpp>
#include <three-dim-util/gl-wrapper.hpp>
#include <string>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QString>

namespace threedimutil
{
    class AbstractWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit AbstractWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent)
        {
            QSurfaceFormat format = QSurfaceFormat::defaultFormat();
            format.setSamples(num_samples_);
            this->setFormat(format);
        }
        
        void saveImage(const std::string& output_file_path)
        {
            grab().save(QString::fromStdString(output_file_path));
        }
        
        threedimutil::Camera& camera() { return camera_; }
        const threedimutil::Camera& camera() const { return camera_; }
        
        double& near_clip() { return near_clip_; }
        double near_clip() const { return near_clip_; }
        double& far_clip() { return far_clip_; }
        double far_clip() const { return far_clip_; }
        
    protected:
        void initializeGL()
        {
            initializeOpenGLFunctions();
            threedimutil::clear_color_3d(background_color_);
            
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            
            glEnable(GL_DEPTH_TEST);
        }
        
        void resizeGL(int w, int h)
        {
            glViewport(0, 0, w, h);
        }
        
        // This method should be overridden in user-defined classes
        virtual void paintGL() = 0;
        
    private:
        const int num_samples_ = 8;
        const Eigen::Vector3d background_color_ { 0.9, 0.9, 0.9 };
        
        threedimutil::Camera camera_;
        double near_clip_ = 0.05;
        double far_clip_  = 20.0;
    };
}

#endif /* abstract_widget_hpp */
