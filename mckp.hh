#ifndef MCKP_HH_
#define MCKP_HH_

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Item
{
    size_t item_id;
    int w;
    int p;

    Item() : item_id(0), w(0), p(0) {}
    Item(size_t item_id) : item_id(item_id), w(0), p(0) {}
    Item(size_t item_id, int w, int p) : item_id(item_id), w(w), p(p) {}

    bool operator==(const Item& item) const
    {
        return (
            this->item_id == item.item_id &&
            this->w == item.w &&
            this->p == item.p
        );
    }
};

struct N
{
    size_t sack_id;
    vector<Item*> items;

    N(size_t sack_id) : sack_id(sack_id) {}

    bool operator==(const N& sack) const
    {
        return this->sack_id == sack.sack_id;   
    }
};

struct hash_N
{
    size_t operator()(const N& sack) const
    {
        return sack.sack_id;
    }
};

struct weight_compare_item
{
    bool operator()(const Item* a, const Item* b) const
    {
        if (a->w == b->w)
        {
            return a->p > b->p;
        }

        return a->w < b->w;
    }

    bool operator()(const Item& a, const Item& b) const
    {
        if (a.w == b.w)
        {
            return a.p > b.p;
        }

        return a.w < b.w;
    }
};

using item_pair = pair<Item*, Item*>;
using N_item_pairs_map = unordered_map<N, vector<item_pair>, hash_N>;
using N_slope_map = unordered_map<N, double, hash_N>;

#endif