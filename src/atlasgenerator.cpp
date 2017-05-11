//==============================================================================
// Name         : atlasGenerator.cpp
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Implements AtlasGenerator Class
//==============================================================================

#include "atlasgenerator.h"            // AtlasGenerator
#include <set>                         // std::set
#include <vector>                      // std::vector
#include <string>                      // std::string
#include <utility>                     // std::pair
#include <algorithm>                   // std::max
#include <functional>                  // std::greater
#include <fstream>                     // std::ofstream
#include <iostream>                    // std::cout
#include "pngutilities.h"              // ReadPNG, WritePNG
#include "rapidjson/prettywriter.h"    // Prettywriter
#include "rapidjson/stringbuffer.h"    // StringBuffe


//==============================================================================
//! @brief Constructor
//! @param aImgList A List Of All The Image Files With Path
//==============================================================================
AtlasGenerator::AtlasGenerator(const std::vector<std::string> aImgList)
    : iImgFileList(aImgList)
    , iPackingAlgorithm(new BinaryTreeAlgorithm)
{
};


//==============================================================================
//! @brief Destructor
//==============================================================================
AtlasGenerator::~AtlasGenerator()
{
    delete iPackingAlgorithm;
    iPackingAlgorithm = nullptr;

    for (auto img : iSortedImageList)
        {
        delete[] img.data;
        img.data = nullptr;
        }
};


//==============================================================================
//! @brief Run AtlasGenerator
//==============================================================================
void AtlasGenerator::Run()
{
    std::vector<uint8_t> atlas = Packing();

    // output texture atlas and metadata to files
    Output(atlas);
}


//==============================================================================
//! @brief Packing Images Onto The Texture Atlas, Also Collecting Metadata
//==============================================================================
std::vector<uint8_t> AtlasGenerator::Packing()
{
    // sort images by their max side, max(width, height) in descendent order
    SortImages();

    // the initiate canvas dimension is set to the first image's dimension
    const int initialWidth = iSortedImageList[0].width;
    const int initialHeight = iSortedImageList[0].height;

    iPackingAlgorithm->Init(initialWidth, initialHeight);

    for (auto i = 0; i != iSortedImageList.size(); ++i)
        {
        int width = iSortedImageList[i].width;
        int height = iSortedImageList[i].height;

        Node* node = iPackingAlgorithm->Insert(width, height);
        if (node)
            iPackingAlgorithm->SplitNode(node, width, height, i);  // i imgID
        else    // run out space, grow the canvas
            node = iPackingAlgorithm->GrowAtlasCanvas(width, height, i);
        }

    // create an empty texture atlas with the size indicated by the rootNode of the binary tree
    int width = iPackingAlgorithm->rootNode()->width;
    int height = iPackingAlgorithm->rootNode()->height;

    std::vector<uint8_t> atlasBuffer(4 * width * height);
    const int atlasRowBytes = 4 * width;

    // draw images to canvas accoring to their coorespending tree Nodes indicated
    DrawImages(iPackingAlgorithm->rootNode(), atlasBuffer, atlasRowBytes);

    return  atlasBuffer;
}


//==============================================================================
//! @brief Sort Images By Their Max Side, Max(Width, Height) In Descendent Order
//!		   So The One Who Has Largest Side Get Packed First
//==============================================================================
void AtlasGenerator::SortImages()
{
    for (auto i = 0; i != iImgFileList.size(); ++i)
        {
        int width = 0, height = 0, channels = 0;
        uint8_t* imgData = pngutilities::ReadPNG(iImgFileList[i].c_str(), width, height, channels);

        std::string filePathName = iImgFileList[i].c_str();
        int pos = filePathName.find_last_of('/');
        std::string fileName = filePathName.substr(pos + 1, std::string::npos);

        iImageList.push_back(Image(fileName, width, height, imgData, channels));
        }

    std::vector<std::pair<int, int>> maxsideIndexList;  // pair<maxside, index>
    for (auto i = 0; i != iImageList.size(); ++i)
        {
        int maxSide = std::max(iImageList[i].width, iImageList[i].height);
        std::pair<int, int>  maxSideIndex(maxSide, i);
        maxsideIndexList.push_back(maxSideIndex);
        }

    // sort the list in descending order according to maxSide value
    std::sort(maxsideIndexList.begin(), maxsideIndexList.end(), std::greater<std::pair<int, int>>());

    iSortedImageList.reserve(iImageList.size());

    for (auto iter = maxsideIndexList.begin(); iter != maxsideIndexList.end(); ++iter)
        iSortedImageList.push_back(iImageList[iter->second]);
}


