//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     RADIM SAFAR <xsafar27@stud.fit.vutbr.cz>
// $Date:       $2022-03-2,
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author RADIM SAFAR
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//


// Setup

class EmptyTree : public ::testing::Test 
{
	protected:
		BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
	protected:
		virtual void SetUp() {
			int values[] = { 0, 94, 58, 14, 77, 47, 43, 66, 11, 6, 60 };

			for (int i = 0; i < 10; i++) {
				tree.InsertNode(i);
			}
		}

	BinaryTree tree;
};

class AxiomTree : public ::testing::Test
{
	protected:
	virtual void SetUp() {
		int values[] = { 99, 23, 76, 72, 7, 43, 36, 83, 12, 64, 24 };

		for (int i = 0; i < 10; i++) {
			tree.InsertNode(i);
		}
	}

	BinaryTree tree;
};

/**
 * EmptyTree tests
 * Insert node - Tries to insert node into empty tree
 * Delete node - Tries to delete nodes from empty tree
 * Find node	- Tries to find a node from empty tree
 */
TEST_F(EmptyTree, InsertNode) 
{
	EXPECT_TRUE(tree.InsertNode(1).first);
};

TEST_F(EmptyTree, DeleteNode) 
{
	EXPECT_FALSE(tree.DeleteNode(1));
};

TEST_F(EmptyTree, FindNode) 
{
	EXPECT_EQ(tree.FindNode(1), nullptr);
};


/**
 * NonEmptyTree tests
 * InsertNode	- Tries to insert node into non empty tree
 * DeleteNode	- Tries to delete node from non empty tree
 * FindNode	- Tries to delete node from non empty tree
 */
TEST_F(NonEmptyTree, InsertNode_NewNode) 
{
	EXPECT_TRUE(tree.InsertNode(10).first);
};

TEST_F(NonEmptyTree, InsertNode_ExistingNode)
{
	EXPECT_FALSE(tree.InsertNode(1).first);
}

TEST_F(NonEmptyTree, DeleteNode_ExistingNode)
{
	EXPECT_TRUE(tree.DeleteNode(1));
};

TEST_F(NonEmptyTree, DeleteNode_NonExistingNode)
{
	EXPECT_FALSE(tree.DeleteNode(11));
};

TEST_F(NonEmptyTree, FindNode_ExistingNode) 
{
	EXPECT_NE(tree.FindNode(1), nullptr);
};

TEST_F(NonEmptyTree, FindNode_NonExistingNode)
{
	EXPECT_EQ(tree.FindNode(11), nullptr);
};


/**
 * TreeAxioms tests
 * Axiom1	- All leaves are BLACK
 * Axiom2	- All RED nodes only have BLACK children
 * Axiom3	- Route from all leaves have the same amount of BLACK nodes to root
 */

TEST_F(AxiomTree, Axiom1) 
{
	std::vector<BinaryTree::Node_t *> v;
	tree.GetLeafNodes(v);
	
	for(int i = 0; i < v.size(); i++){
		EXPECT_EQ(v.at(i)->color, BLACK);
	}
};

TEST_F(AxiomTree, Axiom2)
{
	std::vector<BinaryTree::Node_t *> v;
	tree.GetNonLeafNodes(v);
	
	for(int i = 0; i < v.size(); i++){
		BinaryTree::Node_t *node = v.at(i);
		if (node->color == RED){
			EXPECT_EQ(node->pLeft->color, BLACK);
			EXPECT_EQ(node->pRight->color, BLACK);
		}
	}

};

TEST_F(AxiomTree, Axiom3)
{
	std::vector<BinaryTree::Node_t *> v;
	tree.GetLeafNodes(v);
	int amn = 0;
	int temp = 0;
	for(int i = 0; i < v.size(); i++){
		BinaryTree::Node_t *node = v.at(i);
		while(node->pParent != nullptr){
			if(node->color == BLACK){
				temp++;
			}
			node = node->pParent;
		}
		
		if(amn == 0){
			amn = temp;
		}

		EXPECT_EQ(amn, temp);
		temp = 0;
	}
};
/*** Konec souboru black_box_tests.cpp ***/
