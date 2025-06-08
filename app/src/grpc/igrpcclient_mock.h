#pragma once



#include "src/grpc/igrpcclient.h"

#include <gmock/gmock.h>



class GrpcClientMock : public IGrpcClient
{
    Q_OBJECT

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
    MOCK_METHOD(
        std::shared_ptr<tinkoff::SharesResponse>,
        findStocks,
        (QThread * parentThread, tinkoff::InstrumentStatus instrumentStatus),
        (override)
    );
    MOCK_METHOD(std::shared_ptr<tinkoff::BondsResponse>, findBonds, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::CurrenciesResponse>, findCurrencies, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::EtfsResponse>, findEtfs, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::FuturesResponse>, findFutures, (QThread * parentThread), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetCandlesResponse>,
        getCandles,
        (QThread * parentThread, const QString& instrumentId, qint64 from, qint64 to),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetOrderBookResponse>,
        getOrderBook,
        (QThread * parentThread, const QString& instrumentId),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PortfolioResponse>, getPortfolio, (QThread * parentThread, const QString& accountId), (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PositionsResponse>, getPositions, (QThread * parentThread, const QString& accountId), (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetOperationsByCursorResponse>,
        getOperations,
        (QThread * parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor),
        (override)
    );

    MOCK_METHOD(std::shared_ptr<MarketDataStream>, createMarketDataStream, (), (override));
    MOCK_METHOD(
        bool,
        subscribeLastPrices,
        (std::shared_ptr<MarketDataStream> & marketDataStream, const QStringList& instrumentIds),
        (override)
    );
    MOCK_METHOD(bool, unsubscribeLastPrices, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(
        bool, subscribeOrderBook, (std::shared_ptr<MarketDataStream> & marketDataStream, const QString& instrumentId), (override)
    );
    MOCK_METHOD(bool, unsubscribeOrderBook, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::MarketDataResponse>,
        readMarketDataStream,
        (std::shared_ptr<MarketDataStream> & marketDataStream),
        (override)
    );
    MOCK_METHOD(bool, closeWriteMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(void, cancelMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(void, finishMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));

    MOCK_METHOD(std::shared_ptr<PortfolioStream>, createPortfolioStream, (const QString& accountId), (override));
    MOCK_METHOD(
        std::shared_ptr<PortfolioStream>,
        createPortfolioStream,
        (const QString& accountId, const QString& anotherAccountId),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PortfolioStreamResponse>,
        readPortfolioStream,
        (std::shared_ptr<PortfolioStream> & portfolioStream),
        (override)
    );
    MOCK_METHOD(void, cancelPortfolioStream, (std::shared_ptr<PortfolioStream> & portfolioStream), (override));
    MOCK_METHOD(void, finishPortfolioStream, (std::shared_ptr<PortfolioStream> & portfolioStream), (override));

    MOCK_METHOD(std::shared_ptr<PositionsStream>, createPositionsStream, (const QString& accountId), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PositionsStreamResponse>,
        readPositionsStream,
        (std::shared_ptr<PositionsStream> & positionsStream),
        (override)
    );
    MOCK_METHOD(void, cancelPositionsStream, (std::shared_ptr<PositionsStream> & positionsStream), (override));
    MOCK_METHOD(void, finishPositionsStream, (std::shared_ptr<PositionsStream> & positionsStream), (override));
};
