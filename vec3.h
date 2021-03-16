#ifndef VEC3_H
#define VEC3_H
#include <cmath>
#include <iostream>
#include "rtweekend.h"
using std::sqrt;

//this class vec3 defines vector of length 3 and some  basic operations
class vec3
{
public:
    vec3() : e{0, 0, 0} {};
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {};
    vec3(double min, double max){
        for (int i=0;i<3;i++){
            e[i] = random_double(min,max);
        }
    };
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }
    vec3 &operator+=(const vec3 &v)
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
    double length() const
    {
        return sqrt(length_squared());
    }
    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

public:
    double e[3];
};
// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color


// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline bool operator==(const vec3 &a, const vec3 &b)
{
    if (a.x() == b.x() && a.y() == b.y() && a.z() == b.z()){
        return true;
    }
    else{
        return false;
    }
}

inline vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}
inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}
#endif