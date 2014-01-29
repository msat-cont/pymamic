/*
 *
 */

#include "named_array.hpp"
#include "mam_trees.hpp"

extern "C" {

static TreePool STreePool;

void TreePoolInitialize(const char *storage_path, int *status)
{
    *status = STreePool.Initialize(storage_path);
    return;
}

void TreePoolInsert(int tree_index, const char *item_name, unsigned data_length, \
    unsigned *data_values, int *status)
{
  *status = STreePool.Insert(tree_index, item_name, data_length, data_values);
  return;
}

void TreePoolQuery(int tree_index, const char *item_name, unsigned data_length, \
    unsigned *data_values, int range, int *status)
{
  *status = STreePool.LoadInRange(tree_index, item_name, data_length, data_values, range);
  return;
}

void TreePoolResult(int tree_index, int item_index, int name_buffer_len, char *item_name, int *distance, int *status)
{
    *status = STreePool.GetResult(tree_index, item_index, name_buffer_len, item_name, distance);
};

void TreePoolDispose(int index, int *status)
{
    STreePool.CleanResult(index);
    *status = 0;
    return;
};

void TreePoolFinalize(int index, int *status)
{
    STreePool.Finalize(index);
    *status = 0;
    return;
}

} // extern "C"


