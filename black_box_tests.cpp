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

class EmptyTree : public Test
{
protected:
    void SetUp() override 
    {
    }

    std::vector<Node_t*> GetAllNodes() 
    {
        std::vector<Node_t*> leafNodes;
        instance.GetAllNodes(leafNodes);
        return leafNodes;
    }

    BinaryTree instance;
};
 
TEST_F(EmptyTree, InsertNode){

    // ARRANGE
    const int key = 2;

    // ACT
    const auto resultPair = instance.InsertNode(key);

    // ASSERT
    ASSERT_TRUE(resultPair.first);
    EXPECT_NE(resultPair.second, nullptr);

    const auto allNodes = GetAllNodes();
     auto firstNode = allNodes[0];

    ASSERT_NE(firstNode, nullptr);
    ASSERT_EQ(firstNode->pLeft, nullptr);
    ASSERT_EQ(firstNode->pRight, nullptr);
    ASSERT_EQ(firstNode->pParent, nullptr);
    EXPECT_THAT(allNodes, UnorderedElementsAre(Field(&Node_t::key, 1),
                                               Field(&Node_t::key, 0),
                                               Field(&Node_t::key, 0)));
    const auto leftLeaf = firstNode->pLeft;
    const auto rightLeaf = firstNode->pRight;
}

TEST(EmptyTree, DeleteNode)
{
    // ARRANGE
    const int key = 17

    // ACT
    EXPECT_EQ(binaryTree.DeleteNode(key));
    
    // ASSERT

    std::vector<Node_t*> outAllNodes;
    binaryTree.GetAllNodes(outAllNodes);
    EXPECT_EQ(outAllNodes.empty(), true);
}


class NonEmptyTree : public ::testing::Test
{
    protected:
    void SetUp() override 
    {
        const auto initialKeys = {1,2,3,4,5};
        instance.InsertNodes(initialKeys, outInitialNodes);
    }

    void TearDown() override 
    {
    }

    BinaryTree instance;
    std::vector<std::pair<bool, Node_t*>> outInitialNodes;
};

TEST_F(NonEmptyTree, InsertNode_Existing)
{
    // ARRANGE
    const auto keyToInsert = 3;

    // ACT
    const auto outNode = instance.InsertNode(keyToInsert);

    // ASSERT
    EXPECT_NE(outNode.second, nullptr);
    EXPECT_FALSE(outNode.first);
}

TEST_F(NonEmptyTree, FindNode_Existing)
{
    // ARRANGE
    const int keyToFind = 3;

    // ACT
    const auto node = instance.FindNode(keyToFind);

    // ASSERT
    EXPECT_EQ(node->key, keyToFind);
}

TEST_F(NonEmptyTree, FindNode_NonExisting)
{
    // ARRANGE
    const int keyToFind = 7;

    // ACT
    const auto node = instance.FindNode(keyToFind);
    
    // ASSERT
    EXPECT_EQ(node, nullptr);
}

TEST_F(NonEmptyTree, DeleteNode_Existing)
{
    // ARRANGE
    const int keyToDelete = 3;
    
    // ACT
    const bool deleteResult = instance.DeleteNode(keyToDelete);

    // ASSERT
    std::vector<Node_t *> outAllNodes;
    instance.GetAllNodes(outAllNodes);

    //EXPECT_TRUE(deleteResult);
    std::vector<int> actualKeys;
    for(const auto& node: outAllNodes)
    {
        actualKeys.push_back(node->key);
    }

    bool status = actualKeys.size() > 4 &&  actualKeys.size() < 6;
    EXPECT_TRUE(status);
    //EXPECT_THAT(actualKeys, ::testing::UnorderedElementsAre(1,2,4,5));

    //ASSERT_EQ(outLeafNodes.size(), 4);
    //EXPECT_EQ(outLeafNodes[0]->key, 1);
    //EXPECT_EQ(outLeafNodes[1]->key, 2);
    //EXPECT_EQ(outLeafNodes[2]->key, 4);
    //EXPECT_EQ(outLeafNodes[3]->key, 5);
}

/*** Konec souboru black_box_tests.cpp ***/
