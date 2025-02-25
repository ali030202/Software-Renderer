#include "canvas.h"
canvas::canvas(int width,int height)
{
	m_width = width;
	m_height = height;
	Z_Buffer = new int[width * height];
	std::memset(Z_Buffer, 0xc0c0c0c0, width * height * sizeof(int));
	this->buffer = new FrameBuffer(width, height);
	this->Canvas = new QImage(this->buffer->Get_ColorBuffer(), width, height, QImage::Format_RGBA8888);
}

void canvas::Cover_Color(int x, int y, m_RGBA color)
{
	if (y < this->m_height && x <this->m_width && x > 0 && y > 0)
	{
		unsigned char* p = this->Canvas->bits();
		p[y * m_width * 4 + 4 * x] = color.R;
		p[y * m_width * 4 + 4 * x + 1] = color.G;
		p[y * m_width * 4 + 4 * x + 2] = color.B;
		p[y * m_width * 4 + 4 * x + 3] = color.A;
	}
}

QImage* canvas::Get_QImage()
{
	return this->Canvas;
}

void canvas::Bresenham_drawline(Vertex p0, Vertex p1)
{
	int x0 = p0.m_x, y0 = p0.m_y;
	int x1 = p1.m_x, y1 = p1.m_y;
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int deltaY = std::abs(dy << 1);
	int middle = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			Vertex p3(y, x, 1, m_RGBA(0, 0, 0, 255));
			Color_Lerp(p0, p1, p3);
			Cover_Color(y, x, p3.m_color);
		}
		else {
			Vertex p3(x, y, 1, m_RGBA(0, 0, 0, 255));
			Color_Lerp(p0, p1, p3);
			Cover_Color(x, y, p3.m_color);
		}
		middle += deltaY;
		if (middle > dx) {
			y += (y1 > y0 ? 1 : -1);
			middle -= std::abs(dx << 1);
		}
	}
}

m_RGBA canvas::Color_Lerp(Vertex&p1, Vertex&p2,Vertex&p3)
{
	if (p1.m_x == p2.m_x)
	{
		float rate = (float(p1.m_y) - float(p3.m_y)) / (float(p1.m_y) - float(p2.m_y));
		p3.m_color.R = (1.00 - rate) * p1.m_color.R + float(p2.m_color.R) * rate;
		p3.m_color.G = (1.00 - rate) * p1.m_color.G + float(p2.m_color.G) * rate;
		p3.m_color.B = (1.00 - rate) * p1.m_color.B + float(p2.m_color.B) * rate;
	}
	else if (p1.m_y==p2.m_y)
	{
		float rate = (float(p1.m_x) - float(p3.m_x)) / (float(p1.m_x) - float(p2.m_x));
		p3.m_color.R = (1.00 - rate) * p1.m_color.R + float(p2.m_color.R) * rate;
		p3.m_color.G = (1.00 - rate) * p1.m_color.G + float(p2.m_color.G) * rate;
		p3.m_color.B = (1.00 - rate) * p1.m_color.B + float(p2.m_color.B) * rate;
	}
	else
	{
		float rate = (float(p1.m_x)-float(p3.m_x)) / (float(p1.m_x) - float(p2.m_x));
		p3.m_color.R = (1.00 - rate) * p1.m_color.R + float(p2.m_color.R) * rate;
		p3.m_color.G = (1.00 - rate) * p1.m_color.G + float(p2.m_color.G) * rate;
		p3.m_color.B = (1.00 - rate) * p1.m_color.B + float(p2.m_color.B) * rate;
	}
	return  p3.m_color;
}

void canvas::draw_trangle_wireframe(Triangle& m_triangle)
{
	for (int i = 0; i < 3; i++)
	{
		Bresenham_drawline(m_triangle.m_triangle[i], m_triangle.m_triangle[(i + 1) % 3]);
	}
}

void canvas::draw_model_wireframe(Model& m_modle)
{
	for (auto itr = m_modle.face.begin(); itr != m_modle.face.end(); itr++)
	{
		draw_trangle_wireframe(*itr);
	}
}

