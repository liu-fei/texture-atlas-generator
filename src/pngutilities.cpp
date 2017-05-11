//==============================================================================
// Name         : pngutilities.cpp 
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Implements Utility Functions For Using libpng
//==============================================================================

#define PNG_SKIP_SETJMP_CHECK
#include "pngutilities.h" 
#include <stdint.h>          // uint8_t
#include <string.h>          // std::string    
#include <setjmp.h>          // setjmp
#include <stdexcept>         // std::runtime_error, std::invalid_argument
#include <png.h>             // png_structp, png_infop, ...


namespace pngutilities
{
    //==============================================================================
    //! @brief Read .png File 
    //! @param aPath An Image File With Path
    //! @param aWidth The Width Of The Image 
    //! @param aHeight The Height Of The Image
    //! @param aChannels The Channels Of The Image
    //! @param aChannels The PNG image Channels
    //! @return The Pointer To The Image Data Bytes
    //==============================================================================
    uint8_t* ReadPNG(const char* aPath, int& aWidth, int& aHeight, int& aChannels)
    {
        // open file as binary
        FILE* file = fopen(aPath, "rb");
        if (!file)
            throw std::runtime_error("Could not open file " + std::string(aPath) + "!");

        // header for testing if it is a png
        uint8_t header[8];
        // read the header
        if (fread(header, 1, 8, file) <= 0)
            throw std::runtime_error("Could not read file " + std::string(aPath) + "!");

        // Check png header
        if (png_sig_cmp(header, 0, 8))
            throw std::invalid_argument(std::string(aPath) + " is not a .png file!");

        // create png struct
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png)
            throw std::runtime_error("png_create_read_struct failed!");

        // create png info struct
        png_infop info = png_create_info_struct(png);
        if (!info)
            throw std::runtime_error("png_create_info_struct failed!");

        // create png end info struct
        png_infop infoEnd = png_create_info_struct(png);
        if (!infoEnd)
            throw std::runtime_error("png_create_info_struct failed!");

        // set libpng error handling mechanism
        if (setjmp(png_jmpbuf(png)))
            throw std::runtime_error("Could not read file " + std::string(aPath) + "!");

        // init png reading
        png_init_io(png, file);
        // let libpng know you already read the first 8 bytes
        png_set_sig_bytes(png, 8);

        png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING |
                     PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, nullptr);

        aWidth = png_get_image_width(png, info);
        aHeight = png_get_image_height(png, info);

        const int color = png_get_color_type(png, info);
        if (color != PNG_COLOR_TYPE_RGB && color != PNG_COLOR_TYPE_RGB_ALPHA)
            throw std::invalid_argument(std::string(aPath) + " is not a RGB image!");
        aChannels = (color == PNG_COLOR_TYPE_RGB_ALPHA?4:3);

        // allocate the image as a big block
        uint8_t* image = new uint8_t[aWidth * aHeight * aChannels];
        png_bytep* rows = png_get_rows(png, info);
        for (auto y = 0; y < aHeight; ++y)
            memcpy(image + y * aWidth * aChannels, rows[y], aWidth * aChannels);

        png_destroy_read_struct(&png, &info, &infoEnd);
        fclose(file);

        if (image == 0 || aChannels < 3)
            {
            throw std::runtime_error("Something is wrong with " + std::string(aPath));
            delete[] image;
            image = nullptr;
            }

        return image;
    }


    //==============================================================================
    //! @brief Write .png File 
    //! @param aFilename A File Name
    //! @param aWidth The Width Of The Texture Atlas 
    //! @param aHeight The Height Of The Texture Atlas 
    //! @param aChannels The Channels Of The Texture Atlas
    //! @param aData The Texture Atlas Data Bytes
    //==============================================================================
    void WritePNG(const char* aFilename, int aWidth, int aHeight, unsigned char* aData)
    {
        FILE* file = fopen(aFilename, "wb");
        if (!file)
            throw std::runtime_error(std::string(aFilename) + "could not be opened for writing!");

        png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!pngPtr)
            throw std::runtime_error("png_create_write_struct failed!");

        png_infop infoPtr = png_create_info_struct(pngPtr);
        if (!infoPtr)
            throw std::runtime_error("png_create_info_struct failed!");

        png_init_io(pngPtr, file);
        png_set_IHDR(pngPtr, infoPtr, aWidth, aHeight, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        png_byte** rowPtrs = new png_byte*[aHeight];
        for (auto i = 0; i < aHeight; ++i)
            rowPtrs[i] = aData + i * aWidth * 4;

        png_set_rows(pngPtr, infoPtr, rowPtrs);
        png_write_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, nullptr);

        png_destroy_write_struct(&pngPtr, &infoPtr);

        fclose(file);
        delete[] rowPtrs;
    }
}

// End Of File
