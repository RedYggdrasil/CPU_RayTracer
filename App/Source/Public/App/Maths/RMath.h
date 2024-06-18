#pragma once


#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <algorithm>
#include <DirectXMath.h>

constexpr DirectX::XMVECTOR X_VECTOR_MASK = { 1.f, 0.f, 0.f , 0.f };
constexpr DirectX::XMVECTOR Y_VECTOR_MASK = { 0.f, 1.f, 0.f , 0.f };
constexpr DirectX::XMVECTOR Z_VECTOR_MASK = { 0.f, 0.f, 1.f , 0.f };
constexpr DirectX::XMVECTOR W_VECTOR_MASK = { 0.f, 0.f, 0.f , 1.f };

constexpr DirectX::XMVECTOR XY_VECTOR_MASK = { 1.f, 1.f, 0.f , 0.f };
constexpr DirectX::XMVECTOR XZ_VECTOR_MASK = { 1.f, 0.f, 1.f , 0.f };
constexpr DirectX::XMVECTOR YZ_VECTOR_MASK = { 0.f, 1.f, 1.f , 0.f };

constexpr DirectX::XMVECTOR XYZ_VECTOR_MASK = { 1.f, 1.f, 1.f , 0.f };

//#define std::numeric_limits<float>::epsilon FLT_EPSILON
namespace AppNmsp
{
	class RMath
	{
	public:
		static const double PI;
	public:
		RMath() = delete;
	public:
		template<typename TInterger>
		inline static TInterger Align(const TInterger SizeValue, TInterger Alignement)
		{
			//SizeValue + (Alignement - 1) result in the biggest number from SizeValue witch is < Alignement
			//Division by Alignement is integer division, so effectively floor result. By then we have minimal number of Aligned block needed (0+) to store SizeValue
			//The final multiply return the size of that number of aligned block
			return ((SizeValue + (Alignement - 1)) / Alignement) * Alignement;
		};

		static double Cos(double rad);
		static double Sin(double rad);
		static double Tan(double rad);

		static DirectX::XMMATRIX TransformMatrixDeg(const DirectX::XMFLOAT3& InLocation, const DirectX::XMFLOAT3& InDegreeRotation, const DirectX::XMFLOAT3& InScale);
		static DirectX::XMMATRIX TransformMatrixRad(const DirectX::XMFLOAT3& InLocation, const DirectX::XMFLOAT3& InRadianRotation, const DirectX::XMFLOAT3& InScale);


		static DirectX::XMVECTOR XM_CALLCONV ComputeNormalFromPointsClockwise(DirectX::FXMVECTOR InPointOne, DirectX::FXMVECTOR InPointTwo, DirectX::FXMVECTOR InPointThree);

		inline static constexpr DirectX::XMFLOAT4X4 XM_CALLCONV ScaleMatrixAxis(const DirectX::XMFLOAT4X4& InMatrix, const DirectX::XMFLOAT3& InScale, bool bInScaleTranslation = true)
		{
			return DirectX::XMFLOAT4X4
			{
				InMatrix._11 * InScale.x, InMatrix._12 * InScale.x	, InMatrix._13 * InScale.x	,  InMatrix._14,
				InMatrix._21 * InScale.y, InMatrix._22 * InScale.y	, InMatrix._23 * InScale.y	,  InMatrix._24,
				InMatrix._31 * InScale.z, InMatrix._32 * InScale.z	, InMatrix._33 * InScale.z	,  InMatrix._34,
				((bInScaleTranslation) ? (InMatrix._41 * InScale.x) : InMatrix._41)			, ((bInScaleTranslation) ? (InMatrix._42 * InScale.y) : InMatrix._42)					, ((bInScaleTranslation) ? (InMatrix._43 * InScale.z) : InMatrix._43)				, InMatrix._44,
			};
		}

		inline static constexpr DirectX::XMMATRIX XM_CALLCONV TranspositionMatrix(DirectX::FXMVECTOR InXTranspositionOnly, DirectX::FXMVECTOR InYTranspositionOnly, DirectX::FXMVECTOR InZTranspositionOnly, DirectX::GXMVECTOR  InXYZTranslation)
		{
			return DirectX::XMMATRIX
			(
				DirectX::XMVectorSetW(InXTranspositionOnly, 0.f),
				DirectX::XMVectorSetW(InYTranspositionOnly, 0.f),
				DirectX::XMVectorSetW(InZTranspositionOnly, 0.f),
				DirectX::XMVectorSetW(InXYZTranslation, 1.f)
			);
		}

