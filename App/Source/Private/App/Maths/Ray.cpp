#include "App/Maths/Ray.h"
#include "App/Tools/RLog.h"

#include <format>

std::string AppNmsp::_tostring(const RayFLT& InRayFLT)
{
	return std::format(TEXT("[ Origin : {}, Direction : {}]"), _tostring(InRayFLT.Origin), _tostring(InRayFLT.Direction));
}
