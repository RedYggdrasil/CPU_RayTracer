#pragma once

#ifdef _DEBUG
#include "App/pch.h"

/// <summary>
/// Colum major matrix to cartesian coordiante
/// From (X Forward, Y Right, Z Up) to (X Right, Y Up, Z Backward)
/// </summary>
inline constexpr DirectX::XMFLOAT4X4 ToRightHandedCartesianCoordinate =
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
inline const DirectX::XMMATRIX lToRightHandedCartesianCoordinate = DirectX::XMLoadFloat4x4(&ToRightHandedCartesianCoordinate);

#endif // DEBUG
