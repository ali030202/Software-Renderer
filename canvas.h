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
	m_RGBA Color_Lerp(Vertex&p1, Vertex&p2,Vertex&p3);//�����Բ�ֵ
public:
	void Cover_Color(int x, int y, m_RGBA color);

	void Bresenham_drawline(Vertex p0, Vertex p1);

	void draw_model_wireframe(Model &m_modle);

	void draw_model_Flat(Model& m_modle,FLATTEPY TYPE );//��դ��ģ�ͣ�ƽ����ɫ

	void draw_trangle_Barycentric_Coordinates(Triangle& m_triangle);//���������ֵ������

	void draw_FlatTrangle(Triangle& m_triangle);//ɨ��ƽ��������

	void fill_line_trangle(Triangle& m_triangle);//������ɨ���㷨

	void clear_canvas();

	float get_wight()const { return m_width; };

	float Flat_shading(Triangle& triangle);

	float get_heigjt()const { return m_height; };

	bool  Flat_Light(Triangle& triangle);

	void draw_trangle_wireframe(Triangle& m_triangle);//���߿�

	QImage* Get_QImage();
};

