/*
 *
 */

#include "mam_trees.hpp"

int TreePool::Initialize(const char *storage_path)
{
    int use = -1;

    for (int i = 0; i < _pool_size; i++)
    {
        if ((!_pool[i]._page_manager) && (!_pool[i]._tree))
        {
            use = i;
            _pool[i]._page_manager = new stPlainDiskPageManager(storage_path, 1024);
            _pool[i]._tree = new narDBMTree(_pool[i]._page_manager);
            break;
        }
    }

    return use;
}

void TreePool::Finalize(int tree_index)
{
    if ((0 <= tree_index) && (tree_index < _pool_size))
    {
        if (_pool[tree_index]._page_manager)
        {
            delete _pool[tree_index]._page_manager;
        }
        if (_pool[tree_index]._tree)
        {
            delete _pool[tree_index]._tree;
        }
        if (_pool[tree_index]._result)
        {
            delete _pool[tree_index]._result;
        }
    }

    if (-1 == tree_index) {
        for (int i = 0; i < _pool_size; i++)
        {
            if (_pool[i]._page_manager)
            {
                delete _pool[i]._page_manager;
            }
            if (_pool[i]._tree)
            {
                delete _pool[i]._tree;
            }
            if (_pool[i]._result)
            {
                delete _pool[i]._result;
            }
        }
    }
}

int TreePool::Insert(int tree_index, const char *item_name, unsigned data_length, unsigned *data_values)
{
    if ((0 > tree_index) || (tree_index >= _pool_size))
    {
        return -1;
    }
    if (!_pool[tree_index]._tree)
    {
        return -2;
    }

    TNamedArray *item = new TNamedArray(item_name, data_length, data_values);
    _pool[tree_index]._tree->Add(item);
    delete item;

    return 1;
}

int TreePool::LoadInRange(int tree_index, const char *item_name, unsigned data_length, unsigned *data_values, int range)
{
    if ((0 > tree_index) || (tree_index >= _pool_size))
    {
        return -1;
    }
    if (!_pool[tree_index]._tree)
    {
        return -2;
    }
    if (_pool[tree_index]._result)
    {
        delete _pool[tree_index]._result;
    }

    TNamedArray *item = new TNamedArray(item_name, data_length, data_values);
    narResult *result = _pool[tree_index]._tree->RangeQuery(item, range);
    delete item;

    _pool[tree_index]._result = result;

    return result->GetNumOfEntries();
}

int TreePool::GetResult(int tree_index, int item_index, int name_buffer_len, char *item_name, int *distance)
{
    if ((0 > tree_index) || (tree_index >= _pool_size))
    {
        return -1;
    }
    if (!_pool[tree_index]._tree)
    {
        return -2;
    }
    if (!_pool[tree_index]._result)
    {
        return -3;
    }
    if ((0 > item_index) || (item_index >= _pool[tree_index]._result->GetNumOfEntries()))
    {
        return -4;
    }

    narResult *result = _pool[tree_index]._result;
    *distance = (*result)[item_index].GetDistance();

    const TNamedArray *item = (TNamedArray *) ((*result)[item_index].GetObject());
    int name_len = item->GetName().length();
    if (name_len >= name_buffer_len) {
        return -5;
    }
    memcpy(item_name, item->GetName().c_str(), name_len);
    item_name[name_len] = 0;

    return 1;
}

void TreePool::CleanResult(int tree_index)
{
    if ((0 <= tree_index) && (tree_index < _pool_size))
    {
        if (_pool[tree_index]._result)
        {
            delete _pool[tree_index]._result;
        }
    }
    return;
}

