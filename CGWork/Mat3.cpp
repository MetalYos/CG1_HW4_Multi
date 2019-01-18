#include "Mat3.h"
#include <assert.h>

// Constructors
Mat3::Mat3(double d)
{
	data = new double*[3];
	data[0] = new double[3] { d, 0.0, 0.0 };
	data[1] = new double[3] { 0.0, d, 0.0 };
	data[2] = new double[3] { 0.0, 0.0, d };
}

Mat3::Mat3(double m00, double m01, double m02,
	double m10, double m11, double m12,
	double m20, double m21, double m22)
{
	data = new double*[3];
	data[0] = new double[3] { m00, m01, m02 };
	data[1] = new double[3] { m10, m11, m12 };
	data[2] = new double[3] { m20, m21, m22 };
}

Mat3::~Mat3()
{
	delete[] data[0];
	delete[] data[1];
	delete[] data[2];
	delete[] data;
}

// Copy Constructor
Mat3::Mat3(const Mat3 & m)
{
	data = new double*[3];
	for (int i = 0; i < 3; i++)
	{
		data[i] = new double[3];
		for (int j = 0; j < 3; j++)
		{
			data[i][j] = m.data[i][j];
		}
	}
}

// Assignment operator overloading
Mat3 & Mat3::operator=(const Mat3 & m)
{
	if (this == &m)
		return *this;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			data[i][j] = m.data[i][j];
		}
	}
	return *this;
}

// Addition operator overloading
Mat3 Mat3::operator+(const Mat3 & m) const
{
	Mat3 result;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result.data[i][j] = data[i][j] + m.data[i][j];
		}
	}
	return result;
}

Mat3 & Mat3::operator+=(const Mat3 & m)
{
	return (*this = *this + m);
}

// Subtraction operator overloading
Mat3 Mat3::operator-(const Mat3 & m) const
{
	Mat3 result;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result.data[i][j] = data[i][j] - m.data[i][j];
		}
	}
	return result;
}

Mat3& Mat3::operator-=(const Mat3 & m)
{
	return (*this = *this - m);
}

// Multiplication operator overloading
Mat3 Mat3::operator*(double c) const
{
	Mat3 result;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result.data[i][j] = data[i][j] * c;
		}
	}
	return result;
}

Mat3 & Mat3::operator*=(double c)
{
	return (*this = *this * c);
}

Mat3 Mat3::operator*(const Mat3 & m) const
{
	Mat3 result(0.0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result.data[i][j] = data[i][j] * m.data[i][j];
		}
	}
	return result;
}

Mat3& Mat3::operator*=(const Mat3 & m)
{
	return (*this = *this * m);
}

// Division operator overloading
Mat3 Mat3::operator/(double c) const
{
	assert(c != 0);
	return ((*this) * (1 / c));
}

Mat3 & Mat3::operator/=(double c)
{
	assert(c != 0);
	return (*this = *this / c);
}

// Subscript operator overloading
const double* Mat3::operator[](int i) const
{
	assert(i >= 0 && i < 3);
	return data[i];
}

double* Mat3::operator[](int i)
{
	assert(i >= 0 && i < 3);
	return data[i];
}

// Cout overloading
std::ostream & operator<<(std::ostream& os, const Mat3& v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			os << v.data[i][j] << " ";
		}
		os << std::endl;
	}
	return os;
}
