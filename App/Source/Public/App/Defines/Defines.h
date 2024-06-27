#pragma once

#include <type_traits>
#include <cmath>
#include <limits>
#include <DirectXMath.h>

using XMVECTOR1 = DirectX::XMVECTOR;
using XMVECTOR2 = DirectX::XMVECTOR;
using XMVECTOR3 = DirectX::XMVECTOR;
using XMVECTOR4 = DirectX::XMVECTOR;


#define R_VIRTUAL_IMPLICIT 
#define R_OVERRIDE_IMPLICIT 
#define R_PURE = 0
#define R_PURE_OVERRIDE override

template<typename T>
using UT = std::underlying_type<T>::type;

template <typename T>
constexpr typename std::underlying_type<T>::type UT_cast(T e) noexcept {
	return static_cast<typename std::underlying_type<T>::type>(e);
}

#define R_SQRTF(InValue)  std::sqrtf(InValue)

constexpr float R_INFINITY_F = std::numeric_limits<float>::infinity();
constexpr float R_PI_F = DirectX::XM_PI;

inline constexpr float Deg2Rad(const float InDegree) noexcept
{
	return InDegree * R_PI_F / 180.f;
}