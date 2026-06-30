#pragma once

#include "../core/Types.hpp"
#include <cmath>
#include <vector>

namespace phageforge::physics {

/**
 * @brief Vector math utilities for 3D physics calculations
 * 
 * All functions are inline for performance
 */

// --- Basic Operations ---

[[nodiscard]] inline float distance(const core::Point3D& a, const core::Point3D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

[[nodiscard]] inline float squaredDistance(const core::Point3D& a, const core::Point3D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

[[nodiscard]] inline float dotProduct(const core::Point3D& a, const core::Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

[[nodiscard]] inline core::Point3D crossProduct(const core::Point3D& a, const core::Point3D& b) {
    return core::Point3D{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

[[nodiscard]] inline float magnitude(const core::Point3D& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

[[nodiscard]] inline core::Point3D normalize(const core::Point3D& v) {
    float mag = magnitude(v);
    if (mag < 1e-10f) {
        return core::Point3D{0.0f, 0.0f, 0.0f};
    }
    return core::Point3D{v.x / mag, v.y / mag, v.z / mag};
}

[[nodiscard]] inline core::Point3D lerp(const core::Point3D& a, const core::Point3D& b, float t) {
    return core::Point3D{
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

[[nodiscard]] inline core::Point3D operator+(const core::Point3D& a, const core::Point3D& b) {
    return core::Point3D{a.x + b.x, a.y + b.y, a.z + b.z};
}

[[nodiscard]] inline core::Point3D operator-(const core::Point3D& a, const core::Point3D& b) {
    return core::Point3D{a.x - b.x, a.y - b.y, a.z - b.z};
}

[[nodiscard]] inline core::Point3D operator*(const core::Point3D& a, float scalar) {
    return core::Point3D{a.x * scalar, a.y * scalar, a.z * scalar};
}

// --- Advanced Operations ---

/**
 * @brief Calculate the angle between two vectors (radians)
 */
[[nodiscard]] inline float angleBetween(const core::Point3D& a, const core::Point3D& b) {
    float dot = dotProduct(a, b);
    float mag_a = magnitude(a);
    float mag_b = magnitude(b);
    if (mag_a < 1e-10f || mag_b < 1e-10f) return 0.0f;
    return std::acos(std::max(-1.0f, std::min(1.0f, dot / (mag_a * mag_b))));
}

/**
 * @brief Rotate a point around an axis
 */
[[nodiscard]] inline core::Point3D rotateAroundAxis(
    const core::Point3D& point,
    const core::Point3D& axis,
    float angle_radians) {
    
    float c = std::cos(angle_radians);
    float s = std::sin(angle_radians);
    float t = 1.0f - c;
    
    core::Point3D axis_norm = normalize(axis);
    
    // Rodrigues' rotation formula
    core::Point3D result;
    result.x = (t * axis_norm.x * axis_norm.x + c) * point.x +
               (t * axis_norm.x * axis_norm.y - s * axis_norm.z) * point.y +
               (t * axis_norm.x * axis_norm.z + s * axis_norm.y) * point.z;
    
    result.y = (t * axis_norm.x * axis_norm.y + s * axis_norm.z) * point.x +
               (t * axis_norm.y * axis_norm.y + c) * point.y +
               (t * axis_norm.y * axis_norm.z - s * axis_norm.x) * point.z;
    
    result.z = (t * axis_norm.x * axis_norm.z - s * axis_norm.y) * point.x +
               (t * axis_norm.y * axis_norm.z + s * axis_norm.x) * point.y +
               (t * axis_norm.z * axis_norm.z + c) * point.z;
    
    return result;
}

/**
 * @brief Create a helix path for phage tail fiber
 */
[[nodiscard]] inline std::vector<core::Point3D> createHelix(
    float radius,
    float pitch,
    int num_points,
    float start_angle = 0.0f) {
    
    std::vector<core::Point3D> points;
    points.reserve(num_points);
    
    for (int i = 0; i < num_points; ++i) {
        float t = static_cast<float>(i) / num_points;
        float angle = start_angle + t * 2.0f * 3.14159f * 3.0f; // 3 turns
        float z = t * pitch;
        
        points.push_back({
            radius * std::cos(angle),
            radius * std::sin(angle),
            z
        });
    }
    
    return points;
}

} // namespace phageforge::physics
