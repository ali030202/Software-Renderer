#pragma once
typedef unsigned char m_bite;
struct m_RGBA
{
	m_bite R;
	m_bite G;
	m_bite B;
	m_bite A;
	constexpr m_RGBA(m_bite r, m_bite g, m_bite b, m_bite a) :R(r), G(g), B(b), A(a) {};
};

