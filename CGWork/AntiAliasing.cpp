#pragma once

#include "AntiAliasing.h"
#include "PngWrapper.h"

/* ----------------------------------------- 3X3 Filters -----------------------------------------*/
Mat3 AntiAliasing::BoxFilter3X3 = Mat3(
	1, 1, 1,
	1, 1, 1,
	1, 1, 1
) * (1.0 / 9.0);

Mat3 AntiAliasing::TriangleFilter3X3 = Mat3(
	1, 2, 1,
	2, 4, 2,
	1, 2, 1
) * (1.0 / 16.0);

Mat3 AntiAliasing::GaussianFilter3X3 = Mat3(
	1, 2, 1,
	2, 5, 2,
	1, 2, 1
) * (1.0 / 17.0);

Mat3 AntiAliasing::SincFilter3X3 = Mat3(
	2, 3, 2,
	3, 4, 3,
	2, 3, 2
) * (1.0 / 24.0);

/* ----------------------------------------- 5X5 Filters -----------------------------------------*/
Mat5 AntiAliasing::BoxFilter5X5 = Mat5(
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1
) * (1.0 / 25.0);

Mat5 AntiAliasing::TriangleFilter5X5 = Mat5(
	1, 2, 3, 2, 1,
	2, 4, 6, 4, 2,
	3, 6, 9, 6, 3,
	2, 4, 6, 4, 2,
	1, 2, 3, 2, 1
) * (1.0 / 81.0);

Mat5 AntiAliasing::GaussianFilter5X5 = Mat5(
	1, 1, 1, 1, 1,
	1, 2, 4, 2, 1,
	1, 4, 10, 4, 1,
	1, 2, 4, 2, 1,
	1, 1, 1, 1, 1
) * (1.0 / 50.0);

Mat5 AntiAliasing::SincFilter5X5 = Mat5(
	-2, -1, 0, -1, -2,
	-1,  4, 6,  4, -1,
	 0,  6, 9,  6,  0,
	-1,  4, 6,  4, -1,
	-2, -1, 0, -1, -2
) * (1.0 / 33.0);


BUFFER AntiAliasing::PrepareForPrint(BUFFER ssFB, int width, int height)
{
	if (!IsEnabled) return ssFB;

	BUFFER newFB = new int[width*height];
	m_sizeFactor = Size == AA_3X3 ? 3 : 5;
	m_bigWidth = width * m_sizeFactor;
	SetMatrix();


	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			COLORREF color = GetColor(ssFB, x, y, width);
			int pos = x + y * width;
			newFB[pos] = color;
		}
	}

	delete[] ssFB;
	return newFB;
}

COLORREF AntiAliasing::GetColor(BUFFER ssFB, int xStart, int yStart,int width)
{
	double r = 0;
	double g = 0;
	double b = 0;
	
	for (int x = 0; x < m_sizeFactor; x++) {
		for (int y = 0; y < m_sizeFactor; y++) {
			int index = m_sizeFactor * xStart + m_bigWidth*m_sizeFactor*yStart + x + m_bigWidth*y;
			COLORREF currColor = ssFB[index];
			double factor = Size == AA_3X3 ? m_currFilter3X3[x][y] : m_currFilter5X5[x][y];
			r += GET_A(currColor) * factor;
			g += GET_B(currColor) * factor;
			b += GET_G(currColor) * factor;
		}
	}

	b = b < 0 ? 0 : b;
	r = r < 0 ? 0 : r;
	g = g < 0 ? 0 : g;

	return RGB((int)r, (int)g, (int)b);
}

void AntiAliasing::SetMatrix()
{
	if (Size == AA_3X3) {
		switch (Type)
		{
		case AA_FILTER_BOX:
			m_currFilter3X3 = AntiAliasing::BoxFilter3X3;
			break;
		case AA_FILTER_TRIANGLE:
			m_currFilter3X3 = AntiAliasing::TriangleFilter3X3;
			break;
		case AA_FILTER_GAUSSIAN:
			m_currFilter3X3 = AntiAliasing::GaussianFilter3X3;
			break;
		case AA_FILTER_SINC:
			m_currFilter3X3 = AntiAliasing::SincFilter3X3;
			break;
		default:
			break;
		}
	}
	else {
		switch (Type)
		{
		case AA_FILTER_BOX:
			m_currFilter5X5 = AntiAliasing::BoxFilter5X5;
			break;
		case AA_FILTER_TRIANGLE:
			m_currFilter5X5 = AntiAliasing::TriangleFilter5X5;
			break;
		case AA_FILTER_GAUSSIAN:
			m_currFilter5X5 = AntiAliasing::GaussianFilter5X5;
			break;
		case AA_FILTER_SINC:
			m_currFilter5X5 = AntiAliasing::SincFilter5X5;
			break;
		default:
			break;
		}
	}
}


