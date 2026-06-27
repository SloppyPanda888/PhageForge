#pragma once

#include "../core/Types.hpp"
#include <cmath>

namespace phageforge::physics {

/**
 * @brief Vector math utilities for physics calculations
 * 
 * All functions are inline for performance in physics simulations
 */

// Convert between Point3D and other formats (Phase 3)
// For now, provide utility functions

/**
 * @brief Calculate the distance between two 3D points
 * 
 * @param a First point
 * @param b Second point
 * @return Distance in nm
 */
[[nodiscard]] inline float distance(const core::Point3D& a, const core::Point3D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

/**
 * @brief Calculate the squared distance between two 3D points
 * (Faster than distance() when you only need to compare distances)
 * 
 * @param a First point
 * @param b Second point
 * @return Squared distance in nm^2
 */
[[nodiscard]] inline float squaredDistance(const core::Point3D& a, const core::Point3D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

/**
 * @brief Calculate the dot product of two 3D vectors
 * 
 * @param a First vector
 * @param b Second vector
 * @return Dot product
 */
[[nodiscard]] inline float dotProduct(const core::Point3D& a, const core::Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Calculate the cross product of two 3D vectors
 * 
 * @param a First vector
 * @param b Second vector
 * @return Cross product vector
 */
[[nodiscard]] inline core::Point3D crossProduct(const core::Point3D& a, const core::Point3D& b) {
    return core::Point3D{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

/**
 * @brief Calculate the magnitude (length) of a 3D vector
 * 
 * @param v Vector
 * @return Magnitude
 */
[[nodiscard]] inline float magnitude(const core::Point3D& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/**
 * @brief Normalize a 3D vector (make it unit length)
 * 
 * @param v Vector to normalize
 * @return Normalized vector
 */
[[nodiscard]] inline core::Point3D normalize(const core::Point3D& v) {
    float mag = magnitude(v);
    if (mag < 1e-10f) {
        return core::Point3D{0.0f, 0.0f, 0.0f};
    }
    return core::Point3D{v.x / mag, v.y / mag, v.z / mag};
}

/**
 * @brief Linear interpolation between two points
 * 
 * @param a Start point
 * @param b End point
 * @param t Interpolation factor (0.0 to 1.0)
 * @return Interpolated point
 */
[[nodiscard]] inline core::Point3D lerp(const core::Point3D& a, const core::Point3D& b, float t) {
    return core::Point3D{
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

} // namespace phageforge::physics
