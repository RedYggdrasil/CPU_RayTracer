#pragma once

#include <DirectXMath.h>
#include <string>
#include <string_view>
#include <format>
#include <sstream>
#include <iomanip>


#define PREPROCESSOR_TO_STRING(x) PREPROCESSOR_TO_STRING_INNER(x)
#define PREPROCESSOR_TO_STRING_INNER(x) #x
#define L_PATH __FILE__ "(" PREPROCESSOR_TO_STRING(__LINE__) "):\n"

namespace AppNmsp
{
	class RLog
	{
	public:
		enum class ESeverity : uint8_t
		{
			Display = 0,
			Warning = 1,
			Error = 2,
			Exception = 3
		};
	public:
		RLog() = delete;
		RLog(const RLog&) = delete;
		RLog& operator=(const RLog&) = delete;
	public:
		inline static void LogDisplay(std::string_view InDisplayText) { Log(RLog::ESeverity::Display, InDisplayText); };
		inline static void LogWarning(std::string_view InWarningText) { Log(RLog::ESeverity::Warning, InWarningText); };
		inline static void LogError(std::string_view InErrorText) { Log(RLog::ESeverity::Error, InErrorText); };
		inline static void LogException(std::string_view InExceptionText) { Log(RLog::ESeverity::Exception, InExceptionText); };

	public:
		static void Log(ESeverity EInSeverity, const std::string_view InDisplayError);

	public:
		template<typename... Args >
		void static Log(ESeverity EInSeverity, std::format_string<Args...> InConstexpFormatText, Args&&... InArgs)
		{
			RLog::Log(EInSeverity, std::format(InConstexpFormatText, std::forward<Args>(InArgs)...));
		};
	public:
		static std::string XMMatrixToString(const DirectX::XMMATRIX& InMatrix);
		static std::string XMFloat4x4ToString(const DirectX::XMFLOAT4X4& InFloat4x4);
		static std::string XMFloat4ToString(const DirectX::XMFLOAT4& InFloat4);
		static std::string XMFloat3ToString(const DirectX::XMFLOAT3& InFloat3);
		static std::string XMFloat2ToString(const DirectX::XMFLOAT2& InFloat2);
		static std::string XMInt4ToString(const DirectX::XMINT4& InInt4);
		static std::string XMInt3ToString(const DirectX::XMINT3& InInt3);
		static std::string XMInt2ToString(const DirectX::XMINT2& InInt2);
		static std::string XM_CALLCONV XMVECTORToString(DirectX::FXMVECTOR InVector, const uint8_t InVectorSize = 4);
	};

	inline constexpr std::string_view ByteView(const void* InData, const size_t size)
	{
		return std::string_view(reinterpret_cast<const char*>(InData), size);
	};
	inline std::string HexadecimalView(const void* InData, const size_t size)
	{
		const unsigned char* uchar_data = static_cast<const unsigned char*>(InData);
		// Stringstream as no reserve function, we know the actual string size is size * 3, but can't use it there, too bad !
		std::stringstream ss;

		const size_t size_minus = (size > 0) ? (size - 1) : 0Ui64;
		// Convert each byte to a 2-digit hexadecimal number
		for (size_t i = 0; i < size; ++i) {
			ss << std::hex << std::setw(2) << std::setfill('0')
				<< static_cast<const int>(uchar_data[i]);
			if (i < size_minus)
			{
				ss << ' ';
			}
		}
		return ss.str();
	};

	inline std::string _tostring(const DirectX::XMMATRIX& InMatrix)
	{
		return RLog::XMMatrixToString(InMatrix);
	};
	static std::string XMFloat4x4ToString(const DirectX::XMFLOAT4X4& InFloat4x4)
	{
		return RLog::XMFloat4x4ToString(InFloat4x4);
	};
	inline std::string XM_CALLCONV _tostring(DirectX::FXMVECTOR InVector, const uint8_t InVectorSize = 4)
	{
		return RLog::XMVECTORToString(InVector, InVectorSize);
	};


	inline std::string _tostring(const DirectX::XMFLOAT4& InFloat4)
	{
		return RLog::XMFloat4ToString(InFloat4);
	}
	inline std::string _tostring(const DirectX::XMFLOAT3& InFloat3)
	{
		return RLog::XMFloat3ToString(InFloat3);
	}
	inline std::string _tostring(const DirectX::XMFLOAT2& InFloat2)
	{
		return RLog::XMFloat2ToString(InFloat2);
	}

	inline std::string _tostring(const DirectX::XMINT4& InInt4)
	{
		return RLog::XMInt4ToString(InInt4);
	}
	inline std::string _tostring(const DirectX::XMINT3& InInt3)
	{
		return RLog::XMInt3ToString(InInt3);
	}
	inline std::string _tostring(const DirectX::XMINT2& InInt2)
	{
		return RLog::XMInt2ToString(InInt2);
	}
	constexpr RLog::ESeverity LOG_DISPLAY = RLog::ESeverity::Display;
	constexpr RLog::ESeverity LOG_WARNING = RLog::ESeverity::Warning;
	constexpr RLog::ESeverity LOG_ERROR = RLog::ESeverity::Error;
	constexpr RLog::ESeverity LOG_EXCEPTION = RLog::ESeverity::Exception;
	const std::string& ToString(RLog::ESeverity EInSeverity);
};
