#pragma once



#include "src/grpc/igrpcclient.h"

#include <gmock/gmock.h>



class GrpcClientMock : public IGrpcClient
{
public:
    explicit GrpcClientMock(QObject* parent = nullptr) :
        IGrpcClient(parent)
    {
    }
    ~GrpcClientMock() override = default;

    GrpcClientMock(const GrpcClientMock& another)            = delete;
    GrpcClientMock& operator=(const GrpcClientMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<tinkoff::GetInfoResponse>, getUserInfo, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::GetAccountsResponse>, getAccounts, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::SharesResponse>, findStocks, (QThread * parentThread), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetCandlesResponse>,
        getCandles,
        (QThread * parentThread, const QString& uid, qint64 from, qint64 to),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetOrderBookResponse>, getOrderBook, (QThread * parentThread, const QString& uid), (override)
    );
    MOCK_METHOD(std::shared_ptr<MarketDataStream>, createMarketDataStream, (), (override));
    MOCK_METHOD(
        bool, subscribeLastPrices, (std::shared_ptr<MarketDataStream> & marketDataStream, const QStringList& uids), (override)
    );
    MOCK_METHOD(bool, unsubscribeLastPrices, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(bool, subscribeOrderBook, (std::shared_ptr<MarketDataStream> & marketDataStream, const QString& uid), (override));
    MOCK_METHOD(bool, unsubscribeOrderBook, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::MarketDataResponse>,
        readMarketDataStream,
        (std::shared_ptr<MarketDataStream> & marketDataStream),
        (override)
    );
    MOCK_METHOD(bool, closeWriteMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(void, finishMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
};
