#pragma once

#include "set"
#include "IUpdatable.h"

class UpdatableCollection
{
public:
    void Add(IUpdatable *const u)
    {
        _items.insert(u);
    }
    void Delete(IUpdatable *const u)
    {
        _items.erase(u);
    }

    static UpdatableCollection *const GetInstance()
    {
        return _instance;
    }

    static void UpdateAll()
    {
        _instance->UpdateAllImpl();
    }

private:
    UpdatableCollection(){}
    
    void UpdateAllImpl()
    {
        for(auto& item : _items)
            item->Update();
    }

    static UpdatableCollection *const _instance;
    std::set<IUpdatable *> _items{};
};

inline UpdatableCollection* const UpdatableCollection::_instance = new UpdatableCollection();