void canvas:: fill_line_trangle(Triangle& m_triangle)
{
	std::sort(m_triangle.m_triangle.begin(), m_triangle.m_triangle.end(),
		[](Vertex v1, Vertex v2) {return (v1.m_y > v2.m_y); });
	Vertex v1 = m_triangle.m_triangle[0];
	Vertex v2 = m_triangle.m_triangle[1];
	Vertex v3 = m_triangle.m_triangle[2];

	if (v1.m_x == v2.m_x == v3.m_x || v1.m_y == v2.m_y == v3.m_y)
	{
		return;
	}//判断是否为三角形
	else if (v1.m_y == v2.m_y ||v1.m_y == v3.m_y || v3.m_y == v2.m_y)
	{
		draw_FlatTrangle(m_triangle);
	}//判断是否为平底
	else if (v1.m_x == v3.m_x)
	{
		Vertex v4(v1.m_x, v2.m_y, 1, m_RGBA(0, 0, 0, 255));
		Color_Lerp(v1, v3, v4);
		Triangle flattrangle1(v1, v2, v4);
		Triangle flattrangle2(v3, v2, v4);
		draw_FlatTrangle(flattrangle1);
		draw_FlatTrangle(flattrangle2);
	}//判断是以直线分割
	else
	{
		float k2 = (v1.m_y - v3.m_y) / (v1.m_x - v3.m_x);
		float b2 = v1.m_y - k2 * v1.m_x;
		float x2 = (v2.m_y - b2) / k2;
		Vertex v4(x2, v2.m_y, 1, m_RGBA(0, 0, 0, 255));
		Color_Lerp(v1, v3, v4);
		Triangle flattrangle1(v1, v2, v4);
		Triangle flattrangle2(v3, v2, v4);
		draw_FlatTrangle(flattrangle1);
		draw_FlatTrangle(flattrangle2);
	}
}

void canvas::draw_FlatTrangle(Triangle& m_triangle)
{
	std::sort(m_triangle.m_triangle.begin(), m_triangle.m_triangle.end(),
		[](Vertex v1, Vertex v2) {return (v1.m_y >v2.m_y); });
	//当尖朝上
	if (m_triangle.m_triangle[2].m_y == m_triangle.m_triangle[1].m_y)
	{
		Vertex v1 = m_triangle.m_triangle[0];
		Vertex v2 = m_triangle.m_triangle[1];
		Vertex v3 = m_triangle.m_triangle[2];
		float k1, k2;
		if (v1.m_x == v2.m_x){
			k1 = 0;
			k2 = float(v1.m_y - v3.m_y) / float(v1.m_x - v3.m_x);
			float b2 = v1.m_y - k2 * v1.m_x;
			for (float y = v1.m_y; y >= v2.m_y; y--)
			{
				float x1 = v1.m_x;
				float x2 = float(y - b2) / k2;
				Vertex temp1(x1, y, 1, m_RGBA(0, 0, 0, 255));
				Vertex temp2(x2, y, 1, m_RGBA(0, 0, 0, 255));
				Color_Lerp(v1, v2, temp1);
				Color_Lerp(v1, v3, temp2);
				Bresenham_drawline(temp1, temp2);
			}
		}
		else if(v1.m_x==v3.m_x)
		{
			k2 = 0;
			k1 = float(v1.m_y - v2.m_y) / float(v1.m_x - v2.m_x);
			float b1 = v1.m_y - k1 * v1.m_x;
			for (float y = v1.m_y; y >= v2.m_y; y--)
			{
				float x1 = float(y - b1) / k1;
				float x2 = v1.m_x;
				Vertex temp1(x1, y, 1, m_RGBA(0, 0, 0, 255));
				Vertex temp2(x2, y, 1, m_RGBA(0, 0, 0, 255));
				Color_Lerp(v1, v2, temp1);
				Color_Lerp(v1, v3, temp2);
				Bresenham_drawline(temp1, temp2);
			}
		}//直角
		else
		{
			k1 = float(v1.m_y - v2.m_y) / float(v1.m_x - v2.m_x);
			k2 = float(v1.m_y - v3.m_y) / float(v1.m_x - v3.m_x);
			float b1 = v1.m_y - k1 * v1.m_x;
			float b2 = v1.m_y - k2 * v1.m_x;
			for (float y = v1.m_y; y >= v2.m_y; y--)
			{
				float x1 = float(y - b1) / k1;
				float x2 = float(y - b2) / k2;
				Vertex temp1(x1, y, 1, m_RGBA(0, 0, 0, 255));
				Vertex temp2(x2, y, 1, m_RGBA(0, 0, 0, 255));
				Color_Lerp(v1, v2, temp1);
				Color_Lerp(v1, v3, temp2);
				Bresenham_drawline(temp1, temp2);
			}
		}
	}
	//当尖朝下
	if (m_triangle.m_triangle[0].m_y == m_triangle.m_triangle[1].m_y)
	{
		Vertex v1 = m_triangle.m_triangle[2];
		Vertex v2 = m_triangle.m_triangle[1];
		Vertex v3 = m_triangle.m_triangle[0];
		if (v1.m_x == v3.m_x)
		{
			float k2 = (v1.m_y - v2.m_y) / (v1.m_x - v2.m_x);
			float b2 = v1.m_y - k2 * v1.m_x;
			for (float y = v1.m_y; y <= v2.m_y; y++)
			{
				float x1 = v1.m_x;
				float x2 = float(y - b2) / k2;
				Vertex temp1(x1, y , 1, m_RGBA(0, 0, 0, 255));
				Vertex temp2(x2, y, 1, m_RGBA(0, 0, 0, 255));
				Color_Lerp(v1, v2, temp1);
				Color_Lerp(v1, v3, temp2);
				Bresenham_drawline(temp1, temp2);
			}
		}
		else if(v1.m_x==v2.m_x)
		{
			float k2 = (v1.m_y - v3.m_y) / (v1.m_x - v3.m_x);
			float b2 = v1.m_y - k2 * v1.m_x;
			for (float y = v1.m_y; y <= v2.m_y; y++)
			{
				float x1 = v1.m_x;
				float x2 = float(y - b2) / k2;
				Vertex temp1(x1, y , 1, m_RGBA(0, 0, 0, 255));
				Vertex temp2(x2, y , 1, m_RGBA(0, 0, 0, 255));
				Color_Lerp(v1, v2, temp1);
				Color_Lerp(v1, v3, temp2);
				Bresenham_drawline(temp1, temp2);
			}
		}
		else
		{
			float k1 = (v1.m_y - v2.m_y) / (v1.m_x - v2.m_x);
			float k2 = (v1.m_y - v3.m_y) / (v1.m_x - v3.m_x);
			float b1 = v1.m_y - k1 * v1.m_x;
			float b2 = v1.m_y - k2 * v1.m_x;
			for (float y = v1.m_y; y <= v2.m_y; y++)
			{
				float x1 = float(y - b1) / k1;
				float x2 = float(y - b2) / k2;
				Vertex temp1(x1, y, 1, m_RGBA(0, 0, 0, 255));
				Vertex temp2(x2, y, 1, m_RGBA(0, 0, 0, 255));
				Color_Lerp(v1, v2, temp1);
				Color_Lerp(v1, v3, temp2);
				Bresenham_drawline(temp1, temp2);
			}
		}
	}
}

