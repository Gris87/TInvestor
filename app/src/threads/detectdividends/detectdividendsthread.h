#pragma once



#include "src/threads/detectdividends/idetectdividendsthread.h"

#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/timeutils/itimeutils.h"



class DetectDividendsThread : public IDetectDividendsThread
{
    Q_OBJECT

public:
    explicit DetectDividendsThread(
        IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IHttpClient* httpClient, QObject* parent = nullptr
    );
    ~DetectDividendsThread() override;

    DetectDividendsThread(const DetectDividendsThread& another)            = delete;
    DetectDividendsThread& operator=(const DetectDividendsThread& another) = delete;

    void run() override;

    void terminateThread() override;

    void processDividendsResponse(const QByteArray& resp);

private:
    QMap<QString, QJsonObject> convertDividendsResponseToMap(const QByteArray& resp);
    void                       updateDividendsMeta(QList<Stock*> stocks, const QMap<QString, QJsonObject>& dividendsMap);

    IStocksStorage* mStocksStorage;
    ITimeUtils*     mTimeUtils;
    IHttpClient*    mHttpClient;
};
