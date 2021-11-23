//==============================================================================	 
// Name         : main.cpp
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Implements atlasgenerator Application Main Function
//==============================================================================

#include <vector>              // std::vector
#include <string>              // std::string
#include <iostream>            // std::cout
#include <stdexcept>           // std::runtime_error, std::logic_error
#include <dirent.h>            // DIR, dirent
#include "atlasgenerator.h"    // AtlasGenerator

//! Function To Get .png Files From Command Line Argument argv[1]
std::vector<std::string> GetpngFiles(const char* aArgv1);


//==============================================================================
//! Application Entry Point
//==============================================================================
int main(int argc, char* argv[])
{
    // argc should be 2: application executable name, path to the image folder
    if (argc < 4)
        {
        // tell the user how to run the application
        std::cout << "App usage: " << argv[0] << " <image folder> " << "<output file name> " << "<grid grow -- example : right or down or default>" << std::endl;
        std::cout << "If image folder path contains space, "
                  << "please put the path in double quote." << std::endl;
        return 1;
        }
    else    // argc is 2
        {
        try
            {
            std::vector<std::string> pngList = GetpngFiles(argv[1]);

            if (pngList.size() != 0)
                {
                AtlasGenerator atlasGenerator(pngList);
                std::cout << "Start generating texture atlas..." << std::endl;
                atlasGenerator.Run(argv[2],argv[3]);
                std::cout << "The texture atlas and it's metadata is successfully generated." << std::endl;
                }
            else
                {
                std::cout << "This folder does not contain any .png image." << std::endl;
                return 1;
                }
            }
        catch (const std::logic_error& err)
            {
            std::cout << err.what() << std::endl;
            return 1;
            }
        }

    return 0;
}


//==============================================================================
//! @brief Get .png Files From Command Line Argument argv[1]
//! @param aArgv1 Command Line Argument argv[1]
//! @return A List Of .png Files With Path
//==============================================================================
std::vector<std::string> GetpngFiles(const char* aArgv1)
{
    dirent* entry;
    std::vector<std::string> fileList;

    DIR* dir = opendir(aArgv1);
    if (dir != nullptr)
        {
        while ((entry = readdir(dir)) != nullptr)
            {
            if (entry->d_type == DT_REG)
                {
                std::string fileName = entry->d_name;
                std::string extension = ".png";
                if (fileName.find(extension, fileName.length() - extension.length())
                    != std::string::npos)
                    {
                    std::string filePathName;
                    std::string path(aArgv1);
                    filePathName = path + std::string("/") + fileName;
                    fileList.push_back(filePathName);
                    }
                }
            }
        closedir(dir);
        }
    else    // dir is nullptr
        {
        std::cout << "Please provide a valid directory" << std::endl;
        std::exit(EXIT_FAILURE);
        }

    return fileList;
}

// End Of File
