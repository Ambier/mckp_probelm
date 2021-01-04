#ifndef MCKP_HELPERS_HH_
#define MCKP_HELPERS_HH_

#include "mckp.hh"
#include "unordered_map"
#include <algorithm>

using namespace std;

void check_dominating_elements(N& sack, bool sort_sack=false);
void make_item_pairs(vector<N>& sacks, N_item_pairs_map& map, bool sort_sacks=true);
void derive_slope(vector<N>& sacks, N_item_pairs_map& map, N_slope_map& y);

void check_dominating_elements(N& sack, bool sort_sack)
{
    if (sort_sack)
    {
        sort(sack.items.begin(), sack.items.end(), weight_compare_item());
    }

    auto first_elem = sack.items.begin();
    auto second_elem = sack.items.begin();
    for (auto it = sack.items.begin(); it != sack.items.end(); ++it)
    {
        if ((*first_elem)->w <= (*it)->w && (*first_elem)->p >= (*it)->p)
        {
            second_elem = it;
        }
        else if (first_elem != second_elem)
        {
            it = sack.items.erase(next(first_elem), next(second_elem));
            first_elem = it;
            second_elem = it;
        }
    }

    if (first_elem != second_elem)
    {
        sack.items.erase(next(first_elem), next(second_elem));
    }
}

///
/// @brief  Makes pairs
///
/// @arg    sacks
/// @arg    map
///
/// NOTE:   Function Presumes all vectors have been already allocated on the calling function stack
///         This helps reduce heap memory usage & speed up the program
void make_item_pairs(vector<N>& sacks, N_item_pairs_map& map, bool sort_sacks)
{
    for (auto sack = sacks.begin(); sack != sacks.end(); ++sack)
    {
        // This sorts and removes elements that are dominated by others
        check_dominating_elements(*sack, sort_sacks);

        size_t i = 0;
        while(i < sack->items.size()-1)
        {
            size_t j = i+1;
            while (j < sack->items.size() && sack->items[i]->p >= sack->items[j]->p)
            {
                j++;
            }

            if (j < sack->items.size())
            {
                map[*sack].push_back(make_pair(sack->items[i], sack->items[j]));
            }
            i = j+1;
        }
    }
}

///
/// @brief  Derive Slope
///
/// @arg    sacks
/// @arg    map
/// @arg    y
///
/// NOTE:
///
void derive_slope(vector<N>& sacks, N_item_pairs_map& map, N_slope_map& y)
{
    for (auto sack = sacks.begin(); sack != sacks.end(); ++sack)
    {
        vector<item_pair>& item_pairs = map[*sack];
        if (!item_pairs.size())
        {
            continue;
        }
        
        vector<double> slopes;
        for (size_t i = 0; i < item_pairs.size(); i++)
        {
            slopes.push_back(
                (item_pairs[i].second->p - item_pairs[i].first->p) /
                (item_pairs[i].second->w - item_pairs[i].first->w)
            );
        }
        sort(slopes.begin(), slopes.end());

        size_t mid = slopes.size() / 2;
        if (slopes.size() % 2)
        {
            y[*sack] = (slopes[mid-1] + slopes[mid]) / 2;
        }
        else
        {
            y[*sack] = slopes[mid];
        }
    }
}

#endif