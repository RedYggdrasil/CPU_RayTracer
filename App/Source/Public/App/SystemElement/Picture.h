#pragma once

#include "App/pch.h"
#include <filesystem>
#include <vector>

namespace AppNmsp
{
	struct Color
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};
	struct PixelData
	{
		union
		{
			uint8_t RGB[3];
			Color Color;
		};
	};

	class Picture
	{
	private:
		DirectX::XMINT2 m_size;
		size_t m_totalSize;
		std::vector<PixelData> m_pixels;
		std::filesystem::path m_path;

	public:
		const DirectX::XMINT2& GetSize() const { return m_size; }
		size_t GetTotalSize() const { return m_totalSize; }
		const std::filesystem::path& GetPath() const { return m_path; }

		inline const PixelData& operator [](const DirectX::XMINT2& InCoord) const
		{
			return m_pixels[(InCoord.y * m_size.x) + InCoord.x];
		}
		inline PixelData& operator [](const DirectX::XMINT2& InCoord)
		{
			return m_pixels[(InCoord.y * m_size.x) + InCoord.x];
		}
	public:
		Picture(DirectX::XMINT2 InSize, std::string_view InPictureNameExt);

	public:
		void WriteToDisk(bool bInVerbose = false);
	private:
		void ComputeTotalSize() { m_totalSize = m_size.x * m_size.y; }
	};
};