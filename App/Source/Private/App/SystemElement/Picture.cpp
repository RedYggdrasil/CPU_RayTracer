#include "App/SystemElement/Picture.h"

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
:m_size(InSize), m_pixels(), m_path(GetExecutablePath() / InPictureNameExt)
{
	ComputeTotalSize();
	m_pixels.resize(GetTotalSize());
}

const std::string_view ByteView(const void* InData, const size_t InSize)
{
	return std::string_view(reinterpret_cast<const char*>(InData), InSize);
};

void AppNmsp::Picture::WriteToDisk(bool bInVerbose)
{
	std::ofstream MyFile(m_path, std::ios::out | std::ios::binary);
	MyFile << "P6\n"<< m_size.x <<" " << m_size.y << "\n255\n" << ByteView(&m_pixels[0].RGB[0], sizeof(PixelData) * GetTotalSize());

	uint8_t* data = &m_pixels[0].RGB[0];

	MyFile.close();
	if (bInVerbose)
	{
		std::cout << "Picture written to disk at : " << m_path << std::endl;
	}
}
