/****************************************************************************
Copyright (c) Chukong Technologies Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef VECTOR4_H_
#define VECTOR4_H_
#include <iostream>

namespace cocos3d
{
class C3DMatrix;

/**
 * Defines 4-element floating point vector.
 */
class  C3DVector4
{
public:

    /**
     * The x-coordinate.
     */
    float x;

    /**
     * The y-coordinate.
     */
    float y;

    /**
     * The z-coordinate.
     */
    float z;

    /**
     * The w-coordinate.
     */
    float w;

    /**
     * Constructs a new vector initialized to all zeros.
     */
    C3DVector4();

    /**
     * Constructs a new vector initialized to the specified values.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     * @param w The w coordinate.
     */
    C3DVector4(float x, float y, float z, float w);

    /**
     * Constructs a new vector from the values in the specified array.
     *
     * @param array An array containing the elements of the vector in the order x, y, z, w.
     */
    C3DVector4(const float* array);

    /**
     * Constructs a vector that describes the direction between the specified points.
     *
     * @param p1 The first point.
     * @param p2 The second point.
     */
    C3DVector4(const C3DVector4& p1, const C3DVector4& p2);

    /**
     * Constructor.
     *
     * Creates a new vector that is a copy of the specified vector.
     *
     * @param copy The vector to copy.
     */
    C3DVector4(const C3DVector4& copy);

    /**
     * Creates a new vector from an integer interpreted as an RGBA value.
     * E.g. 0xff0000ff represents opaque red or the vector (1, 0, 0, 1).
     *
     * @param color The integer to interpret as an RGBA value.
     *
     * @return A vector corresponding to the interpreted RGBA color.
     */
    static C3DVector4 fromColor(unsigned int color);

    /**
     * Destructor.
     */
    ~C3DVector4();

    /**
     * Returns the zero vector.
     *
     * @return The 4-element vector of 0s.
     */
    static const C3DVector4& zero();

    /**
     * Returns the one vector.
     *
     * @return The 4-element vector of 1s.
     */
    static const C3DVector4& one();

    /**
     * Returns the unit x vector.
     *
     * @return The 4-element unit vector along the x axis.
     */
    static const C3DVector4& unitX();

    /**
     * Returns the unit y vector.
     *
     * @return The 4-element unit vector along the y axis.
     */
    static const C3DVector4& unitY();

    /**
     * Returns the unit z vector.
     *
     * @return The 4-element unit vector along the z axis.
     */
    static const C3DVector4& unitZ();

    /**
     * Returns the unit w vector.
     *
     * @return The 4-element unit vector along the w axis.
     */
    static const C3DVector4& unitW();

    /**
     * Indicates whether this vector contains all zeros.
     *
     * @return true if this vector contains all zeros, false otherwise.
     */
    bool isZero() const;

    /**
     * Indicates whether this vector contains all ones.
     *
     * @return true if this vector contains all ones, false otherwise.
     */
    bool isOne() const;

    /**
     * Returns the angle (in radians) between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     *
     * @return The angle between the two vectors (in radians).
     */
    static float angle(const C3DVector4& v1, const C3DVector4& v2);

    /**
     * Adds the elements of the specified vector to this one.
     *
     * @param v The vector to add.
     */
    inline void add(const C3DVector4& v);

    /**
     * Adds the specified vectors and stores the result in dst.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst A vector to store the result in.
     */
    static void add(const C3DVector4& v1, const C3DVector4& v2, C3DVector4* dst);

    /**
     * Clamps this vector within the specified range.
     *
     * @param min The minimum value.
     * @param max The maximum value.
     */
    void clamp(const C3DVector4& min, const C3DVector4& max);

    /**
     * Clamps the specified vector within the specified range and returns it in dst.
     *
     * @param v The vector to clamp.
     * @param min The minimum value.
     * @param max The maximum value.
     * @param dst A vector to store the result in.
     */
    static void clamp(const C3DVector4& v, const C3DVector4& min, const C3DVector4& max, C3DVector4* dst);

    /**
     * Returns the distance between this vector and v.
     *
     * @param v The other vector.
     *
     * @return The distance between this vector and v.
     *
     * @see distanceSquared
     */
    float distance(const C3DVector4& v) const;

    /**
     * Returns the squared distance between this vector and v.
     *
     * When it is not neccessary to get the exact distance between
     * two vectors (for example, when simply comparing the
     * distance between different vectors), it is advised to use
     * this method instead of distance.
     *
     * @param v The other vector.
     *
     * @return The squared distance between this vector and v.
     *
     * @see distance
     */
    float distanceSquared(const C3DVector4& v) const;

    /**
     * Returns the dot product of this vector and the specified vector.
     *
     * @param v The vector to compute the dot product with.
     *
     * @return The dot product.
     */
    float dot(const C3DVector4& v) const;

    /**
     * Returns the dot product between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     *
     * @return The dot product between the vectors.
     */
    static float dot(const C3DVector4& v1, const C3DVector4& v2);

    /**
     * Computes the length of this vector.
     *
     * @return The length of the vector.
     *
     * @see lengthSquared
     */
    float length() const;

    /**
     * Returns the squared length of this vector.
     *
     * When it is not neccessary to get the exact length of a
     * vector (for example, when simply comparing the lengths of
     * different vectors), it is advised to use this method
     * instead of length.
     *
     * @return The squared length of the vector.
     *
     * @see length
     */
    float lengthSquared() const;

