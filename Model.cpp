#include "Model.h"
#include<sstream>
Model::Model(std::string file)
{
	std::ifstream fin;
	try 
	{
		fin.open(file);
		if (!fin.is_open())
		{
			return;
		}
		char buffer[1024];
		point;//顶点集合
		Vertex ver(0,0,0,m_RGBA(0,0,0,255));
		float number;
		char ch;
		while (fin.getline(buffer,sizeof(buffer)))
		{
			if (buffer[0] == 'v' && buffer[1] == ' ')
			{
				std::istringstream str_to_num(buffer);
				str_to_num >> ch;
				str_to_num >> ver.m_x;
				str_to_num >> ver.m_y;
				str_to_num >> ver.m_z;
				point.push_back(ver);
				this->vertex_number++;
			}//读取所有点
			if (buffer[0] == 'f')
			{
				Triangle triangle;
				std::istringstream str_to_num(buffer);
				str_to_num >> ch;
				for (int i = 0; i < 3; i++)
				{
					str_to_num >> number;
					triangle.m_triangle.push_back(point[number - 1]);
					str_to_num >> ch;
					str_to_num >> number;
					str_to_num >> ch;
					str_to_num >> number;
				}
				face.push_back(triangle);
				this->face_number++;
			}
		}


	}
	catch (...)
	{
		return;
	}
}

void Model::get_min_max()
{
	x_max = -100000.0;
	x_min = 100000.0;
	y_max = -100000.0;
	y_min = 100000.0;
	z_max = -100000.0;
	z_min = 100000.0;
	for (int i = 0; i <face_number; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			x_max = std::max(face[i].m_triangle[j].m_x,x_max);
			y_max = std::max(face[i].m_triangle[j].m_y, y_max);
			z_max = std::max(face[i].m_triangle[j].m_z, z_max);
			x_min = std::min(face[i].m_triangle[j].m_x, x_min);
			y_min = std::min(face[i].m_triangle[j].m_x, y_min);
			z_min = std::min(face[i].m_triangle[j].m_x, z_min);
		}
	}
}