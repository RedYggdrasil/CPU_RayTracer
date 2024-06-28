#include "App/Tools/RRenderRandomizer.h"

using namespace AppNmsp;

constexpr uint8_t RAYGEN_BITSHIFT = 4;

RRenderRandomizer* RRenderRandomizer::s_renderRandomizer = nullptr;
RRenderRandomizer::RRenderRandomizer()
{
}

void RRenderRandomizer::Initialize(const uint64_t InSeed)
{
	RRandomizer::Initialize(InSeed);
	this->SetDefaultInstance(this);
	m_rayGenenrator = std::mt19937(static_cast<uint32_t>((m_seed >> RAYGEN_BITSHIFT) & 0xFFFFFFFF));
	s_renderRandomizer = this;

	static std::uniform_int_distribution<uint32_t> distribution(0, 1);
}
