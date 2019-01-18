#include "Mat5.h"
#include <assert.h>

// Constructors
Mat5::Mat5(double d)
{
	data = new double*[5];
	data[0] = new double[5]{ d, 0.0, 0.0, 0.0, 0.0 };
	data[1] = new double[5]{ 0.0, d, 0.0, 0.0, 0.0 };
	data[2] = new double[5]{ 0.0, 0.0, d, 0.0, 0.0 };
	data[3] = new double[5]{ 0.0, 0.0, 0.0, d, 0.0 };
	data[4] = new double[5]{ 0.0, 0.0, 0.0, 0.0, d };
}

Mat5::Mat5(double m00, double m01, double m02, double m03, double m04,
		   double m10, double m11, double m12, double m13, double m14,
		   double m20, double m21, double m22, double m23, double m24,
		   double m30, double m31, double m32, double m33, double m34,
		   double m40, double m41, double m42, double m43, double m44)
{
	data = new double*[5];
	data[0] = new double[5]{ m00, m01, m02, m03, m04 };
	data[1] = new double[5]{ m10, m11, m12, m13, m14 };
	data[2] = new double[5]{ m20, m21, m22, m23, m24 };
	data[3] = new double[5]{ m30, m31, m32, m33, m34 };
	data[4] = new double[5]{ m40, m41, m42, m43, m44 };
}

Mat5::~Mat5()
{
	delete[] data[0];
	delete[] data[1];
	delete[] data[2];
	delete[] data[3];
	delete[] data[4];
	delete[] data;
}

// Copy Constructor
Mat5::Mat5(const Mat5 & m)
{
	data = new double*[5];
	for (int i = 0; i < 5; i++)
	{
		data[i] = new double[5];
		for (int j = 0; j < 5; j++)
		{
			data[i][j] = m.data[i][j];
		}
	}
}

// Assignment operator overloading
Mat5 & Mat5::operator=(const Mat5 & m)
{
	if (this == &m)
		return *this;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			data[i][j] = m.data[i][j];
		}
	}
	return *this;
}

// Addition operator overloading
Mat5 Mat5::operator+(const Mat5 & m) const
{
	Mat5 result;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			result.data[i][j] = data[i][j] + m.data[i][j];
		}
	}
	return result;
}

Mat5 & Mat5::operator+=(const Mat5 & m)
{
	return (*this = *this + m);
}

// Subtraction operator overloading
Mat5 Mat5::operator-(const Mat5 & m) const
{
	Mat5 result;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			result.data[i][j] = data[i][j] - m.data[i][j];
		}
	}
	return result;
}

Mat5& Mat5::operator-=(const Mat5 & m)
{
	return (*this = *this - m);
}

// Multiplication operator overloading
Mat5 Mat5::operator*(double c) const
{
	Mat5 result;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			result.data[i][j] = data[i][j] * c;
		}
	}
	return result;
}

Mat5 & Mat5::operator*=(double c)
{
	return (*this = *this * c);
}

Mat5 Mat5::operator*(const Mat5 & m) const
{
	Mat5 result(0.0);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			result.data[i][j] = data[i][j] * m.data[i][j];
		}
	}
	return result;
}

Mat5& Mat5::operator*=(const Mat5 & m)
{
	return (*this = *this * m);
}

// Division operator overloading
Mat5 Mat5::operator/(double c) const
{
	assert(c != 0);
	return ((*this) * (1 / c));
}

Mat5 & Mat5::operator/=(double c)
{
	assert(c != 0);
	return (*this = *this / c);
}

// Subscript operator overloading
const double* Mat5::operator[](int i) const
{
	assert(i >= 0 && i < 5);
	return data[i];
}

double* Mat5::operator[](int i)
{
	assert(i >= 0 && i < 5);
	return data[i];
}

// Cout overloading
std::ostream & operator<<(std::ostream& os, const Mat5& v)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			os << v.data[i][j] << " ";
		}
		os << std::endl;
	}
	return os;
}
