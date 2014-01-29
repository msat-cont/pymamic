/*
 *
 */
#ifndef mam_trees_H__
#define mam_trees_H__

// Metric Tree includes
#include <arboretum/stMetricTree.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stDBMTree.h>

#include "named_array.hpp"

#define TREE_POOL_SIZE 8

typedef stResultPair < TNamedArray > narResultPair;
typedef stResult < TNamedArray > narResult;
typedef stDBMTree < TNamedArray, TNamedArrayDistanceEvaluator > narDBMTree;

/*
struct ResultItem
{
    public:
        ResultItem()
        {
            _name = "";
            _distance = -1;
            //_result = NULL;
        }

        string _name;
        int _distance;
};
*/

struct PoolItem
{
    public:
        stPlainDiskPageManager *_page_manager;
        narDBMTree *_tree;
        narResult *_result;
};

class TreePool
{
    public:
        TreePool()
        {
            _pool_size = TREE_POOL_SIZE;
            for (int i = 0; i < _pool_size; i++) {
                _pool[i]._page_manager = NULL;
                _pool[i]._tree = NULL;
                _pool[i]._result = NULL;
            }
        }

        ~TreePool()
        {
            Finalize(-1);
        }

        int Initialize(const char *storage_path);
        void Finalize(int tree_index);

        int Insert(int tree_index, const char *item_name, unsigned data_length, unsigned *data_values);
        int LoadInRange(int tree_index, const char *item_name, unsigned data_length, unsigned *data_values, int range);
        int GetResult(int tree_index, int item_index, int name_buffer_len, char *item_name, int *distance);
        void CleanResult(int tree_index);

    private:
        PoolItem _pool[TREE_POOL_SIZE];
        int _pool_size;

};

#endif //end mam_trees_H__