		inline static constexpr DirectX::XMFLOAT4X4 TranspostionMatrix(const DirectX::XMFLOAT3& InXTranspositionOnly, const DirectX::XMFLOAT3& InYTranspositionOnly, const DirectX::XMFLOAT3& InZTranspositionOnly, const DirectX::XMFLOAT3& InXYZTranslation)
		{
			return DirectX::XMFLOAT4X4
			{
				InXTranspositionOnly.x, InXTranspositionOnly.y, InXTranspositionOnly.z, 0.f,
				InYTranspositionOnly.x, InYTranspositionOnly.y, InYTranspositionOnly.z, 0.f,
				InZTranspositionOnly.x, InZTranspositionOnly.y, InZTranspositionOnly.z, 0.f,
				InXYZTranslation.x,		InXYZTranslation.y,		InXYZTranslation.z,		1.f,
			};
		}
		inline static constexpr DirectX::XMFLOAT4X4 ExtractTranspostionMatrix(const DirectX::XMFLOAT4X4& InTransposeTranslateMatrix)
		{
			return DirectX::XMFLOAT4X4
			{
				InTransposeTranslateMatrix._11	, InTransposeTranslateMatrix._12, InTransposeTranslateMatrix._13	, 0.f,
				InTransposeTranslateMatrix._21	, InTransposeTranslateMatrix._22, InTransposeTranslateMatrix._23	, 0.f,
				InTransposeTranslateMatrix._31	, InTransposeTranslateMatrix._32, InTransposeTranslateMatrix._33	, 0.f,
				0.f								, 0.f							, 0.f								, 1.f
			};
		}
		inline static constexpr DirectX::XMFLOAT3 ExtractTranslationVector(const DirectX::XMFLOAT4X4& InTransposeTranslateMatrix)
		{
			return DirectX::XMFLOAT3
			{
				InTransposeTranslateMatrix._41, InTransposeTranslateMatrix._42, InTransposeTranslateMatrix._43
			};
		}

		static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
		{
			DirectX::XMMATRIX A = M;
			A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
			return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
		}
		static DirectX::XMFLOAT4X4 InverseTranspose(DirectX::XMFLOAT4X4 M)
		{
			DirectX::XMMATRIX TransformMatrix = DirectX::XMLoadFloat4x4(&M);
			TransformMatrix = AppNmsp::RMath::InverseTranspose(TransformMatrix);

			DirectX::XMFLOAT4X4  StoredInverseTranspose;
			DirectX::XMStoreFloat4x4(&StoredInverseTranspose, TransformMatrix);
			return StoredInverseTranspose;
		}

		inline static bool GetIsDrawClockwise(const DirectX::XMFLOAT3& InExpectedNormalDir, const DirectX::XMFLOAT4X4& InTransposeMatrix)
		{
			DirectX::XMMATRIX loadedTransposeMatrix = DirectX::XMLoadFloat4x4(&InTransposeMatrix);
			DirectX::XMVECTOR loadedExpectedNormalDir = DirectX::XMLoadFloat3(&InExpectedNormalDir);
			//Vector
			loadedExpectedNormalDir = DirectX::XMVectorSetW(loadedExpectedNormalDir, 0.f);

			return GetIsDrawClockwise(loadedExpectedNormalDir, loadedTransposeMatrix);
		}
		static bool XM_CALLCONV GetIsDrawClockwise(DirectX::FXMVECTOR InExpectedNormalDir, DirectX::FXMMATRIX InTransposeMatrix);

		static DirectX::XMFLOAT3 QuatToEuler(DirectX::XMFLOAT4 InQuaternion);

		static DirectX::XMVECTOR XM_CALLCONV QuatToEulerDegXMVECTOR(DirectX::FXMVECTOR InQuaternion);

		static DirectX::XMVECTOR XM_CALLCONV LookAtQuatRad(DirectX::FXMVECTOR InFromPosition, DirectX::FXMVECTOR InToPosition);
		static DirectX::XMVECTOR XM_CALLCONV FromForwardVectorQuatRad(DirectX::FXMVECTOR InForwardVector);

		template <typename Num, typename T> //Num : Numerical, T : Float Numerical
		static Num Lerp(const Num& A, const Num& B, T Time)
		{
			return A + ((B - A) * std::clamp<T>(Time, (T)0.0, (T)1.0));
		};
	};


	/// XYZ to XY ///
	template<typename T>
	struct TypeLookupTNumericalAggrToUnderlying {
		using type = void;
	};
	template<>
	struct TypeLookupTNumericalAggrToUnderlying<DirectX::XMFLOAT3> {
		using type = float;
	};
	template<>
	struct TypeLookupTNumericalAggrToUnderlying<DirectX::XMFLOAT2> {
		using type = float;
	};

	template<>
	struct TypeLookupTNumericalAggrToUnderlying<DirectX::XMINT3> {
		using type = int32_t;
	};
	template<>
	struct TypeLookupTNumericalAggrToUnderlying<DirectX::XMINT2> {
		using type = int32_t;
	};


	/// XYZ to XY ///
	template<typename T>
	struct TypeLookupTNumericalXYZToAggr2D {
		using type = void;
	};

	template<>
	struct TypeLookupTNumericalXYZToAggr2D<DirectX::XMFLOAT3> {
		using type = DirectX::XMFLOAT2;
	};

