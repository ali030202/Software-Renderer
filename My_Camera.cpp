#include "My_Camera.h"
My_Camera::My_Camera()
{
	Eigen::Vector3f At; At << 0, 0, 0;
	Eigen::Vector3f E;E<<0,0,500;
	Eigen::Vector3f Up; Up << 0, 1, 0;
	this->set_M_cam(E,Up,At);
	////摄像机变换初始化
	this->set_M_model(300, 180, 300);
	//模型变换初始化
}


void My_Camera::set_M_model(float N, float D, float M)
{
	Eigen::Matrix4f revolve1;
	Eigen::Matrix4f scanl;
	revolve1 << cos(D * pi), 0.0, sin(D * pi), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(D * pi), 0.0, cos(D * pi), 0.0,
		0.0, 0.0, 0.0, 1.0;//旋转180度
	scanl << N, 0, 0, M,
		0, N, 0, M,
		0, 0, N, M,
		0, 0, 0, 1;//缩放,平移	
	this->M_model = revolve1*scanl;
}

void My_Camera::set_M_per(Model& modle)
{
	modle.get_min_max();
	float r = modle.get_x_max();
	float l = modle.get_x_min();
	float t = modle.get_y_max();
	float b = modle.get_y_min();
	float n = modle.get_z_max();
	float f = modle.get_z_min();
	M_per << n/r, 0, 0, 0,
		0, n/t,0, 0,
		0, 0, -(f + n) / (n - f), -2 * f * n / (f - n),
		0, 0, -1, 0;
		

}

void My_Camera::set_view(float w,float h)
{
	M_view << w / 2, 0, 0, w / 2,
		0, h / 2, 0, h / 2,
		0, 0, 1, 0,
		0, 0, 0, 1;
}

void My_Camera::set_M_cam(Eigen::Vector3f _E, Eigen::Vector3f _Up, Eigen::Vector3f _At)
{
	e = _E, up = _Up,at=_At;
	Eigen::Matrix4f P;
	Eigen::Matrix4f T;
	Eigen::Vector3f Z;
	Eigen::Vector3f X;
	Eigen::Vector3f Y;
	Z = (at - e).normalized();
	X = up.cross(Z).normalized();
	Y = Z.cross(X).normalized();
	//Eigen::Vector3f w;
	//Eigen::Vector3f u;
	//Eigen::Vector3f v;
	//w = -1 * g / fabs(g.norm());
	//u = (t.cross(w)) / fabs((t.cross(w)).norm());
	//v = w.cross(u);
	P << 1, 0, 0, -e.x(),
		0, 1, 0, -e.y(),
		0, 0, 1, -e.z(),
		0, 0, 0, 1;
	T << X.x(), X.y(), X.z(), 0,
		Y.x(), Y.y(), Y.z(), 0,
		Z.x(), Z.y(), Z.z(), 0,
		0, 0, 0, 1;
	M_cam = T * P;
}


void My_Camera::transform(Model& modle)
{
	this->set_M_per(modle);
	this->set_view(800, 800);
	for (int i = 0; i < modle.face.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float x = modle.face[i].m_triangle[j].m_x;
			float y = modle.face[i].m_triangle[j].m_y;
			float z = modle.face[i].m_triangle[j].m_z;
			Eigen::Vector4f point(x, y, z, 1);
			//point = M_model * point;
			//point = M_cam * point;
			//point = M_per * point;
			point = M_view * point;
			modle.face[i].m_triangle[j].m_x = (point.x() - 0.5);
			modle.face[i].m_triangle[j].m_y = (point.y() - 0.5);
			modle.face[i].m_triangle[j].m_z = (point.z() - 0.5);
		}
	}
}
