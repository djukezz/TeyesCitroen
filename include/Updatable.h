#pragma once

#include "Arduino.h"

#include "IUpdatable.h"
#include "UpdatableCollection.h"

class Updatable : public IUpdatable
{
public:
    Updatable() : _collection(UpdatableCollection::GetInstance()) 
    {
        _collection->Add(this);
    }
    virtual ~Updatable()
    {
        _collection->Delete(this);
    }
    virtual void Update() {}

protected:
    size_t GetTime()
    {
        return static_cast<size_t>(millis());
    }

private:
    UpdatableCollection *const _collection;
};