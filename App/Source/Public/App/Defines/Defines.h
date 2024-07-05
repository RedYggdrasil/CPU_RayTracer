#pragma once

#define WITH_REFERENCE 0
#define USE_DOUBLE_PRECISION 0

#if !WITH_REFERENCE
#define USE_DOUBLE_PRECISION 0
#endif


#define USE_SCHLICK_REFLECTANCE 1

#include <type_traits>
#include <cmath>
#include <limits>
#include <DirectXMath.h>
#include <iostream>

using XMVECTOR1 = DirectX::XMVECTOR;
using XMVECTOR2 = DirectX::XMVECTOR;
using XMVECTOR3 = DirectX::XMVECTOR;
using XMVECTOR4 = DirectX::XMVECTOR;


#define R_VIRTUAL_IMPLICIT 
#define R_OVERRIDE_IMPLICIT 
#define R_PURE = 0
#define R_PURE_OVERRIDE override

#define STATIC_CLASS_R5(ClassName)\
public:\
ClassName() = delete;\
ClassName(const ClassName& _) = delete;\
ClassName& operator=(const ClassName& other) = delete;\
ClassName(ClassName&& other) noexcept = delete;\
ClassName& operator=(ClassName&& other) noexcept = delete;

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

#if WITH_REFERENCE
#include "App/Debug/DebugReference.h"
#endif