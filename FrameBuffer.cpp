#include "FrameBuffer.h"
#include<cstring>
FrameBuffer::FrameBuffer(int w, int h)
{
	width = w;
	height = h;
	this->buffer = new unsigned char[4 * w * h];
	memset(buffer, 255, 4 * w * h);//³õÊ¼»¯buffer
}

unsigned char* FrameBuffer::Get_ColorBuffer()
{
	return this->buffer;
}