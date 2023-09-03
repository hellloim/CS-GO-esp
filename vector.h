#pragma once
#include <numbers>
#include <cmath>

struct Vector3
{
    float x, y, z;

    // Constructors
    Vector3() noexcept : x(0.f), y(0.f), z(0.f) {}
    Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

    // Operator Overloads
    Vector3& operator+=(const Vector3& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    const Vector3 operator-(const Vector3& other) const noexcept
    {
        return Vector3{ x - other.x, y - other.y, z - other.z };
    }

    const Vector3 operator+(const Vector3& other) const noexcept
    {
        return Vector3{ x + other.x, y + other.y, z + other.z };
    }

    const Vector3 operator/(const float factor) const noexcept
    {
        return Vector3{ x / factor, y / factor, z / factor };
    }

    const Vector3 operator*(const float factor) const noexcept
    {
        return Vector3{ x * factor, y * factor, z * factor };
    }

    // Utils
    const Vector3 ToAngle() const noexcept
    {
        return Vector3{
            std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
            std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
            0.0f
        };
    }

    const bool IsZero() const noexcept
    {
        return x == 0.f && y == 0.f && z == 0.f;
    }
};
