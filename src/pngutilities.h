//==============================================================================
// Name         : pngutilities.h 
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Declares Utility Functions For Using libpng
//==============================================================================
#include <cstdint>    // uint8_t

namespace pngutilities
{
	//! @brief Read .png File 
	//! @param aPath An Image File With Path
	//! @param aWidth The Width Of The Image 
	//! @param aHeight The Height Of The Image
	//! @param aChannels The Channels Of The Image
	//! @param aChannels The PNG image Channels
	//! @return The Pointer To The Image Data Bytes
	uint8_t* ReadPNG(const char* aPath, int& aWidth, int& aHeight, int& aChannels);
    
	//! @brief Write .png File 
	//! @param aFilename A File Name
	//! @param aWidth The Width Of The Texture Atlas 
	//! @param aHeight The Height Of The Texture Atlas 
	//! @param aChannels The Channels Of The Texture Atlas
	//! @param aData The Texture Atlas Data Bytes
	void WritePNG(const char* aFilename, int aWidth, int aHeight, unsigned char* aData);
}


// End Of File
