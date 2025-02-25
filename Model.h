#pragma once
#include"Triangle.h"
#include<fstream>
#include<string>

class Model
{
private:
	int face_number=0;
	int vertex_number=0;
	float x_max = -10000.0;
	float x_min =  10000.0;
	float y_max = -10000.0;
	float y_min =  10000.0;
	float z_max = -10000.0;
	float z_min =  10000.0;
public:
	std::vector<Vertex> point;
	std::vector<Triangle> face;
public:
	Model(std::string file);
	void get_min_max();
	float get_x_max()const { return x_max; };
	float get_y_max()const { return y_max; };
	float get_z_max()const { return z_max; };
	float get_x_min()const { return x_min; };
	float get_y_min()const { return y_min; };
	float get_z_min()const { return z_min; };
};

