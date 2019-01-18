#pragma once

#include "ALMath.h"
#include "stdafx.h"

enum AAFilterType
{
	AA_FILTER_BOX,
	AA_FILTER_TRIANGLE,
	AA_FILTER_GAUSSIAN,
	AA_FILTER_SINC
};

enum AAKernelSize
{
	AA_3X3,
	AA_5X5
};

class AntiAliasing
{
public:
	bool IsEnabled;
	AAKernelSize Size;
	AAFilterType Type;

	static Mat3 BoxFilter3X3;
	static Mat3 TriangleFilter3X3;
	static Mat3 GaussianFilter3X3;
	static Mat3 SincFilter3X3;

	static Mat5 BoxFilter5X5;
	static Mat5 TriangleFilter5X5;
	static Mat5 GaussianFilter5X5;
	static Mat5 SincFilter5X5;
private:
	int m_sizeFactor;
	int m_bigWidth;
	Mat3 m_currFilter3X3;
	Mat5 m_currFilter5X5;

public:
	AntiAliasing()
		: IsEnabled(false), Size(AA_3X3), m_sizeFactor(3), m_bigWidth(0), Type(AA_FILTER_BOX) {}
	BUFFER PrepareForPrint(BUFFER ssFB, int width, int height);
private:
	COLORREF GetColor(BUFFER ssFB, int i, int j, int width);
	void SetMatrix();
};
