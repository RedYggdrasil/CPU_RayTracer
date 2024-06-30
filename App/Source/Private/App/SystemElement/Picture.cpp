#include "App/SystemElement/Picture.h"

#include "App/Maths/Interval.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

using std::filesystem::path;

path GetExecutablePath()
{
	static path resultPath;

	if (resultPath.empty())
	{
		//Get module nullptr (this) result in "C:/.../filepath/myexecutable.exe"
		wchar_t moduleFileName[MAX_PATH];
		GetModuleFileNameW(nullptr, moduleFileName, MAX_PATH);

		resultPath = moduleFileName;
		resultPath.remove_filename();
	}
	return resultPath;
}

AppNmsp::Picture::Picture(DirectX::XMINT2 InSize, std::string_view InPictureNameExt)
	:m_size(InSize), m_pixels(),
#if WITH_REFERENCE
		m_pixelDBLs(), 
#endif
		m_path(GetExecutablePath() / InPictureNameExt)
{
	ComputeTotalSize();
	m_pixels.resize(GetTotalSize());
#if WITH_REFERENCE
	m_pixelDBLs.resize(GetTotalSize());
#endif
}

const std::string_view ByteView(const void* InData, const size_t InSize)
{
	return std::string_view(reinterpret_cast<const char*>(InData), InSize);
};

void AppNmsp::Picture::WriteToDisk(bool bInVerbose)
{
	std::ofstream MyFile(m_path, std::ios::out | std::ios::binary);
	MyFile << "P6\n" << m_size.x << " " << m_size.y << "\n255\n";// << ByteView(&m_pixels[0].RGB[0], sizeof(PixelData) * GetTotalSize());

	static constexpr FInterval intencity(0.000f, 0.999f);
	static constexpr DInterval intencityDBL(0.000, 0.999);
	for (size_t i = 0; i < m_totalSize; ++i)
	{
#if USE_DOUBLE_PRECISION
		MyFile
			<< uint8_t(256.0 * intencityDBL.Clamp(m_pixelDBLs[i].x))
			<< uint8_t(256.0 * intencityDBL.Clamp(m_pixelDBLs[i].y))
			<< uint8_t(256.0 * intencityDBL.Clamp(m_pixelDBLs[i].z));
#else
		MyFile
			<< uint8_t(256.f * intencity.Clamp(m_pixels[i].x))
			<< uint8_t(256.f * intencity.Clamp(m_pixels[i].y))
			<< uint8_t(256.f * intencity.Clamp(m_pixels[i].z));
#endif
	}
	//uint8_t* data = &m_pixels[0].RGB[0];

	MyFile.close();
	if (bInVerbose)
	{
		std::cout << "Picture written to disk at : " << m_path << std::endl;
	}
}
