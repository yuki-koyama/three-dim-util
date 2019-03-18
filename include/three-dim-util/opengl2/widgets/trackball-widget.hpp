#ifndef TrackballWidget_hpp
#define TrackballWidget_hpp

#include <three-dim-util/opengl2/widgets/abstract-widget.hpp>
#include <three-dim-util/camera.hpp>
#include <string>

namespace threedimutil
{
    class TrackballWidget : public AbstractWidget
    {
        Q_OBJECT
    public:
        explicit TrackballWidget(QWidget *parent = nullptr);
        
        void saveImageSequence(const std::string& output_directory_path,
                               const std::string& prefix,
                               int num_digits = 6);
        
    protected:
        // This method should be overridden in user-defined classes
        virtual void paintGL() = 0;
        
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent*);
        void wheelEvent(QWheelEvent* event);
        
        void setProjectionMatrix();
        void setModelViewMatrix();
    };
}

#endif /* TrackballWidget_hpp */