//==============================================================================
//! @brief Start From rootNood, Recursively Call Itself To Draw All Images
//! @param aNode The Node In The Binary Tree
//! @param aAtlasBuffer The Buffter For PNG Image Bytes Of The Texture Atlas
//! @param aAtlasRowBytes The Row Bytes, libpng Draws Image Row by Row
//==============================================================================
void AtlasGenerator::DrawImages(Node* aNode, std::vector<uint8_t>&
                                aAtlasBuffer, const int aAtlasRowBytes)
{
    if (aNode)
        {
        if (aNode->imgID >= 0)
            {
            // add new image's info to metadata
            iSortedImageList[aNode->imgID].x = aNode->x;
            iSortedImageList[aNode->imgID].y = aNode->y;

            int width = iSortedImageList[aNode->imgID].width;
            int height = iSortedImageList[aNode->imgID].height;
            int channels = iSortedImageList[aNode->imgID].channels;
            uint8_t* imgData = iSortedImageList[aNode->imgID].data;

            const int srcRowBytes = width  * channels;
            const int column_size = channels * width;

            uint8_t* dst = &aAtlasBuffer[4 * (aNode->y * iPackingAlgorithm->rootNode()->width + aNode->x)];

            if (channels == 4)
                {
                for (int y = 0; y < height; y++)
                    {
                    memcpy(dst, imgData, column_size);
                    imgData += srcRowBytes;
                    dst += aAtlasRowBytes;
                    }
                }
            else if (channels == 3)
                {
                for (int y = 0; y < height; y++)
                    {
                    for (int xs = 0, xd = 0; xs < column_size; xs += 3, xd += 4)
                        {
                        dst[xd + 0] = imgData[xs + 0];
                        dst[xd + 1] = imgData[xs + 1];
                        dst[xd + 2] = imgData[xs + 2];
                        dst[xd + 3] = 0xFF;
                        }
                    imgData += srcRowBytes;
                    dst += aAtlasRowBytes;
                    }
                }
            }

        DrawImages(aNode->rightChild, aAtlasBuffer, aAtlasRowBytes);
        DrawImages(aNode->downChild, aAtlasBuffer, aAtlasRowBytes);
        }
}


//==============================================================================
//! @brief Output The Texture Atlas Image And Metadata To Files 
//! @param aAtlasBuffer The Texture Atlas
//==============================================================================
void AtlasGenerator::Output(std::vector<uint8_t>& aAtlasBuffer)
{
    // save the texture atlas in .png format in the working directory
    pngutilities::WritePNG("texture_atlas.png", iPackingAlgorithm->rootNode()->width,
                           iPackingAlgorithm->rootNode()->height, &aAtlasBuffer[0]);

    // save the metadata in .json format in the working directory
    OutputMetadata();
}


//==============================================================================
// ! @brief Save The Metadata In .json Format In The Working Directory
//==============================================================================
void AtlasGenerator::OutputMetadata() const
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    writer.Key("Metadata");

    if (iSortedImageList.size() >= 2)
        writer.StartArray();
    for (const Image& img : iSortedImageList)
        {
        writer.StartObject();
        writer.Key("name");
        writer.String(img.name.c_str());
        writer.Key("x");
        writer.Int(img.x);
        writer.Key("y");
        writer.Int(img.y);
        writer.Key("width");
        writer.Int(img.width);
        writer.Key("height");
        writer.Int(img.height);
        writer.EndObject();
        }
    if (iSortedImageList.size() >= 2)
        writer.EndArray();

    writer.EndObject();

    std::ofstream metadataFile("metadata.json");
    metadataFile << buffer.GetString();
    if (!metadataFile.good())
        throw std::runtime_error("Can't write the JSON string to the file!");
};

// End Of File