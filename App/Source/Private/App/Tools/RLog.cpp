#include "App/Tools/RLog.h"
#include <format>
#include <winnt.rh>
#include <iostream>

static const std::string ESEVERITY_DISPLAY = TEXT("Display");
static const std::string ESEVERITY_WARNING = TEXT("Warning");
static const std::string ESEVERITY_ERROR = TEXT("Error");
static const std::string ESEVERITY_EXCEPTION = TEXT("Exception");
static const std::string ESEVERITY_UNKNOWN = TEXT("Unknown");

using namespace DirectX;
using namespace AppNmsp;
using std::cin, std::cout, std::endl;

void AppNmsp::RLog::Log(ESeverity EInSeverity, const std::string_view InDisplayError)
{
    cout << "[" << ToString(EInSeverity) << "] : " << InDisplayError << endl;
}

std::string RLog::XMMatrixToString(const XMMATRIX& InMatrix)
{
    std::string result;
    XMFLOAT4X4 elements;
    XMStoreFloat4x4(&elements, InMatrix);
    return XMFloat4x4ToString(elements);
}

std::string RLog::XMFloat4x4ToString(const XMFLOAT4X4& InFloat4x4)
{
    std::string result;
    result.reserve(120);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result += std::to_string(InFloat4x4.m[i][j]) + "\t";
        }
        result += "\n";
    }

    return result;
}

std::string RLog::XMFloat4ToString(const XMFLOAT4& InFloat4)
{
    return std::format(TEXT("[ x : {}, y : {}, z : {}, w : {}]"), InFloat4.x, InFloat4.y, InFloat4.z, InFloat4.w);
}

std::string RLog::XMFloat3ToString(const XMFLOAT3& InFloat3)
{
    return std::format(TEXT("[ x : {}, y : {}, z : {}]"), InFloat3.x, InFloat3.y, InFloat3.z);
}

std::string RLog::XMFloat2ToString(const XMFLOAT2& InFloat2)
{
    return std::format(TEXT("[ x : {}, y : {}]"), InFloat2.x, InFloat2.y);
}

std::string RLog::XMInt4ToString(const XMINT4& InInt4)
{
    return std::format(TEXT("[ x : {}, y : {}, z : {}, w : {}]"), InInt4.x, InInt4.y, InInt4.z, InInt4.w);
}

std::string RLog::XMInt3ToString(const XMINT3& InInt3)
{
    return std::format(TEXT("[ x : {}, y : {}, z : {}]"), InInt3.x, InInt3.y, InInt3.z);
}

std::string RLog::XMInt2ToString(const XMINT2& InInt2)
{
    return std::format(TEXT("[ x : {}, y : {}]"), InInt2.x, InInt2.y);
}

std::string RLog::XMVECTORToString(FXMVECTOR InVector, const uint8_t InVectorSize)
{
    std::string returnValue;
    XMFLOAT4 flt;
    XMStoreFloat4(&flt, InVector);

    switch (InVectorSize)
    {
    case 4ui8:
        return std::format(TEXT("( X : {}, Y : {}, Z : {}, W : {}) "), flt.x, flt.y, flt.z, flt.w);
    case 3ui8:
        return std::format(TEXT("( X : {}, Y : {}, Z : {}) "), flt.x, flt.y, flt.z);
    case 2ui8:
        return std::format(TEXT("( X : {}, Y : {}) "), flt.x, flt.y);
    case 1ui8:
        return std::format(TEXT("( X : {} ) "), flt.x);
    default:
        return std::format(TEXT("( invalid vector size {} ) "), InVectorSize);
    }
}

const std::string& AppNmsp::ToString(RLog::ESeverity EInSeverity)
{
    switch (EInSeverity)
    {
    case RLog::ESeverity::Display:
        return ESEVERITY_DISPLAY;
    case RLog::ESeverity::Warning:
        return ESEVERITY_WARNING;
    case RLog::ESeverity::Error:
        return ESEVERITY_ERROR;
    case RLog::ESeverity::Exception:
        return ESEVERITY_EXCEPTION;
    default:
        cout << "[UnimplExcep] Case not handled in switch of type 'RSRLog::ESeverity' in RSRLog.cpp" << endl;
        return ESEVERITY_UNKNOWN;
    }
}
