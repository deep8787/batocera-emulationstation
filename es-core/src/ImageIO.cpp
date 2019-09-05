#include "ImageIO.h"

#include "Log.h"
#include <FreeImage.h>
#include <string.h>

unsigned char* ImageIO::loadFromMemoryRGBA32(const unsigned char * data, const size_t size, size_t & width, size_t & height)
{
	std::vector<unsigned char> rawData;
	width = 0;
	height = 0;
	FIMEMORY * fiMemory = FreeImage_OpenMemory((BYTE *)data, (DWORD)size);
	if (fiMemory != nullptr) 
	{
		//detect the filetype from data
		FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(fiMemory);
		if (format != FIF_UNKNOWN && FreeImage_FIFSupportsReading(format))
		{
			//file type is supported. load image
			FIBITMAP * fiBitmap = FreeImage_LoadFromMemory(format, fiMemory);
			if (fiBitmap != nullptr)
			{
				//loaded. convert to 32bit if necessary
				if (FreeImage_GetBPP(fiBitmap) != 32)
				{
					FIBITMAP * fiConverted = FreeImage_ConvertTo32Bits(fiBitmap);
					if (fiConverted != nullptr)
					{
						//free original bitmap data
						FreeImage_Unload(fiBitmap);
						fiBitmap = fiConverted;
					}
				}
				if (fiBitmap != nullptr)
				{
					width = FreeImage_GetWidth(fiBitmap);
					height = FreeImage_GetHeight(fiBitmap);

					unsigned char* tempData = new unsigned char[width * height * 4];

					int w = (int)width;

					for (int y = (int)height; --y >= 0; )
					{
						unsigned int* argb = (unsigned int*)FreeImage_GetScanLine(fiBitmap, y);
						unsigned int* abgr = (unsigned int*)(tempData + (y * width * 4));
						for (int x = w; --x >= 0;)
						{
							unsigned int c = argb[x];
							abgr[x] = (c & 0xFF00FF00) | ((c & 0xFF) << 16) | ((c >> 16) & 0xFF);
						}
					}

					FreeImage_Unload(fiBitmap);
					FreeImage_CloseMemory(fiMemory);

					return tempData;
				}
			}
			else
			{
				LOG(LogError) << "Error - Failed to load image from memory!";
			}
		}
		else
		{
			LOG(LogError) << "Error - File type " << (format == FIF_UNKNOWN ? "unknown" : "unsupported") << "!";
		}
		//free FIMEMORY again
		FreeImage_CloseMemory(fiMemory);
	}

	return nullptr;
}

void ImageIO::flipPixelsVert(unsigned char* imagePx, const size_t& width, const size_t& height)
{
	unsigned int temp;
	unsigned int* arr = (unsigned int*)imagePx;
	for(size_t y = 0; y < height / 2; y++)
	{
		for(size_t x = 0; x < width; x++)
		{
			temp = arr[x + (y * width)];
			arr[x + (y * width)] = arr[x + (height * width) - ((y + 1) * width)];
			arr[x + (height * width) - ((y + 1) * width)] = temp;
		}
	}
}

Vector2f ImageIO::getPictureMinSize(Vector2f imageSize, Vector2f maxSize)
{
	float cxDIB = maxSize.x();
	float cyDIB = maxSize.y();

	float xCoef = maxSize.x() / imageSize.x();
	float yCoef = maxSize.y() / imageSize.y();

	if (imageSize.x() * yCoef < maxSize.x())
		cyDIB = imageSize.y() * xCoef;
	else
		cxDIB = imageSize.x() * yCoef;

	return Vector2f(cxDIB, cyDIB);
}