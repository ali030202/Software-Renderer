#include "Triangle.h"
Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3)
{
	this->m_triangle.push_back(v1);
	this->m_triangle.push_back(v2);
	this->m_triangle.push_back(v3);
}