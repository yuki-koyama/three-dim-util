#include <three-dim-util/trackball-widget.hpp>
#include <three-dim-util/draw-functions.hpp>
#include <Eigen/Core>
#include <QApplication>

class Widget : public threedimutil::TrackballWidget
{
public:
    Widget(QWidget *parent = nullptr) : threedimutil::TrackballWidget(parent) {}
    
protected:
    void paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        setProjectionMatrix();
        setModelViewMatrix();
        
        threedimutil::draw_points(points_, colors_);
    }
    
private:
    const Eigen::MatrixXd points_ = Eigen::MatrixXd::Random(3, 300);
    const Eigen::MatrixXd colors_ = 0.5 * (points_ + Eigen::MatrixXd::Ones(3, 300));
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}
