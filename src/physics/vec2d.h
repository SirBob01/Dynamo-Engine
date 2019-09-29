#ifndef DYNAMO_PHYSICS_VEC2D_H_
#define DYNAMO_PHYSICS_VEC2D_H_

#include <math.h>

namespace Physics {
	struct Mat2D {
		/*
		[a, b
		 c, d]
		*/
		float a, b;
		float c, d;

		float determinant() const;

		const Mat2D operator+(const Mat2D &rhs) const;
		const Mat2D operator-(const Mat2D &rhs) const;
		const Mat2D operator*(const Mat2D &rhs) const;
		const Mat2D operator*(float scalar) const;
		const Mat2D operator/(float scalar) const;
		const Mat2D operator-() const;

		const Mat2D inverse() const;

		Mat2D &operator+=(const Mat2D &rhs);
		Mat2D &operator-=(const Mat2D &rhs);
		Mat2D &operator*=(const Mat2D &rhs);
		Mat2D &operator*=(float scalar);
		Mat2D &operator/=(float scalar);

		bool operator==(const Mat2D &rhs) const;
		bool operator!=(const Mat2D &rhs) const;
	};

	struct Vec2D {
		float x;
		float y;

		float length_squared();
		float length();

		const Vec2D left_normal();
		const Vec2D right_normal();

		const Vec2D operator+(const Vec2D &rhs) const;
		const Vec2D operator-(const Vec2D &rhs) const;
		const Vec2D operator*(float scalar) const;
		const Vec2D operator/(float scalar) const;
		const Vec2D operator-() const; // Negation

		Vec2D &operator+=(const Vec2D &rhs);
		Vec2D &operator-=(const Vec2D &rhs);
		Vec2D &operator*=(float scalar);
		Vec2D &operator/=(float scalar);

		Vec2D &normalize();
		Vec2D &transform(const Mat2D &mat);

		float operator*(const Vec2D &rhs) const; // Dot product
		float cross(const Vec2D &rhs) const; // "Cross-product"

		bool operator==(const Vec2D &rhs) const;
		bool operator!=(const Vec2D &rhs) const;
	};
}

#endif