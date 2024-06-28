#pragma once
#include "App/Tools/RRandomizer.h"
#include <random>

namespace AppNmsp
{
    class RRenderRandomizer : public RRandomizer
    {
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
	};
};