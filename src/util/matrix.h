#ifndef DYNAMO_MATRIX_H_
#define DYNAMO_MATRIX_H_

namespace Dynamo {
    struct Mat2D {
        /*
        [a, b
         c, d]
        */
        float a, b;
        float c, d;

        float determinant() const;

        Mat2D operator+(const Mat2D &rhs) const;
        Mat2D operator-(const Mat2D &rhs) const;
        Mat2D operator*(const Mat2D &rhs) const;
        Mat2D operator*(float scalar) const;
        Mat2D operator/(float scalar) const;
        Mat2D operator-() const;

        Mat2D inverse() const;

        Mat2D &operator+=(const Mat2D &rhs);
        Mat2D &operator-=(const Mat2D &rhs);
        Mat2D &operator*=(const Mat2D &rhs);
        Mat2D &operator*=(float scalar);
        Mat2D &operator/=(float scalar);

        bool operator==(const Mat2D &rhs) const;
        bool operator!=(const Mat2D &rhs) const;
    };
}

#endif