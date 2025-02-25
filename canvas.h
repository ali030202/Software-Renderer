#pragma once
#include <QImage>
#include"FrameBuffer.h"
#include"m_RGBA.h"
#include"Vertex.h"
#include"Model.h"
#include <Eigen/Dense>
enum FLATTEPY { BARYCENTRIC,FLAT_LINE };
class canvas
{
public:
	canvas(int width, int height);
private:
	QImage* Canvas=nullptr;
	FrameBuffer* buffer=nullptr;
	int m_width;
	int m_height;
	Eigen::Vector3f light ;
	int* Z_Buffer = nullptr;
private:
	m_RGBA Color_Lerp(Vertex&p1, Vertex&p2,Vertex&p3);//简单线性插值
public:
	void Cover_Color(int x, int y, m_RGBA color);

	void Bresenham_drawline(Vertex p0, Vertex p1);

	void draw_model_wireframe(Model &m_modle);

	void draw_model_Flat(Model& m_modle,FLATTEPY TYPE );//光栅化模型，平面着色

	void draw_trangle_Barycentric_Coordinates(Triangle& m_triangle);//重心坐标插值三角形

	void draw_FlatTrangle(Triangle& m_triangle);//扫线平底三角形

	void fill_line_trangle(Triangle& m_triangle);//三角形扫线算法

	void clear_canvas();

	float get_wight()const { return m_width; };

	float Flat_shading(Triangle& triangle);

	float get_heigjt()const { return m_height; };

	bool  Flat_Light(Triangle& triangle);

	void draw_trangle_wireframe(Triangle& m_triangle);//画线框

	QImage* Get_QImage();
};

