#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication3.h"
#include<qpainter.h>
#include <QKeyEvent>
#include<string>
#include"canvas.h"
#include"My_Camera.h"
#include"Model.h"

class QtWidgetsApplication3 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication3(int w,int h,QWidget *parent = nullptr);
    ~QtWidgetsApplication3();
    canvas* Get_canvas()const { return m_canvas; };
    My_Camera* Get_camera()const { return m_camera; };
    void push_back_model(std::string file) 
    { 
        model = new Model(file);
    //m_camera->set_model(*model);
    //m_camera->set_world(m_canvas); 
    };

private:
    Ui::QtWidgetsApplication3Class ui;
    canvas* m_canvas = nullptr;
    My_Camera* m_camera = nullptr;
    Model* model = nullptr;

protected:
    void paintEvent(QPaintEvent*)override;
    void keyPressEvent(QKeyEvent* event)override;
};
