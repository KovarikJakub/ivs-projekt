//======= Copyright (c) 2024, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JAKUB KOVARIK <xkovarj00@stud.fit.vutbr.cz>
// $Date:       $2024-02-14
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JAKUB KOVARIK
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"
#include "red_black_tree.h"
#include <gmock/gmock.h>


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

// TEST - Klasicky ytest
// TEST_F - Test pro Fixture

using namespace ::testing;

// common base class for our fixture classes.
// allows for easy accesss to leaf nodes or all nodes.
class BinaryTreeFixtureBase : public Test
{
protected:
    std::vector<Node_t*> GetAllNodes() 
    {
        std::vector<Node_t*> allNodes;
        instance.GetAllNodes(allNodes);
        return allNodes;
    }

    std::vector<Node_t*> GetLeafNodes() 
    {
        std::vector<Node_t*> leafNodes;
        instance.GetLeafNodes(leafNodes);
        return leafNodes;
    }

    BinaryTree instance;
};

//
//
// EMPTY TREE TESTS

class EmptyTree : public Test //public BinaryTreeFixtureBase
{
protected:
    void SetUp() override 
    {
    }
        std::vector<Node_t*> GetAllNodes() 
    {
        std::vector<Node_t*> allNodes;
        instance.GetAllNodes(allNodes);
        return allNodes;
    }

    std::vector<Node_t*> GetLeafNodes() 
    {
        std::vector<Node_t*> leafNodes;
        instance.GetLeafNodes(leafNodes);
        return leafNodes;
    }

    BinaryTree instance;

};
 
TEST_F(EmptyTree, InsertNode){

    const int key = 2;

    const auto resultPair = instance.InsertNode(key);

    ASSERT_TRUE(resultPair.first);
    EXPECT_NE(resultPair.second, nullptr);

    EXPECT_EQ(GetLeafNodes().size(), 2);

    const auto allNodes = GetAllNodes();
    const auto firstNode = allNodes[0];

    ASSERT_NE(firstNode, nullptr);
    ASSERT_NE(firstNode->pLeft, nullptr);
    ASSERT_NE(firstNode->pRight, nullptr);
    ASSERT_EQ(firstNode->pParent, nullptr);
    EXPECT_THAT(allNodes, UnorderedElementsAre(Field(&Node_t::key, 2),
                                               Field(&Node_t::key, 0),
                                               Field(&Node_t::key, 0)));
    const auto leftLeaf = firstNode->pLeft;
    const auto rightLeaf = firstNode->pRight;
}

TEST_F(EmptyTree, DeleteNode)
{
    EXPECT_FALSE(instance.DeleteNode(17));
    EXPECT_TRUE(GetAllNodes().empty());
}

TEST_F(EmptyTree, FindNode)
{
    EXPECT_EQ( instance.FindNode(1), nullptr);
}

//
//
// NON-EMPTY TREE TEST

class NonEmptyTree : public Test //public BinaryTreeFixtureBase
{
    protected:
    void SetUp() override 
    {
        const auto initialKeys = {1,2,3};
        instance.InsertNodes(initialKeys, outInitialNodes);
    }

    std::vector<Node_t*> GetAllNodes() 
    {
        std::vector<Node_t*> allNodes;
        instance.GetAllNodes(allNodes);
        return allNodes;
    }

    std::vector<Node_t*> GetLeafNodes() 
    {
        std::vector<Node_t*> leafNodes;
        instance.GetLeafNodes(leafNodes);
        return leafNodes;
    }

    BinaryTree instance;

    std::vector<std::pair<bool, Node_t*>> outInitialNodes;
};

