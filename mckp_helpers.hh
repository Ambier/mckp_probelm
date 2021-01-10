#ifndef MCKP_HELPERS_HH_
#define MCKP_HELPERS_HH_

#include "mckp.hh"
#include "unordered_map"
#include <algorithm>

using namespace std;

void check_dominating_elements(N& sack, bool sort_sack=false);
void make_item_pairs(vector<N>& sacks, N_item_pairs_map& map, bool sort_sacks=true);
void derive_slope(vector<N>& sacks, N_item_pairs_map& map, N_slope_map& y);

template <typename T>
int partition(T arr[], int low, int high) 
{ 
    T pivot = arr[high]; 
    int i = (low - 1); 
    for (int j = low; j < high; j++) 
    { 
        if (arr[j] <= pivot) 
        { 
            i++; 
            swap(arr[i], arr[j]); 
        } 
    } 
    swap(arr[i + 1], arr[high]); 
    return (i + 1);
}

template <typename T>
T quickSelect(T a[], int left, int right, int k) 
{
    while (left <= right) 
    {
        // Partition around a pivot and then check its position
        // and find the position of the pivot 
        int pivotIndex = partition(a, left, right); 
  
        if (pivotIndex == k)
        {
            return a[pivotIndex];
        }
        else if (pivotIndex > k)
        {
            right = pivotIndex - 1; 
        }  
        else
        {
            left = pivotIndex + 1; 
        }
    } 
    return -1; 
} 

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
        
        double slopes[item_pairs.size()];
        for (size_t i = 0; i < item_pairs.size(); i++)
        {
            slopes[i] = (
                (double) (item_pairs[i].second->p - item_pairs[i].first->p) /
                (double) (item_pairs[i].second->w - item_pairs[i].first->w)
            );
        }

        // Set the median slope for the respective sack
        y[*sack] = quickSelect<double>(slopes, 0, item_pairs.size()-1, 
            (item_pairs.size()-1) / 2);
    }
}

#endif