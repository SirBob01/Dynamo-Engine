#include "vec2d.h"

float Physics::Mat2D::determinant() const {
	return a * d - b * c;
}

const Physics::Mat2D Physics::Mat2D::operator+(const Physics::Mat2D &rhs) const {
	return {a+rhs.a, b+rhs.b, 
			c+rhs.c, d+rhs.d};
}

const Physics::Mat2D Physics::Mat2D::operator-(const Physics::Mat2D &rhs) const {
	return {a-rhs.a, b-rhs.b, 
			c-rhs.c, d-rhs.d};
}

const Physics::Mat2D Physics::Mat2D::operator*(const Physics::Mat2D &rhs) const {
	return {a*rhs.a + b*rhs.c, a*rhs.b + b*rhs.d,
			c*rhs.a + d*rhs.c, c*rhs.b + d*rhs.d};
}

const Physics::Mat2D Physics::Mat2D::operator*(float scalar) const {
	return {a * scalar, b * scalar,
			c * scalar, d * scalar};
}

const Physics::Mat2D Physics::Mat2D::operator/(float scalar) const {
	return {a / scalar, b / scalar,
			c / scalar, d / scalar};
}

const Physics::Mat2D Physics::Mat2D::operator-() const {
	return {-a, -b, -c, -d};
};

const Physics::Mat2D Physics::Mat2D::inverse() const {
	Mat2D inv = {d, -b, -c, a};
	return inv / determinant();
}

Physics::Mat2D &Physics::Mat2D::operator+=(const Physics::Mat2D &rhs) {
	a += rhs.a;
	b += rhs.b;
	c += rhs.c;
	d += rhs.d;
	return *this;
};

Physics::Mat2D &Physics::Mat2D::operator-=(const Physics::Mat2D &rhs) {
	a -= rhs.a;
	b -= rhs.b;
	c -= rhs.c;
	d -= rhs.d;
	return *this;
};

Physics::Mat2D &Physics::Mat2D::operator*=(const Physics::Mat2D &rhs) {
	float new_a = a*rhs.a + b*rhs.c;
	float new_b = a*rhs.b + b*rhs.d;
	float new_c = c*rhs.a + d*rhs.c;
	float new_d = c*rhs.b + d*rhs.d;
	a = new_a;
	b = new_b;
	c = new_c;
	d = new_d;
	return *this;
};

Physics::Mat2D &Physics::Mat2D::operator*=(float scalar) {
	a *= scalar;
	b *= scalar;
	c *= scalar;
	d *= scalar;
	return *this;
}

Physics::Mat2D &Physics::Mat2D::operator/=(float scalar) {
	a /= scalar;
	b /= scalar;
	c /= scalar;
	d /= scalar;
	return *this;
}

bool Physics::Mat2D::operator==(const Physics::Mat2D &rhs) const {
	return a == rhs.a && b == rhs.b && c == rhs.c && d == rhs.d;
}

bool Physics::Mat2D::operator!=(const Physics::Mat2D &rhs) const {
	return a != rhs.a || b != rhs.b || c != rhs.c || d != rhs.d;	
}

float Physics::Vec2D::length_squared() {
	return pow(x, 2) + pow(y, 2);
}

float Physics::Vec2D::length() {
	return sqrt(length_squared());
}

const Physics::Vec2D Physics::Vec2D::left_normal() {
	return {-y, x};
}

const Physics::Vec2D Physics::Vec2D::right_normal() {
	return {y, -x};
}

const Physics::Vec2D Physics::Vec2D::operator+(const Physics::Vec2D &rhs) const {
	return {x + rhs.x, y + rhs.y};
}

const Physics::Vec2D Physics::Vec2D::operator-(const Physics::Vec2D &rhs) const {
	return {x - rhs.x, y - rhs.y};
}

const Physics::Vec2D Physics::Vec2D::operator*(float scalar) const {
	return {scalar * x, scalar * y};
}

const Physics::Vec2D Physics::Vec2D::operator/(float scalar) const {
	return {x / scalar, y / scalar};
}

const Physics::Vec2D Physics::Vec2D::operator-() const {
	return {-x, -y};
}

Physics::Vec2D &Physics::Vec2D::operator+=(const Physics::Vec2D &rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Physics::Vec2D &Physics::Vec2D::operator-=(const Physics::Vec2D &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Physics::Vec2D &Physics::Vec2D::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Physics::Vec2D &Physics::Vec2D::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

Physics::Vec2D &Physics::Vec2D::normalize() {
	float mag = length();
	if(mag != 0.0) {
		x /= mag;
		y /= mag;
	}
	return *this;
}

Physics::Vec2D &Physics::Vec2D::transform(const Physics::Mat2D &rhs) {
	float new_x = rhs.a*x + rhs.b*y;
	float new_y = rhs.c*x + rhs.d*y;
	x = new_x;
	y = new_y;
	return *this;
}

float Physics::Vec2D::operator*(const Physics::Vec2D &rhs) const {
	return x * rhs.x + y * rhs.y;
}

float Physics::Vec2D::cross(const Physics::Vec2D &rhs) const {
	return x * rhs.y - y * rhs.x;
}

bool Physics::Vec2D::operator==(const Physics::Vec2D &rhs) const {
	return (x == rhs.x && y == rhs.y);
}

bool Physics::Vec2D::operator!=(const Physics::Vec2D &rhs) const {
	return (x != rhs.x || y != rhs.y);
}