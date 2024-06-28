#include "App/Tools/RRenderRandomizer.h"

using namespace AppNmsp;



RRenderRandomizer* RRenderRandomizer::s_renderRandomizer = nullptr;
RRenderRandomizer::RRenderRandomizer()
{
}

void RRenderRandomizer::Initialize(const uint64_t InSeed)
{
	RRandomizer::Initialize(InSeed);
	this->SetDefaultInstance(this);
	m_rayGenenrator = std::mt19937(static_cast<uint32_t>(RayGenSeed(m_seed)));
	s_renderRandomizer = this;
}
