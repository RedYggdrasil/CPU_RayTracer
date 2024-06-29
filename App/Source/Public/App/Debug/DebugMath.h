#pragma once

#include "App/pch.h"
#ifdef _DEBUG

/// <summary>
/// Colum major matrix to cartesian coordiante
/// From (X Forward, Y Right, Z Up) to (X Right, Y Up, Z Backward)
/// </summary>
inline constexpr DirectX::XMFLOAT4X4 MatToRightHandedCartesianCoordinate =
{
    +0.f, +0.f, -1.f, 0.f,
    +1.f, +0.f, +0.f, 0.f,
    +0.f, +1.f, +0.f, 0.f,
    +0.f, +0.f, +0.f, 1.f
};
/// <summary>
/// Colum major matrix to cartesian coordiante
/// From (X Forward, Y Right, Z Up) to (X Right, Y Up, Z Backward)
/// </summary>
inline const DirectX::XMMATRIX lMatToRightHandedCartesianCoordinate = DirectX::XMLoadFloat4x4(&MatToRightHandedCartesianCoordinate);

#endif // DEBUG

inline DirectX::XMVECTOR XM_CALLCONV DEBUG_ToRightHandedCartesianCoordinate(DirectX::FXMVECTOR INVector) noexcept
{
#ifdef _DEBUG
    return DirectX::XMVector3TransformCoord(INVector, lMatToRightHandedCartesianCoordinate);
#else
    return INVector;
#endif // DEBUG
}