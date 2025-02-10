#pragma once



template<typename T>
class ObjectHolder
{
public:
    ObjectHolder(T* obj) :
        mObject(obj)
    {
    }

    ~ObjectHolder()
    {
        delete mObject;
    }

    ObjectHolder(const ObjectHolder& another)            = delete;
    ObjectHolder& operator=(const ObjectHolder& another) = delete;

private:
    T* mObject;
};
