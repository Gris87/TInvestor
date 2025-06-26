#pragma once



#include <QPixmap>
#include <QReadWriteLock>



class Logo
{
public:
    Logo()  = default;
    ~Logo() = default;

    Logo(const Logo& another)            = delete;
    Logo& operator=(const Logo& another) = delete;

    inline void readLock()
    {
        mRwMutex.lockForRead();
    }

    inline void readUnlock()
    {
        mRwMutex.unlock();
    }

    inline void writeLock()
    {
        mRwMutex.lockForWrite();
    }

    inline void writeUnlock()
    {
        mRwMutex.unlock();
    }

    QPixmap pixmap;

private:
    QReadWriteLock mRwMutex;
};

using Logos = QMap<QString, Logo*>; // Instrument UID => Logo
