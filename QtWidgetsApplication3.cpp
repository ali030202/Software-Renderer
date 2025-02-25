#include "QtWidgetsApplication3.h"

QtWidgetsApplication3::QtWidgetsApplication3(int w,int h,QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(QSize(w, h));
    this->m_canvas = new canvas(w, h);
    this->m_camera = new My_Camera;
    this->model = nullptr;
    ui.setupUi(this);
}

QtWidgetsApplication3::~QtWidgetsApplication3()
{}

void QtWidgetsApplication3::paintEvent(QPaintEvent*event)
{
    if(m_canvas)
    {
        QPainter painter(this);  //QPainter对象
        this->update();
        m_canvas->clear_canvas();
        m_camera->transform(*model);
        m_canvas->draw_model_wireframe(*model);//线框模式
        m_canvas->draw_model_Flat(*model,BARYCENTRIC);//扫线填充模型
       Vertex p1(300,300,500, m_RGBA(255, 0, 0, 255));
        Vertex p2(300,400,500, m_RGBA(0, 255, 0, 255));
        Vertex p3(500,350,300, m_RGBA(0, 0, 255, 255));
        Triangle one(p1, p2, p3);
        m_canvas->draw_trangle_Barycentric_Coordinates(one);
        painter.drawImage(10, 10, *(this->m_canvas->Get_QImage()));
        QWidget::paintEvent(event);
    }
}

void QtWidgetsApplication3::keyPressEvent(QKeyEvent* event)
{
    //float angle = m_camera->get_angle();
    switch(event->key())
    {
    case Qt::Key_A:
        //m_camera->ch_revolve_y(++angle,150,150);
        return;
    case Qt::Key_D:
        //m_camera->ch_revolve_y(--angle,150,150);
        return;
    default:
        return;
    }
}