#include "Vector3.h"
#include <cmath>

Vector3::Vector3() {
	x = 0.1f;
	y = 0.2f;
	z = 0.3f;
}

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length() const { return sqrt(x * x + y * y + z * z); }

Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }

Vector3 Vector3::cross(const Vector3& v) const {

	Vector3 a;
	a.x = this->y * v.z - this->z * v.y;
	a.y = this->z * v.x - this->x * v.z;
	a.z = this->x * v.y - this->y * v.x;

	return a;
}

Vector3 Vector3::mat(Vector3 vec, Matrix4 m) {

	this->x = m.m[0][0] * vec.x;
	this->x += m.m[1][0] * vec.y;
	this->x += m.m[2][0] * vec.z;

	this->y = m.m[0][1] * vec.x;
	this->y += m.m[1][1] * vec.y;
	this->y += m.m[2][1] * vec.z;

	this->z = m.m[0][2] * vec.x;
	this->z += m.m[1][2] * vec.y;
	this->z += m.m[2][2] * vec.z;

	return *this;
}

Vector3 Vector3::operator+() const { return *this; }

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

// vectorƒNƒ‰ƒX‚É‘®‚³‚È‚¢
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3 v, float s) {
	Vector3 temp(v);
	return v * s;
}

const Vector3 operator*(float s, const Vector3& v) { return v * s; }

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	temp.x = temp.x / s;
	temp.y = temp.y / s;
	temp.z = temp.z / s;
	return temp;
}