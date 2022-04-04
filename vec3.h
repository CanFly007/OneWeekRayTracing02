#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include<iostream>

class vec3
{
public:
	vec3() :e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) :e{ e0,e1,e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int index) const { return e[index]; }
	double& operator[](int index) { return e[index]; }

	vec3& operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3& operator*=(const double t) 
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}
	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	double length()const
	{
		return sqrt(length_squared());
	}
	void write_color(std::ostream& out)
	{
		out << static_cast<int>(255.999 * e[0]) << ' '
			<< static_cast<int>(255.999 * e[1]) << ' '
			<< static_cast<int>(255.999 * e[2]) << '\n';
	}
public:
	double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}
inline vec3 operator/(vec3 v, double t)
{
	return (1 / t) * v;
}
inline vec3 operator+(vec3 v, double i)
{
	return vec3(v.e[0] + i, v.e[1] + i, v.e[2] + i);
}

inline double dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

vec3 random_unit_vector()//选取球面上一点
{
	double a = random_double(0, 2 * pi);
	double z = random_double(-1, 1);
	double r = sqrt(1 - z * z);//xy平面圆的半径
	return vec3(cos(a) * r, sin(a) * r, z);
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}
#endif
