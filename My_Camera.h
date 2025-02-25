#pragma once
#include <Eigen/Dense>
#include<math.h>
#include<vector>
#include"Model.h"
#include"canvas.h"
#define pi 3.14/180
class My_Camera
{
private:
	std::vector<Model*>model_pool;
	Eigen::Matrix4f M_cam;
	Eigen::Matrix4f M_model;
	Eigen::Matrix4f M_per;
	Eigen::Matrix4f M_view;
	Eigen::Vector3f at;//原点坐标
	Eigen::Vector3f e;//相机位置
	Eigen::Vector3f up;//视点正上方
public:
	My_Camera();
	void set_M_model(float N, float D, float M);
	void set_M_cam(Eigen::Vector3f _E, Eigen::Vector3f T,Eigen::Vector3f _At);
	void set_M_per(Model& modle);
	void transform(Model& modle);
	void set_view(float w, float h);
	
};

