#include "src/threads/detectdividends/detectdividendsthread.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrlQuery>



const char* const DATE_FORMAT = "yyyy-MM-dd";

constexpr qint64 MS_IN_SECOND        = 1000LL;
constexpr qint64 ONE_MINUTE          = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR            = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY             = 24LL * ONE_HOUR;
constexpr qint64 ONE_MONTH           = 31LL * ONE_DAY;
constexpr qint64 DIVIDENDS_INTERVAL  = 6LL * ONE_MONTH;    // 6 months
constexpr qint64 SLEEP_DELAY         = 5LL * MS_IN_SECOND; // 5 seconds
constexpr int    HTTP_STATUS_CODE_OK = 200;



DetectDividendsThread::DetectDividendsThread(
    IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IHttpClient* httpClient, QObject* parent
) :
    IDetectDividendsThread(parent),
    mStocksStorage(stocksStorage),
    mTimeUtils(timeUtils),
    mHttpClient(httpClient)
{
    qDebug() << "Create DetectDividendsThread";
}

DetectDividendsThread::~DetectDividendsThread()
{
    qDebug() << "Destroy DetectDividendsThread";
}

void DetectDividendsThread::run()
{
    qDebug() << "Running DetectDividendsThread";

    blockSignals(false);

    const qint64 startTimestamp = QDateTime::currentMSecsSinceEpoch();
    const qint64 endTimestamp   = startTimestamp + DIVIDENDS_INTERVAL;

    QUrl url = QUrl("https://api-invest.tbank.ru/invest-terminal/api-invest-gw/fireg-advisory/invest-calendar/v2/event/info");

    QUrlQuery query;

    // clang-format off
    query.addQueryItem("startDate",        QDateTime::fromMSecsSinceEpoch(startTimestamp).toString(DATE_FORMAT));
    query.addQueryItem("endDate",          QDateTime::fromMSecsSinceEpoch(endTimestamp).toString(DATE_FORMAT));
    query.addQueryItem("mySecuritiesFlag", "false");
    query.addQueryItem("qualFlag",         "false");
    query.addQueryItem("eventTypes",       "dividends");
    query.addQueryItem("limit",            "100");
    // clang-format on

    url.setQuery(query.query());

    const IHttpClient::Headers headers;

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const HttpResult httpResult = mHttpClient->get(url, headers);

        if (httpResult.statusCode != HTTP_STATUS_CODE_OK)
        {
            if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
            {
                break;
            }

            continue;
        }

        processDividendsResponse(httpResult.body);

        break;
    }

    qDebug() << "Finish DetectDividendsThread";
}

void DetectDividendsThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void DetectDividendsThread::processDividendsResponse(const QByteArray& resp)
{
    QMap<QString, QJsonObject> dividendsMap = convertDividendsResponseToMap(resp);

    qInfo() << dividendsMap.keys();
}

QMap<QString, QJsonObject> DetectDividendsThread::convertDividendsResponseToMap(const QByteArray& resp)
{
    QMap<QString, QJsonObject> res;

    QJsonParseError error;
    QJsonDocument   doc = QJsonDocument::fromJson(resp, &error);

    if (error.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON parse error:" << error.errorString();

        return res;
    }

    QJsonObject rootObject = doc.object();
    QJsonArray  datesArray = rootObject.value("dates").toArray();

    for (const QJsonValue& dateValue : datesArray)
    {
        QJsonObject dateObject = dateValue.toObject();

        const QString date           = dateObject.value("date").toString();
        QJsonArray    dividendsArray = dateObject.value("dividends").toArray();

        for (const QJsonValue& dividendValue : dividendsArray)
        {
            QJsonObject dividendObject = dividendValue.toObject();

            const QString instrumentId = dividendObject.value("instrumentUid").toString();
            dividendObject.insert("date", date);

            res[instrumentId] = dividendObject;
        }
    }

    return res;
}
