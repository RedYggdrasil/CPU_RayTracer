#pragma once
#include "App/pch.h"

#include "App/Maths/RMath.h"
#include "App/Tools/RRenderRandomizer.h"

namespace AppNmsp
{
	struct LocalVectorDistributionUnitSphereDistribution;
	struct LocalVectorDistribution
	{
	public:
		RRenderRandomizer::LocalRealDistribution<float> RealDistribution;
		inline DirectX::XMFLOAT3 GetFLT3()
		{
			return DirectX::XMFLOAT3
			{
				RealDistribution(),
				RealDistribution(),
				RealDistribution()
			};
		};

		inline DirectX::XMFLOAT3 GetFLT3Scaled(const float InMin, const float InMax)
		{
			float diff = InMax - InMin;
			return DirectX::XMFLOAT3
			{
				InMin + RealDistribution() * diff,
				InMin + RealDistribution() * diff,
				InMin + RealDistribution() * diff
			};
		}

		inline DirectX::XMVECTOR XM_CALLCONV GetVEC3()
		{
			return DirectX::XMVECTOR
			{
				RealDistribution(),
				RealDistribution(),
				RealDistribution(),
				0.f
			};
		};
		inline DirectX::XMVECTOR XM_CALLCONV GetVEC3Point()
		{
			return DirectX::XMVECTOR
			{
				RealDistribution(),
				RealDistribution(),
				RealDistribution(),
				1.f
			};
		};

		inline DirectX::XMVECTOR XM_CALLCONV GetVEC3(const float InMin, const float InMax)
		{
			float diff = InMax - InMin;
			return DirectX::XMVECTOR
			{
				InMin + RealDistribution() * diff,
				InMin + RealDistribution() * diff,
				InMin + RealDistribution() * diff,
				0.f
			};
		};
		inline DirectX::XMVECTOR XM_CALLCONV GetVEC3Point(const float InMin, const float InMax)
		{
			float diff = InMax - InMin;
			return DirectX::XMVECTOR
			{
				InMin + RealDistribution() * diff,
				InMin + RealDistribution() * diff,
				InMin + RealDistribution() * diff,
				1.f
			};
		};

	public:
		static LocalVectorDistribution CreateDesyncUnitDistribution();
		static LocalVectorDistribution CreateDesyncRangedDistribution(const float InMin, const float InMax);
	
	protected:
		LocalVectorDistribution(const uint32_t InSeed, const float InMinIncluded, const float InMaxExcluded)
			: RealDistribution(InSeed, InMinIncluded, InMaxExcluded) { }
		LocalVectorDistribution(const uint32_t InSeed, const float InMaxExcluded)
			: RealDistribution(InSeed, InMaxExcluded) { }
		LocalVectorDistribution(const uint32_t InSeed)
			: RealDistribution(InSeed) { }
	};

	struct LocalVectorDistributionUnitSphereDistribution : public LocalVectorDistribution
	{
	public:
		/// <summary>
		/// Need a -1 to 1 distribution, use CreateDesyncUnitSphereDistribution to create the distribution
		/// </summary>
		/// <returns></returns>
		inline DirectX::XMVECTOR XM_CALLCONV RandomInUnitSphere() {
			DirectX::XMVECTOR result;
			while (true) {
				result = this->GetVEC3();
				if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(result)) < 1.f)
					return result;
			}
		}

		inline DirectX::XMVECTOR XM_CALLCONV RandomUnitVector() {
			return DirectX::XMVector3Normalize(RandomInUnitSphere());
		}

		inline DirectX::XMVECTOR XM_CALLCONV RandomOnHemisphere(DirectX::FXMVECTOR InNormal) {
			DirectX::XMVECTOR onUnitSphere = RandomUnitVector();
			if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(onUnitSphere, InNormal)) > 0.f) // In the same hemisphere as the normal
			{
				return onUnitSphere;
			}
			else
			{
				return DirectX::XMVectorSubtract(VECTOR_ZERO, onUnitSphere);
			}
		}
	public:
		LocalVectorDistributionUnitSphereDistribution();
	};
};