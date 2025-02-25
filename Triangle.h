#pragma once
#include<vector>
#include"Vertex.h"
class Triangle
{
public:
	std::vector<Vertex>m_triangle;
public:
	Triangle() {};
	Triangle(Vertex v1, Vertex v2, Vertex v3);
};