void canvas::clear_canvas()
{
	for (int i = 0; i <= m_width; i++)
	{
		for (int j = 0; j <= m_height; j++)
		{
			Cover_Color(i, j, m_RGBA(255, 255, 255, 255));
		}
	}
}

bool  canvas::Flat_Light(Triangle& triangle)
{
	light << 0, 0, -1;
	Eigen::Vector3f p1;
	Eigen::Vector3f p2;
	Eigen::Vector3f n;
	float p1_X = triangle.m_triangle[0].m_x - triangle.m_triangle[2].m_x;
	float p1_Y = triangle.m_triangle[0].m_y - triangle.m_triangle[2].m_y;
	float p1_Z = triangle.m_triangle[0].m_z - triangle.m_triangle[2].m_z;

	float p2_X = triangle.m_triangle[0].m_x - triangle.m_triangle[1].m_x;
	float p2_Y = triangle.m_triangle[0].m_y - triangle.m_triangle[1].m_y;
	float p2_Z = triangle.m_triangle[0].m_z - triangle.m_triangle[1].m_z;
	p1 << p1_X, p1_Y, p1_Z;
	p2 << p2_X, p2_Y, p2_Z;

	n = p1.cross(p2);
	n.normalize();
	float k = (n.dot(light));
	if (k > 0)
	{
		m_RGBA color(255 * k, 255 * k, 255 * k, 255);
		triangle.m_triangle[0].m_color = color;
		triangle.m_triangle[1].m_color = color;
		triangle.m_triangle[2].m_color = color;
		return true;
	}
	return false;
}

float canvas::Flat_shading(Triangle& triangle)
{
	if (Flat_Light(triangle))
	{		
		fill_line_trangle(triangle);
		draw_trangle_wireframe(triangle);
		return 1;
	}
	return 0;
}

