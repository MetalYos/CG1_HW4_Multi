#pragma once

#include <iostream>

class Mat5
{
private:
	double** data;

public:
	// Constructors
	explicit Mat5(double d = 1.0);
	Mat5(double m00, double m01, double m02, double m03, double m04,
		 double m10, double m11, double m12, double m13, double m14,
		 double m20, double m21, double m22, double m23, double m24,
		 double m30, double m31, double m32, double m33, double m34,
		 double m40, double m41, double m42, double m43, double m44);

	// Destructor
	~Mat5();

	// Copy Constructor
	Mat5(const Mat5& m);

	// Assignment operator overloading
	Mat5& operator =(const Mat5& m);

	// Addition operator overloading
	Mat5 operator +(const Mat5& m) const;
	Mat5& operator +=(const Mat5& m);

	// Subtraction operator overloading
	Mat5 operator -(const Mat5& m) const;
	Mat5& operator -=(const Mat5& m);

	//Scalar Multiplication operator overloading
	Mat5 operator *(double c) const;
	Mat5& operator *=(double c);

	// Element-Element Multiplication operator overloading
	Mat5 operator *(const Mat5& m) const;
	Mat5& operator *=(const Mat5& m);

	// Division operator overloading
	Mat5 operator /(double c) const;
	Mat5& operator /=(double c);

	// Subscript operator overloading
	const double* operator [](int i) const;
	double* operator [](int i);

	// Cout overloading
	friend std::ostream& operator<<(std::ostream& os, const Mat5& v);
};
