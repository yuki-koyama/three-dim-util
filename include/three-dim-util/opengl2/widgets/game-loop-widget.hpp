#ifndef game_loop_widget_hpp
#define game_loop_widget_hpp

#include <three-dim-util/opengl2/widgets/abstract-widget.hpp>
#include <chrono>
#include <memory>
#include <string>

class QTimer;

namespace threedimutil
{
    class GameLoopWidget : public AbstractWidget
    {
        Q_OBJECT
    public:
        explicit GameLoopWidget(QWidget *parent = nullptr);
        
        void startTimer();
        void stopTimer();
        bool isTimerActive() const;
        
    protected:
        // This method should be overridden in user-defined classes
        virtual void paintGL() = 0;
        
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
        std::shared_ptr<QTimer> timer_;
        std::chrono::steady_clock::time_point time_point_;
    };
}

#endif /* game_loop_widget_hpp */
