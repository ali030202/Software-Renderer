#pragma once
class FrameBuffer
{
private:
	int width, height;
	unsigned char *buffer;
public:
	FrameBuffer(int w, int h);
	unsigned char* Get_ColorBuffer();
};