    /**
     * Negates this vector.
     */
    void negate();

    /**
     * Normalizes this vector.
     *
     * This method normalizes this C3DVector4 so that it is of
     * unit length (in other words, the length of the vector
     * after calling this method will be 1.0f). If the vector
     * already has unit length or if the length of the vector
     * is zero, this method does nothing.
     *
     * @return This vector, after the normalization occurs.
     */
    C3DVector4& normalize();

    /**
     * Normalizes this vector and stores the result in dst.
     *
     * If the vector already has unit length or if the length
     * of the vector is zero, this method simply copies the
     * current vector into dst.
     *
     * @param dst The destination vector.
     */
    void normalize(C3DVector4* dst);

    /**
     * Scales all elements of this vector by the specified value.
     *
     * @param scalar The scalar value.
     */
    void scale(float scalar);

    /**
     * Sets the elements of this vector to the specified values.
     *
     * @param x The new x coordinate.
     * @param y The new y coordinate.
     * @param z The new z coordinate.
     * @param w The new w coordinate.
     */
    void set(float x, float y, float z, float w);

    /**
     * Sets the elements of this vector from the values in the specified array.
     *
     * @param array An array containing the elements of the vector in the order x, y, z, w.
     */
    void set(const float* array);

    /**
     * Sets the elements of this vector to those in the specified vector.
     *
     * @param v The vector to copy.
     */
    void set(const C3DVector4& v);

    /**
     * Sets this vector to the directional vector between the specified points.
     *
     * @param p1 The first point.
     * @param p2 The second point.
     */
    void set(const C3DVector4& p1, const C3DVector4& p2);

    /**
     * Subtracts this vector and the specified vector as (this - v)
     * and stores the result in this vector.
     *
     * @param v The vector to subtract.
     */
    void subtract(const C3DVector4& v);

    /**
     * Subtracts the specified vectors and stores the result in dst.
     * The resulting vector is computed as (v1 - v2).
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst The destination vector.
     */
    static void subtract(const C3DVector4& v1, const C3DVector4& v2, C3DVector4* dst);

    /**
     * Calculates the sum of this vector with the given vector.
     *
     * Note: this does not modify this vector.
     *
     * @param v The vector to add.
     * @return The vector sum.
     */
    inline const C3DVector4 operator+(const C3DVector4& v) const;

    /**
     * Adds the given vector to this vector.
     *
     * @param v The vector to add.
     * @return This vector, after the addition occurs.
     */
    inline C3DVector4& operator+=(const C3DVector4& v);

    /**
     * Calculates the sum of this vector with the given vector.
     *
     * Note: this does not modify this vector.
     *
     * @param v The vector to add.
     * @return The vector sum.
     */
    inline const C3DVector4 operator-(const C3DVector4& v) const;

    /**
     * Subtracts the given vector from this vector.
     *
     * @param v The vector to subtract.
     * @return This vector, after the subtraction occurs.
     */
    inline C3DVector4& operator-=(const C3DVector4& v);

    /**
     * Calculates the negation of this vector.
     *
     * Note: this does not modify this vector.
     *
     * @return The negation of this vector.
     */
    inline const C3DVector4 operator-() const;

    /**
     * Calculates the scalar product of this vector with the given value.
     *
     * Note: this does not modify this vector.
     *
     * @param x The value to scale by.
     * @return The scaled vector.
     */
    inline const C3DVector4 operator*(float x) const;

	inline const C3DVector4 operator*(const C3DVector4& v) const;

    /**
     * Scales this vector by the given value.
     *
     * @param x The value to scale by.
     * @return This vector, after the scale occurs.
     */
    inline C3DVector4& operator*=(float x);

    /**
     * Determines if this vector is less than the given vector.
     *
     * @param v The vector to compare against.
     *
     * @return True if this vector is less than the given vector, false otherwise.
     */
    inline bool operator<(const C3DVector4& v) const;

    /**
     * Determines if this vector is equal to the given vector.
     *
     * @param v The vector to compare against.
     *
     * @return True if this vector is equal to the given vector, false otherwise.
     */
    inline bool operator==(const C3DVector4& v) const;

    /**
     * Determines if this vector is not equal to the given vector.
     *
     * @param v The vector to compare against.
     *
     * @return True if this vector is not equal to the given vector, false otherwise.
     */
    inline bool operator!=(const C3DVector4& v) const;
};

/**
 * Calculates the scalar product of the given vector with the given value.
 *
 * @param x The value to scale by.
 * @param v The vector to scale.
 * @return The scaled vector.
 */
inline const C3DVector4 operator*(float x, const C3DVector4& v);

inline std::ostream& operator << (std::ostream& stream, const C3DVector4& v)
{
    return stream << v.x << "," << v.y << "," << v.z << "," << v.w;
}

inline std::istream& operator >> (std::istream& stream, C3DVector4& v)
{
    stream >> v.x;
    stream.ignore(256, ',');
    stream >> v.y;
    stream.ignore(256, ',');
    stream >> v.z;
    stream.ignore(256, ',');
    stream >> v.w;
    return stream;
}
}

#include "C3DVector4.inl"

#endif
