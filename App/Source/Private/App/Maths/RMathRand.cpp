#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;
LocalVectorDistribution LocalVectorDistribution::CreateDesyncUnitDistribution()
{
    return LocalVectorDistribution(RRenderRandomizer::RayGenSeed(RRenderRandomizer::s_renderRandomizer->GetSeed()));
}


LocalVectorDistribution LocalVectorDistribution::CreateDesyncRangedDistribution(const float InMin, const float InMax)
{
    return LocalVectorDistribution
    (
        RRenderRandomizer::RayGenSeed(RRenderRandomizer::s_renderRandomizer->GetSeed()),
        InMin,
        InMax
    );
}


LocalVectorDistributionUnitSphereDistribution::LocalVectorDistributionUnitSphereDistribution()
    :LocalVectorDistribution
    (
        RRenderRandomizer::RayGenSeed(RRenderRandomizer::s_renderRandomizer->GetSeed()),
        -1.f,
        1.f
    )
{
}
