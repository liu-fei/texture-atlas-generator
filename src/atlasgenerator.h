//==============================================================================
// Name         : atlasgenerator.h
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Declares AtlasGenerator Class
//==============================================================================

#ifndef ATLASGENERATOR_H
#define ATLASGENERATOR_H

#include <vector>                   // std::vector
#include <string>                   // std::string
#include <cstdint>                  // uint8_t
#include "binarytreealgorithm.h"    // BinaryTreeAlgorithm


//==============================================================================
//! Image Struct For Each Image On The Texture Atlas
//==============================================================================
struct Image
{
    //! @brief Constructor
    //! @param aName The Image Name
    //! @param aX The X Coordinate Of The Image Top-Left Point on Texture Atlas
    //! @param aY The Y Coordinate Of The Image Top-Left Point on Texture Atlas
    //! @param aWidth The Image Width
    //! @param aHeight The Image Height
    Image(std::string aName, int aX, int aY, int aWidth, int aHeight)
        : name(aName), x(aX), y(aY), width(aWidth), height(aHeight), data(nullptr)
    {
    };

    //! @brief Constructor
    //! @param aName The Image Name
    //! @param aWidth The Image Width
    //! @param aHeight The Image Height
    //! @param aData The png image bytes
    //! @param aChannels The png image Channels
    Image(std::string aName, int aWidth, int aHeight, uint8_t* aData, int aChannels)
        : name(aName), width(aWidth), height(aHeight), data(aData), channels(aChannels)
    {
    };

    std::string name;
    int         x;
    int         y;
    int         width;
    int         height;
    uint8_t*    data;    // png image bytes
    int         channels;
};


//==============================================================================
//! AtlasGenerator Class
//==============================================================================
class AtlasGenerator
{
    public:
    //! @brief Constructor
    //! @param aImgList A List Of All The Image Files With Path
    AtlasGenerator(const std::vector<std::string> aImgList);

    //! @brief Destructor
    ~AtlasGenerator();

    //! @brief Run AtlasGenerator
    void Run(std::string name,std::string growType);

    private:
    //! @brief Packing Images Onto The Texture Atlas, Also Collecting Metadata
    std::vector<uint8_t> Packing(std::string growType);

    //! @brief Sort Images By Their Max Side, Max(Width, Height) In Descendent Order
    //!        So The One Who Has Largest Side Get Packed First
    void SortImages();

    //! @brief Start From rootNood, Recursively Call Itself To Draw All Images
    //! @param aNode The Node In The Binary Tree
    //! @param aAtlasBuffer The Buffter For PNG Image Bytes Of The Texture Atlas
    //! @param aAtlasRowBytes The Row Bytes, libpng Draws Image Row by Row
    void DrawImages(Node* aNode, std::vector<uint8_t>&  aAtlasBuffer,
                    const int aAtlasRowBytes);

    //! @brief Output The Texture Atlas And Metadata To Files 
    //! @param aAtlasDataBuffer The Data (Raw Bytes) Of The Texture Atlas
    void Output(std::vector<uint8_t>& aAtlasDataBuffer,std::string name);

    // ! @brief Save The Metadata In .json Format In The Working Directory
    void OutputMetadata() const;

    private:
    BinaryTreeAlgorithm*        iPackingAlgorithm;
    std::vector<std::string>    iImgFileList;
    std::vector<Image>          iImageList;
    std::vector<Image>          iSortedImageList;
};

#endif    // ATLASGENERATOR_H

// End Of File
