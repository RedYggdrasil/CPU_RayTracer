#pragma once
#include "App/Tools/RRandomizer.h"
#include <random>

namespace AppNmsp
{
    class RRenderRandomizer : public RRandomizer
    {
	public:
		static constexpr uint8_t RAYGEN_BITSHIFT = 4;
		inline static constexpr uint32_t RayGenSeed(const uint64_t InSeed) { return static_cast<uint32_t>((InSeed >> RAYGEN_BITSHIFT) & 0xFFFFFFFF); }
	public:
		static RRenderRandomizer* s_renderRandomizer;
	private:
		std::mt19937 m_rayGenenrator;
	public:
		RRenderRandomizer();
		using RRandomizer::Initialize;
		virtual void Initialize(const uint64_t InSeed) override;
	public:
		inline static bool GetIsInitialized() { return mp_inst; }
		inline static const RRenderRandomizer& Get() { return *static_cast<RRenderRandomizer*>(mp_inst); }
	public:
		inline std::mt19937& GetRayGenerator() { return m_rayGenenrator; }
		inline uint64_t GetSeed() const { return m_seed; }

		/// <summary>
		/// Default real distribution from 0 (Inc) to 1 (Exc)
		/// </summary>
		/// <typeparam name="T32BReal"></typeparam>
		/// <returns></returns>
		template <typename T32BReal>
		inline T32BReal fGetRayValue()
		{
			static std::uniform_real_distribution<T32BReal> distribution(static_cast<T32BReal>(0.f), static_cast<T32BReal>(1.f));
			return distribution(m_rayGenenrator);
		}

		/// <summary>
		/// Default real distribution from 0 (Inc) to 1 (Exc)
		/// </summary>
		/// <typeparam name="T32BReal"></typeparam>
		/// <returns></returns>
		template <typename T32BReal>
		static inline T32BReal s_fGetRayValue()
		{
			return m_rayGenenrator->fGetRayValue<T32BReal>();
		}

		template <typename T32BReal>
		struct LocalRealDistribution
		{
		public:
			std::mt19937 Generator;
			std::uniform_real_distribution<T32BReal> Distribution;
		public:
			inline T32BReal operator()() { return Distribution(Generator); }
		public:
			LocalRealDistribution(const uint32_t InSeed, const T32BReal InMinIncluded, const T32BReal InMaxExcluded)
				: Generator(InSeed), Distribution(InMinIncluded, InMaxExcluded) { }
			LocalRealDistribution(const uint32_t InSeed, const T32BReal InMaxExcluded)
				: Generator(InSeed), Distribution(InMaxExcluded) { }
			LocalRealDistribution(const uint32_t InSeed)
				: Generator(InSeed), Distribution() { }
		};

		template <typename T32BReal, typename... DistribArgs>
		inline LocalRealDistribution<T32BReal> GetLocalDesyncDistribution(DistribArgs&&... InDistribArgs) const
		{
			return LocalRealDistribution<T32BReal>(RayGenSeed(this->m_seed), std::forward<DistribArgs>(InDistribArgs)...);
		}
		template <typename T32BReal, typename... DistribArgs>
		inline static LocalRealDistribution<T32BReal> s_GetLocalDesyncDistribution(DistribArgs&&... InDistribArgs)
		{
			return s_renderRandomizer->GetLocalDesyncDistribution<T32BReal>(std::forward<DistribArgs>(InDistribArgs)...);
		}
	};
};