TEST_F(NonEmptyTree, InsertNode)
{
    // Insert a existing node:
    auto keyToInsert = 3;

    auto resultPair = instance.InsertNode(keyToInsert);
    EXPECT_FALSE(resultPair.first); 
    EXPECT_NE(resultPair.second, nullptr);

    EXPECT_THAT(GetLeafNodes().size(), 4);

    EXPECT_THAT(GetAllNodes(), UnorderedElementsAre(Field(&Node_t::key, 1),
                                                    Field(&Node_t::key, 2),
                                                    Field(&Node_t::key, 3),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0)));

    
    // Insert a new, non-existing node:
    keyToInsert = 5;  

    resultPair = instance.InsertNode(keyToInsert); 
    EXPECT_TRUE(resultPair.first);
    EXPECT_NE(resultPair.second, nullptr);

    EXPECT_THAT(GetLeafNodes().size(), 5);
    EXPECT_THAT(GetAllNodes(), UnorderedElementsAre(Field(&Node_t::key, 1),
                                                    Field(&Node_t::key, 2),
                                                    Field(&Node_t::key, 3),
                                                    Field(&Node_t::key, 5),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0)));
}

TEST_F(NonEmptyTree, FindNode)
{
    // Find a existing node
    int keyToFind = 3;

    auto node = instance.FindNode(keyToFind);

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->key, keyToFind);

    keyToFind = 9;
    node = instance.FindNode(keyToFind);

    EXPECT_EQ(node, nullptr);
}

TEST_F(NonEmptyTree, DeleteNode)
{
    // Delete a non-existing node:
    int keyToDelete = 9;
    
    EXPECT_FALSE(instance.DeleteNode(keyToDelete));
    EXPECT_EQ(GetLeafNodes().size(), 4);
    EXPECT_THAT(GetAllNodes(), UnorderedElementsAre(Field(&Node_t::key, 1),
                                                    Field(&Node_t::key, 2),
                                                    Field(&Node_t::key, 3),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0)));
    // Delete existing node:
    keyToDelete = 3;
    
    EXPECT_TRUE(instance.DeleteNode(keyToDelete));
    EXPECT_EQ(GetLeafNodes().size(), 3);
    EXPECT_THAT(GetAllNodes(), UnorderedElementsAre(Field(&Node_t::key, 1),
                                                    Field(&Node_t::key, 2),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0),
                                                    Field(&Node_t::key, 0)));

}


//
//
// AXIOM TESTS

// AXIOM #1
TEST_F(NonEmptyTree, LeafsAreBlack)
{
    const auto leafNodes = GetLeafNodes();
    for(const auto& leaf: leafNodes)
    {
        EXPECT_EQ(leaf->color, Color_t::BLACK);
    }
}

// AXIOM #2
TEST_F(NonEmptyTree, RedNodesChildrenAreBlack)
{
    const auto allNodes = GetAllNodes();
    for(const auto& node: allNodes)
    {
        ASSERT_NE(node, nullptr);

        if(node->color == Color_t::RED)
        {
            const auto leftChild = node->pLeft;
            const auto rightChild = node->pRight;

            ASSERT_NE(leftChild, nullptr);
            ASSERT_NE(rightChild, nullptr);

            EXPECT_EQ(leftChild->color, Color_t::BLACK);
            EXPECT_EQ(rightChild->color, Color_t::BLACK);
        }
    }
}

// AXIOM #3
TEST_F(NonEmptyTree, ConsistentBlackNodeCounts)
{
    auto leafNodes = GetLeafNodes();

    std::vector<int> blackNodesCoutsAcrossPaths(leafNodes.size());     
    int pathNum = 0; 

    for(Node_t* leaf: leafNodes)
    {
        // for each leaf:

        Node_t* current = leaf;

        // climb the binary tree to the root:
        while(current->pParent != nullptr)
        {   
            // count all the black nodes for each path:
            if(current->color == Color_t::BLACK)
            {
                blackNodesCoutsAcrossPaths[pathNum]++;
            }

            current = current->pParent;
        }
        
        ++pathNum;
    }

    EXPECT_FALSE(blackNodesCoutsAcrossPaths.empty());

    auto allSame = [](const std::vector<int> vec)
    {
        auto firstValue = vec[0];
        return std::all_of(vec.begin(), vec.end(),[firstValue](int val) { return val == firstValue; });
    };

    EXPECT_TRUE(allSame(blackNodesCoutsAcrossPaths));
}

/*** Konec souboru black_box_tests.cpp ***/
