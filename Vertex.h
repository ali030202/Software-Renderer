#pragma once
#include<vector>
#include"m_RGBA.h"
class Vertex
{
public:
	float m_x;
	float m_y;
	float m_z;
	m_RGBA m_color;
public:
	Vertex(float x, float y, float z = 1,m_RGBA color=m_RGBA(0,0,0,255)) :m_x(x), m_y(y), m_z(z),m_color(color){};
};

