#include "App/Tools/RRandomizer.h"
#include <random>

using namespace AppNmsp;
RRandomizer* RRandomizer::mp_inst = nullptr;

void RRandomizer::Initialize(const uint64_t InSeed)
{
	m_seed = InSeed;
}

void RRandomizer::Initialize()
{
	std::random_device rd;   // non-deterministic generator
	std::mt19937_64 gen(rd());  // to seed mersenne twister.
	std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
	Initialize(dist(gen));
	return;
}

void RRandomizer::SetDefaultInstance(RRandomizer* InDefaultInstance)
{
	RRandomizer::mp_inst = InDefaultInstance;
}
