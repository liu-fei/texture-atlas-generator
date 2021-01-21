//========================================================================================
// Name         : binarytreealgorithm.cpp
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Implements BinaryTreeAlgorithm Class
//========================================================================================

#include "binarytreealgorithm.h"
#include <iostream> // std::cout

//========================================================================================
//! @brief Starts Packing Process To A Texture Atlas Canvas Of The Given Dimension
//! @param aAtlasWidth The Atlas's Width
//! @param aAtlasHeight The Atlas's Height
//========================================================================================
void BinaryTreeAlgorithm::Init(const int aAtlasWidth, const int aAtlasHeight)
{
    // initiate the root node which represents the initiate whole empty atlas rectangle
    // with top-left coordinate (0,0) and dimension aAtlasWidth, aAtlasHeight
    iRootNode = new Node(0, 0, aAtlasWidth, aAtlasHeight);
}

//========================================================================================
//! @brief Inserts The Image In The Binary Tree
//! @param aImgWidth The The Image's Width
//! @param aImgHeight The The Image's Height
//! @return The Pointer To The Node That Stores The Image
//!         or nullptr If It Didn't Fit In The Binary Tree
//========================================================================================
Node *BinaryTreeAlgorithm::Insert(const int aImgWidth, const int aImgHeight) const
{
    return Insert(iRootNode, aImgWidth, aImgHeight);
}

//========================================================================================
//! @brief Inserts An Image In The Subtree Rooted At The Given Node
//! @param aNode The Given Node Where To Insert The Image
//! @param aImgWidth The The Image's Width
//! @param aImgHeight The The Image's Height
//! @return The Pointer To The Node That Stores The Newly Added Image
//          or nullptr If It Didn't Fit
//========================================================================================
Node *BinaryTreeAlgorithm::Insert(Node *aNode, const int aImgWidth, const int aImgHeight) const
{
    if (aNode->isUsed)
    {
        // recursively try aNode's children branchs
        if (Node *here = Insert(aNode->rightChild, aImgWidth, aImgHeight))
            return here;
        else if (Node *there = Insert(aNode->downChild, aImgWidth, aImgHeight))
            return there;
        else
            return nullptr;
    }
    // the image fits in this node
    else if (aImgWidth <= aNode->width && aImgHeight <= aNode->height)
        return aNode;
    else // the image can not fit in this node
        return nullptr;
}

//========================================================================================
//! @brief SplitNode Happens Every Time After Placing An Image
//         On The Top Left Corner Of An Empty Rectangular
//! @param aNode The Node Where The Image Just Inserted
//! @param aImgWidth The Image's Width
//! @param aImgHeight The Image's Height
//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
//========================================================================================
void BinaryTreeAlgorithm::SplitNode(Node *aNode, const int aImgWidth, const int aImgHeight, const int aImgID)
{
    aNode->rightChild = new Node(aNode->x + aImgWidth, aNode->y, aNode->width - aImgWidth, aImgHeight);
    aNode->downChild = new Node(aNode->x, aNode->y + aImgHeight, aNode->width, aNode->height - aImgHeight);

    aNode->isUsed = true;
    aNode->imgID = aImgID;
}

//========================================================================================
//! @brief GrowAtlasCanvas Provides An Algorithm Which Decides Where The Canvas Should Expend,
//         On Its Right Side by Call GrowRight Method Or On Its Down Side by Call GrowDown Method
//! @param aImgWidth The Image's Width
//! @param aImgHeight The Image's Height
//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
//========================================================================================
Node *BinaryTreeAlgorithm::GrowAtlasCanvas(const int aImgWidth, const int aImgHeight, const int aImgID)
{
    bool canGrowRight = (aImgHeight <= iRootNode->height);
    bool canGrowDown = (aImgWidth <= iRootNode->width);

    bool shouldGrowRight = canGrowRight && (iRootNode->height >= (iRootNode->width + aImgWidth));
    bool shouldGrowDown = canGrowDown && (iRootNode->width >= (iRootNode->height + aImgHeight));

    if (shouldGrowRight)
        return GrowRight(aImgWidth, aImgHeight, aImgID);
    else if (shouldGrowDown)
        return GrowDown(aImgWidth, aImgHeight, aImgID);
    else if (canGrowRight)
        return GrowRight(aImgWidth, aImgHeight, aImgID);
    else if (canGrowDown)
        return GrowDown(aImgWidth, aImgHeight, aImgID);
    else
        return nullptr;
}

//========================================================================================
//! @brief GrowAtlasCanvas Grows Canvas On Right Side
//! @param aImgWidth The Image's Width
//! @param aImgHeight The Image's Height
//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
//! @return The Pointer To The Node That Stores The Newly Added Image
//========================================================================================
Node *BinaryTreeAlgorithm::GrowRight(int aImgWidth, int aImgHeight, const int aImgID)
{
    Node *newRoot = new Node(0, 0, iRootNode->width + aImgWidth, iRootNode->height);
    newRoot->isUsed = true;

    // for new image
    newRoot->rightChild = new Node(iRootNode->width, 0, aImgWidth, iRootNode->height);
    newRoot->downChild = iRootNode;

    iRootNode = newRoot;

    // insert the new image in the new root node
    if (Node *node = Insert(iRootNode, aImgWidth, aImgHeight))
    {
        SplitNode(node, aImgWidth, aImgHeight, aImgID);
        return node;
    }
    else
        return nullptr;
}

int BinaryTreeAlgorithm::DownChildLength(int count, Node *node)
{

    if (node->downChild)
        return DownChildLength(count + 1, node->downChild);
    else
        return count;

    return count;
}

int BinaryTreeAlgorithm::RightChildLength(int count, Node *node)
{

    if (node->rightChild)
        return RightChildLength(count + 1, node->downChild);
    else
        return count;

    return count;
}

//========================================================================================
//! @brief GrowAtlasCanvas Grows Canvas On Down Side
//! @param aImgWidth The Image's Width
//! @param aImgHeight The Image's Height
//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
//! @return The Pointer To The Node That Stores The Newly Added Image
//========================================================================================
Node *BinaryTreeAlgorithm::GrowDown(int aImgWidth, int aImgHeight, const int aImgID)
{
    Node *newRoot = new Node(0, 0, iRootNode->width, iRootNode->height + aImgHeight);
    newRoot->isUsed = true;

    newRoot->rightChild = iRootNode;
    newRoot->downChild = new Node(0, iRootNode->height, iRootNode->width, aImgHeight);

    iRootNode = newRoot;

    if (Node *node = Insert(iRootNode, aImgWidth, aImgHeight))
    {
        SplitNode(node, aImgWidth, aImgHeight, aImgID);
        return node;
    }
    else
        return nullptr;
}

// End Of File