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
        (QThread * parentThread, const QString& instrumentId, int depth),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PortfolioResponse>, getPortfolio, (QThread * parentThread, const QString& accountId), (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetOperationsByCursorResponse>,
        getOperations,
        (QThread * parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetMaxLotsResponse>,
        getMaxLots,
        (QThread * parentThread, const QString& accountId, const QString& instrumentId, const Quotation& price),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::PostOrderResponse>,
        postOrder,
        (QThread * parentThread,
         const QString&          accountId,
         const QString&          instrumentId,
         tinkoff::OrderDirection direction,
         qint64                  quantity,
         const Quotation&        price,
         bool                    confirmMarginTrade,
         GrpcPriority            priority),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::OrderState>,
        getOrderState,
        (QThread * parentThread, const QString& accountId, const QString& orderId),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::CancelOrderResponse>,
        cancelOrder,
        (QThread * parentThread, const QString& accountId, const QString& orderId),
        (override)
    );

    MOCK_METHOD(
        std::shared_ptr<MarketDataStream>, createMarketDataStreamForLastPrice, (const QStringList& instrumentIds), (override)
    );
    MOCK_METHOD(
        std::shared_ptr<MarketDataStream>,
        createMarketDataStreamForOrderBook,
        (const QString& instrumentId, int depth),
        (override)
    );
    MOCK_METHOD(
        std::shared_ptr<tinkoff::MarketDataResponse>,
        readMarketDataStream,
        (std::shared_ptr<MarketDataStream> & marketDataStream),
        (override)
    );
    MOCK_METHOD(void, cancelMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(void, finishMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));

    MOCK_METHOD(std::shared_ptr<OperationsStream>, createOperationsStream, (const QString& accountId), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::OperationsStreamResponse>,
        readOperationsStream,
        (std::shared_ptr<OperationsStream> & operationsStream),
        (override)
    );
    MOCK_METHOD(void, cancelOperationsStream, (std::shared_ptr<OperationsStream> & operationsStream), (override));
    MOCK_METHOD(void, finishOperationsStream, (std::shared_ptr<OperationsStream> & operationsStream), (override));

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
};
