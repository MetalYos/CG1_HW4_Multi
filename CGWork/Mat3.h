#pragma once

#include <iostream>

class Mat3
{
private:
	double** data;

public:
	// Constructors
	explicit Mat3(double d = 1.0);
	Mat3(double m00, double m01, double m02,
		double m10, double m11, double m12,
		double m20, double m21, double m22);

	// Destructor
	~Mat3();

	// Copy Constructor
	Mat3(const Mat3& m);

	// Assignment operator overloading
	Mat3& operator =(const Mat3& m);

	// Addition operator overloading
	Mat3 operator +(const Mat3& m) const;
	Mat3& operator +=(const Mat3& m);

	// Subtraction operator overloading
	Mat3 operator -(const Mat3& m) const;
	Mat3& operator -=(const Mat3& m);
	
	//Scalar Multiplication operator overloading
	Mat3 operator *(double c) const;
	Mat3& operator *=(double c);

	// Element-Element Multiplication operator overloading
	Mat3 operator *(const Mat3& m) const;
	Mat3& operator *=(const Mat3& m);

	// Division operator overloading
	Mat3 operator /(double c) const;
	Mat3& operator /=(double c);

	// Subscript operator overloading
	const double* operator [](int i) const;
	double* operator [](int i);

	// Cout overloading
	friend std::ostream& operator<<(std::ostream& os, const Mat3& v);
};