void canvas::draw_model_Flat(Model& m_modle, FLATTEPY TYPE)
{
	std::memset(Z_Buffer, 0xc0c0c0c0, m_width * m_height * sizeof(int));
	if (TYPE==FLAT_LINE) {
		for (auto itr = m_modle.face.begin(); itr != m_modle.face.end(); itr++)
		{

			Flat_shading(*itr);
		}
	}
	else
	{
		for (auto itr = m_modle.face.begin(); itr != m_modle.face.end(); itr++)
		{

			draw_trangle_Barycentric_Coordinates(*itr);
		}
	}
	int a = 0;
	return;
}


void canvas::draw_trangle_Barycentric_Coordinates(Triangle& m_triangle)
{
	float x, y;
	Eigen::Vector3f AB;
	Eigen::Vector3f BC;
	Eigen::Vector3f CA;
	Eigen::Vector3f AP,BP,CP;
	AB << m_triangle.m_triangle[1].m_x-m_triangle.m_triangle[0].m_x, m_triangle.m_triangle[1].m_y-m_triangle.m_triangle[0].m_y,0;
	BC << m_triangle.m_triangle[2].m_x-m_triangle.m_triangle[1].m_x, m_triangle.m_triangle[2].m_y-m_triangle.m_triangle[1].m_y,0;
	CA << m_triangle.m_triangle[0].m_x-m_triangle.m_triangle[2].m_x, m_triangle.m_triangle[0].m_y-m_triangle.m_triangle[2].m_y,0;
	float x_max = ceil(std::max(std::max(m_triangle.m_triangle[1].m_x, m_triangle.m_triangle[2].m_x), m_triangle.m_triangle[0].m_x));
	float x_min = floor(std::min(std::min(m_triangle.m_triangle[1].m_x, m_triangle.m_triangle[2].m_x), m_triangle.m_triangle[0].m_x));
	float y_max = ceil(std::max(std::max(m_triangle.m_triangle[1].m_y, m_triangle.m_triangle[2].m_y), m_triangle.m_triangle[0].m_y));
	float y_min = floor(std::min(std::min(m_triangle.m_triangle[1].m_y, m_triangle.m_triangle[2].m_y), m_triangle.m_triangle[0].m_y));
	if (y_max <= 0 || x_max >= m_height) { x_max <= 0 ? 1 : m_height - 1; };
	if (x_max <= 0 || x_max >= m_width) { x_max <= 0 ? 1 : m_width - 1; };
	if (y_min <= 0 || y_min >= m_height) { y_min <= 0 ? 1 : m_height - 1; };
	if (y_min <= 0 || y_min >= m_height) { y_min <= 0 ? 1 : m_height - 1; };

	for (y = y_max; y >= y_min; y--)
	{
		if (y<0 || y>=m_height)continue;
		for (x = x_min; x <= x_max; x++)
		{
			if (x <= 0 || x >=m_width)continue;
			AP << x - m_triangle.m_triangle[0].m_x, y - m_triangle.m_triangle[0].m_y,0;
			BP<< x - m_triangle.m_triangle[1].m_x, y - m_triangle.m_triangle[1].m_y,0;
			CP<< x - m_triangle.m_triangle[2].m_x, y - m_triangle.m_triangle[2].m_y,0;
			float temp1, temp2, temp3;
			auto P1 =AP.cross(AB);
			auto P2 = BP.cross(BC);
			auto P3 = CP.cross(CA);
			if (P1.z() > 0 && P2.z() > 0 && P3.z() > 0 || P1.z() < 0 && P2.z() < 0 && P3.z() < 0)
			{
				float ABC = AB.cross(-BC).norm();
				float i = float(P2.norm()) / ABC;
				float j = float(P3.norm()) / ABC;
				float k = 1 - i - j;
				float p_z = i * m_triangle.m_triangle[0].m_z + j * m_triangle.m_triangle[1].m_z + k *m_triangle.m_triangle[2].m_z;
		

				if (p_z >= Z_Buffer[int(y * m_width + x)])
				{
					Z_Buffer[int(y * m_width + x)] = p_z;
					if (Flat_Light(m_triangle))
					{
						m_RGBA one(
							(m_triangle.m_triangle[0].m_color.R * i + m_triangle.m_triangle[1].m_color.R * j + m_triangle.m_triangle[2].m_color.R * k), (m_triangle.m_triangle[0].m_color.G * i + m_triangle.m_triangle[1].m_color.G * j + m_triangle.m_triangle[2].m_color.G * k),
							(m_triangle.m_triangle[0].m_color.B * i + m_triangle.m_triangle[1].m_color.B * j + m_triangle.m_triangle[2].m_color.B * k), 255
						);
						Cover_Color(x, y, one);
					}
				}
			}
		}
	}
	return;
}

