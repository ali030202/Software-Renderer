#include "QtWidgetsApplication3.h"
#include <QtWidgets/QApplication>
#include <Eigen/Dense>
#include<math.h>
#include"Vertex.h"
#include"My_Camera.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetsApplication3 w(1000,1000);
    w.push_back_model("african_head//african_head.obj");
    //w.push_back_model("diablo3_pose//diablo3_pose.obj");
    w.show();
    return a.exec();
}

