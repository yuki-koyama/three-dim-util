#include <three-dim-util/widgets/abstract-widget.hpp>
#include <three-dim-util/opengl2/gl-wrappers.hpp>

namespace threedimutil
{
    AbstractWidget::AbstractWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        format.setSamples(num_samples_);
        this->setFormat(format);
    }
    
    void AbstractWidget::updateBackgroundColor(const Eigen::Vector3d& color)
    {
        background_color_ = color;
        threedimutil::clear_color_3d(background_color_);
    }
    
    void AbstractWidget::saveImage(const std::string& output_file_path)
    {
        grab().save(QString::fromStdString(output_file_path));
    }
    
    void AbstractWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        threedimutil::clear_color_3d(background_color_);
        
        this->glEnable(GL_LINE_SMOOTH);
        this->glEnable(GL_POLYGON_SMOOTH);
        this->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        this->glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        
        this->glEnable(GL_DEPTH_TEST);
    }
    
    void AbstractWidget::resizeGL(int w, int h)
    {
        this->glViewport(0, 0, w, h);
    }
}