	template<>
	struct TypeLookupTNumericalXYZToAggr2D<DirectX::XMINT3> {
		using type = DirectX::XMINT2;
	};


	/// XY to XYZ ///
	template<typename T>
	struct TypeLookupTNumericalXYToAggr3D {
		using type = void;
	};

	template<>
	struct TypeLookupTNumericalXYToAggr3D<DirectX::XMFLOAT2> {
		using type = DirectX::XMFLOAT3;
	};

	template<>
	struct TypeLookupTNumericalXYToAggr3D<DirectX::XMINT2> {
		using type = DirectX::XMINT3;
	};

	template<typename TNumericalXYZ>
	inline constexpr typename TypeLookupTNumericalXYZToAggr2D<TNumericalXYZ>::type xy(const TNumericalXYZ& InNum3) {
		return typename TypeLookupTNumericalXYZToAggr2D<TNumericalXYZ>::type{ InNum3.x, InNum3.y };
	};
	template<typename TNumericalXYZ>
	inline constexpr typename TypeLookupTNumericalXYZToAggr2D<TNumericalXYZ>::type xz(const TNumericalXYZ& InNum3) {
		return typename TypeLookupTNumericalXYZToAggr2D<TNumericalXYZ>::type{ InNum3.x, InNum3.z };
	};
	template<typename TNumericalXYZ>
	inline constexpr typename TypeLookupTNumericalXYZToAggr2D<TNumericalXYZ>::type yz(const TNumericalXYZ& InNum3) {
		return typename TypeLookupTNumericalXYZToAggr2D<TNumericalXYZ>::type{ InNum3.y, InNum3.z };
	};


	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_add2(const TNumericalXY& InLeft, const TNumericalXY& InRight)
	{
		return { InLeft.x + InRight.x, InLeft.y + InRight.y };
	}
	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_add2(const TNumericalXY& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXY>::type InRight)
	{
		return { InLeft.x + InRight, InLeft.y + InRight };
	}

	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_substract2(const TNumericalXY& InLeft, const TNumericalXY& InRight)
	{
		return { InLeft.x - InRight.x, InLeft.y - InRight.y };
	}

	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_substract2(const TNumericalXY& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXY>::type InRight)
	{
		return { InLeft.x - InRight, InLeft.y - InRight };
	}

	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_multiply2(const TNumericalXY& InLeft, const TNumericalXY& InRight)
	{
		return { InLeft.x * InRight.x, InLeft.y * InRight.y };
	}

	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_multiply2(const TNumericalXY& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXY>::type InRight)
	{
		return { InLeft.x * InRight, InLeft.y * InRight };
	}

	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_divide2(const TNumericalXY& InLeft, const TNumericalXY& InRight)
	{
		return { InLeft.x / InRight.x, InLeft.y / InRight.y };
	}

	template<typename TNumericalXY>
	inline constexpr TNumericalXY unoptimized_divide2(const TNumericalXY& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXY>::type InRight)
	{
		return { InLeft.x / InRight, InLeft.y / InRight };
	}

	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_add3(const TNumericalXYZ& InLeft, const TNumericalXYZ& InRight)
	{
		return { InLeft.x + InRight.x, InLeft.y + InRight.y, InLeft.z + InRight.z };
	}
	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_add3(const TNumericalXYZ& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXYZ>::type InRight)
	{
		return { InLeft.x + InRight, InLeft.y + InRight, InLeft.z + InRight };
	}

	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_substract3(const TNumericalXYZ& InLeft, const TNumericalXYZ& InRight)
	{
		return { InLeft.x - InRight.x, InLeft.y - InRight.y, InLeft.z - InRight.z };
	}
	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_substract3(const TNumericalXYZ& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXYZ>::type InRight)
	{
		return { InLeft.x - InRight, InLeft.y - InRight, InLeft.z - InRight };
	}

	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_multiply3(const TNumericalXYZ& InLeft, const TNumericalXYZ& InRight)
	{
		return { InLeft.x * InRight.x, InLeft.y * InRight.y, InLeft.z * InRight.z };
	}
	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_multiply3(const TNumericalXYZ& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXYZ>::type InRight)
	{
		return { InLeft.x * InRight, InLeft.y * InRight, InLeft.z * InRight };
	}

	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_divide3(const TNumericalXYZ& InLeft, const TNumericalXYZ& InRight)
	{
		return { InLeft.x / InRight.x, InLeft.y / InRight.y, InLeft.z / InRight.z };
	}
	template<typename TNumericalXYZ>
	inline constexpr TNumericalXYZ unoptimized_divide3(const TNumericalXYZ& InLeft, const typename TypeLookupTNumericalAggrToUnderlying<TNumericalXYZ>::type InRight)
	{
		return { InLeft.x / InRight, InLeft.y / InRight, InLeft.z / InRight };
	}
};