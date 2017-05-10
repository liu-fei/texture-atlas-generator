//==============================================================================
// Name         : binarytreealgorithm.h
// Author       : Fei Liu (liu.s.fei@gmail.com)
// Description  : Declares BinaryTreeAlgorithm Class
//==============================================================================

#ifndef BINARYTREEALGORTHM_H
#define BINARYTREEALGORTHM_H


//==============================================================================
//! Node Struct
//! Node Is A Node Of A Binary Tree
//! Each Node Represents A Rectangular Area On The Texture Atlas
//==============================================================================
struct Node
{
	//! @brief Constructor For Creating A New Node
	//! @param aX The X Coordinate Of The Top-Left Point Of The Rectangle
	//! @param aY The Y Coordinate Of The Top-Left Point Of The Rectangle
	//! @param aWidth The Width Of The Rectangle
	//! @param aHeight The Height Of The Rectangle
	Node(int aX, int aY, int aWidth, int aHeight)
		: x(aX), y(aY), width(aWidth), height(aHeight), isUsed(false), 
		  imgID(-1), rightChild(nullptr), downChild(nullptr)
	{
	};

	// the top-left coordinate of the rectangle
	int x;
	int y;

	// the dimension of the rectangle
	int width;
	int height;

	// is this Node used or not
	bool isUsed;
	
	int imgID;
	
	// after placing an image on the top left corner of an empty rectangular
	// the remaining space will be splitted into 2 rectangles by the    
	// horizontal line where the image's bottom line is
	// ---------------------
	// |  img | rightChild |
	// |--------------------     <== split by the image's bottom line
	// |                   |
	// |   downChild       |
	// |                   |
	// |                   |
	// ---------------------
	Node* rightChild;
	Node* downChild;
};


//==============================================================================
//! BinaryTreeAlgorithm Class
//==============================================================================
class BinaryTreeAlgorithm
{
	public:
		//! @brief Starts A New Packing Process To A Texture Atlas Canvas Of The Given Dimension
		//! @param aAtlasWidth The Atlas Canvas's Width
		//! @param aAtlasHeight The Atlas Canvas's Height
		void Init(const int aAtlasWidth, const int aAtlasHeight);

		//! @brief Inserts The Image In The Binary Tree
		//! @param aImgWidth The Image's Width
		//! @param aImgHeight The Image's Height
		//! @return The Pointer To The Node That Stores The Image
		//!         or nullptr If It Didn't Fit In The Binary Tree
		Node* Insert(const int aImgWidth, const int aImgHeight) const;
		
		//! @brief SplitNode Happens Every Time After Placing An Image 
		//         On The Top Left Corner Of An Empty Rectangular
		//! @param aNode The Node Where The Image Just Inserted  
		//! @param aImgWidth The Image's Width
		//! @param aImgHeight The Image's Height
		//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
		void SplitNode(Node* aNode, const int aImgWidth, const int aImgHeight, const int aImgID);

		//! @brief GrowAtlasCanvas Provides An Algorithm Which Decides Where The Canvas Should Expend,
		//         On Its Right Side by Call GrowRight Method Or On Its Down Side by Call GrowDown Method
		//! @param aImgWidth The Image's Width
		//! @param aImgHeight The Image's Height
		//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
		Node*  GrowAtlasCanvas(const int aImgWidth, const int aImgHeight, const int aImgID);

		//! @brief GrowAtlasCanvas Grows Canvas On Right Side
		//! @param aImgWidth The Image's Width
		//! @param aImgHeight The Image's Height
		//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
		//! @return The Pointer To The Node That Stores The Newly Added Image
		Node*  GrowRight(int aWide, int aHeight, const int aImgID);

		//! @brief GrowAtlasCanvas Grows Canvas On Down Side
		//! @param aImgWidth The Image's Width
		//! @param aImgHeight The Image's Height
		//! @param aImgID The Corresponding Index Of The Image In iSortedImageList Is Used As aImgID
		//! @return The Pointer To The Node That Stores The Newly Added Image
		Node*  GrowDown(int aWide, int aHeight, const int aImgID);

		//! @brief Get the rootNode
		Node*  rootNode()
		{
			return iRootNode;
		};

    private:
		//! @brief Inserts An Image In The Subtree Rooted At The Given Node
		//! @param aNode The Given Node Where To Insert The Image
		//! @param aImgWidth The The Image's Width
		//! @param aImgHeight The The Image's Height
		//! @return The Pointer To The Node That Stores The Newly Added Image
		//!         or nullptr If It Didn't Fit In The Binary Tree
		Node* Insert(Node* aNode, const int aImgWidth, const int aImgHeight) const;
	
	private:
		Node* iRootNode;
};

#endif    // BINARYTREEALGORTHM_H

// End Of File

