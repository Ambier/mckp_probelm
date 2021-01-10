#include "mckp.hh"
#include "mckp_helpers.hh"
#include <gtest/gtest.h>
#include <vector>

using namespace std;

#define SACK_ITEM_LIMIT 10

TEST(mckp_helpers_tests, make_pairs_test)
{

    /// NOTE: ADD TEST CASES HERE

    // Test 1 - No Dominating Element
    // Test 2 - Index 2 dominates Index 0
    // Test 3 - Index 2 dominates 2 elements


    int weights[][SACK_ITEM_LIMIT] = {
        {4, 2, 3, 1},               // Test 1
        {4, 2, 3, 1, 5},            // Test 2
        {4, 2, 3, 3, 1, 5}          // Test 3
    };
    int prices[][SACK_ITEM_LIMIT] = {
        {20, 10, 15, 5},            // Test 1
        {15, 10, 20, 5, 25},        // Test 2
        {20, 10, 15, 20, 5, 25}     // Test 3
    };

    // Number of Items in each sack
    size_t n[] = {
        4,
        5,
        6
    };

    pair<int,int> final_pairs_order[][SACK_ITEM_LIMIT/2] = {
        {make_pair(3,1), make_pair(2,0)},
        {make_pair(3,1), make_pair(2,4)},
        {make_pair(4,1), make_pair(3,5)}
    };

    size_t final_pairs_size[] = {
        2,
        2,
        2
    };

    /// NOTE: DO NOT MOIFY BELOW

    size_t num_sacks = sizeof(n) / sizeof(n[0]);

    vector<vector<Item>> items(num_sacks);

    for (size_t i = 0; i < num_sacks; i++)
    {
        for (size_t j = 0; j < n[i]; j++)
        {
            items[i].push_back(Item());
            items[i][j].item_id = j;
            items[i][j].w = weights[i][j];
            items[i][j].p = prices[i][j];
        }
    }

    vector<N> sacks;
    for (size_t i = 0; i < num_sacks; i++)
    {
        sacks.push_back(N(i));
    }

    for (size_t i = 0; i < num_sacks; i++)
    {
        for (size_t j = 0; j < n[i]; j++)
        {
            sacks[i].items.push_back(&items[i][j]);
        }
    }

    N_item_pairs_map map;
    for (size_t i = 0; i < sacks.size(); i++)
    {
        map[sacks[i]] = vector<item_pair>();
    }

    make_item_pairs(sacks, map);

    for (size_t i = 0; i < num_sacks; i++)
    {
        ASSERT_EQ(map[sacks[i]].size(), (size_t) final_pairs_size[i]);
        for (size_t j = 0; j < final_pairs_size[i]; j++)
        {
            EXPECT_EQ(map[sacks[i]][j].first->item_id, final_pairs_order[i][j].first);            
            EXPECT_EQ(map[sacks[i]][j].second->item_id, final_pairs_order[i][j].second);
        }
    }
}

TEST(mckp_helper_test, check_dominating_elements_test)
{

    /// NOTE: Add Test Cases Here

    Item input_items[][SACK_ITEM_LIMIT] = {
        {Item(0,1,2), Item(1,2,3), Item(2,3,4), Item(3,4,5), Item(4,5,6)},
        {Item(0,1,2), Item(1,1,1), Item(2,2,2), Item(3,2,1), Item(4,6,6), Item(5,5,6)},
        {Item(0,1,4), Item(1,1,2), Item(2,1,3), Item(3,2,4), Item(4,2,5), Item(5,6,7)}
    };

    size_t input_n[] = {
        5,
        6,
        6
    };

    Item output_items[][SACK_ITEM_LIMIT] = {
        {Item(0,1,2), Item(1,2,3), Item(2,3,4), Item(3,4,5), Item(4,5,6)},
        {Item(0,1,2), Item(5,5,6)},
        {Item(0,1,4), Item(4,2,5), Item(5,6,7)}
    };
    
    size_t output_n[] = {
        5,
        2,
        3
    };

    /// NOTE: Do Not Modify Below

    size_t num_sacks = sizeof(input_n) / sizeof(input_n[0]);

    vector<N> sacks;
    for (size_t i = 0; i < num_sacks; i++)
    {
        sacks.push_back(N(i));
    }

    for (size_t i = 0; i < num_sacks; i++)
    {
        for (size_t j = 0; j < input_n[i]; j++)
        {
            sacks[i].items.push_back(&input_items[i][j]);
        }
    }

    for (size_t i = 0; i < num_sacks; i++)
    {
        check_dominating_elements(sacks[i], true);

        ASSERT_EQ(sacks[i].items.size(), output_n[i]);
        for(size_t j = 0; j < output_n[i]; j++)
        {
            EXPECT_EQ(*(sacks[i].items[j]), output_items[i][j]);
        }
    }
}

TEST(mckp_helper_tests, quickselect_test)
{
    int arr[][SACK_ITEM_LIMIT] = {
        {4, 5, 2, 6, 3, 1},
        {1, 2, 3, 4, 5, 6, 7},
        {9, 10, 4, 1, 5, 6, 8, 7, 3, 2}
    };

    int arr_size[] = {
        6,
        7,
        10
    };

    int results[] = {
        3,
        4,
        5   
    };

    /// NOTE: Don't edit below

    size_t n = sizeof(arr) / sizeof(arr[0]);

    for (size_t i = 0; i < n; i++)
    {
        int val = quickSelect<int>(arr[i], 0, arr_size[i]-1, (arr_size[i]-1) / 2);
        EXPECT_EQ(val, results[i]);
    }

}

TEST(mckp_helper_tests, derive_slope_test)
{
    pair<int,int> input_pairs_w[][SACK_ITEM_LIMIT/2] = {
        {make_pair(1,3), make_pair(1,3), make_pair(1,3)},
        {make_pair(1,3), make_pair(1,3), make_pair(1,3), make_pair(1,3)}
    };

    pair<int,int> input_pairs_p[][SACK_ITEM_LIMIT/2] = {
        {make_pair(1,3), make_pair(1,2), make_pair(1,10)},
        {make_pair(1,3), make_pair(1,4), make_pair(1,10), make_pair(1,5)}
    };

    size_t num_pairs[] = {
        3,
        4
    };

    double results[] = {
        1,
        1.5
    };

    /// NOTE: Don't edit below

    size_t num_sacks = sizeof(input_pairs_w) / sizeof(input_pairs_w[0]);

    N_item_pairs_map map;

    vector<N> sacks;
    for (size_t i = 0; i < num_sacks; i++)
    {
        sacks.push_back(N(i));
        map[sacks[i]] = vector<item_pair>();
        for (size_t j = 0; j < num_pairs[i]; j++)
        {
            map[sacks[i]].push_back(
                make_pair(
                    new Item(2*j, input_pairs_w[i][j].first, input_pairs_p[i][j].first), 
                    new Item(2*j+1, input_pairs_w[i][j].second, input_pairs_p[i][j].second)
                )
            );
        }
    }

    N_slope_map y;
    derive_slope(sacks, map, y);

    for (size_t i = 0; i < num_sacks; i++)
    {
        EXPECT_EQ(y[i], results[i]);
    }
    
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}