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

    void readLock()
    {
        mRwMutex.lockForRead();
    }

    void readUnlock()
    {
        mRwMutex.unlock();
    }

    void writeLock()
    {
        mRwMutex.lockForWrite();
    }

    void writeUnlock()
    {
        mRwMutex.unlock();
    }

    QPixmap pixmap;

private:
    QReadWriteLock mRwMutex;
};

using Logos = QMap<QString, Logo*>; // Instrument UID => Logo
