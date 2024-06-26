#pragma once

#include "App/pch.h"
#include <filesystem>
#include <vector>

namespace AppNmsp
{
#if WITH_REFERENCE
	struct pictureColor {
	public:
		double x, y, z;
	};
#endif
	class Picture
	{

	public:
#if WITH_REFERENCE
		std::vector<pictureColor> m_pixelDBLs;
		inline size_t mPixelDBLIDX(size_t x, size_t y) const
		{
			return (y * m_size.x) + x;
		}
#endif

	private:
		DirectX::XMINT2 m_size;
		size_t m_totalSize;
		std::vector<DirectX::XMFLOAT3> m_pixels;
		std::filesystem::path m_path;

	public:
		const DirectX::XMINT2& GetSize() const { return m_size; }
		size_t GetTotalSize() const { return m_totalSize; }
		const std::filesystem::path& GetPath() const { return m_path; }

		inline const DirectX::XMFLOAT3& operator [](const DirectX::XMINT2& InCoord) const
		{
			return m_pixels[(InCoord.y * m_size.x) + InCoord.x];
		}
		inline DirectX::XMFLOAT3& operator [](const DirectX::XMINT2& InCoord)
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