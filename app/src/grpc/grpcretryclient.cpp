#include "src/grpc/grpcretryclient.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr double ROUND_2_DECIMALS_DOUBLE = 100.0;
constexpr int    ROUND_2_DECIMALS_NANO   = 10000000;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 SLEEP_BETWEEN_REQUEST   = 1LL * MS_IN_SECOND; // 1 second



GrpcRetryClient::GrpcRetryClient(IUserStorage* userStorage, IGrpcClient* grpcClient, ITimeUtils* timeUtils) :
    IGrpcRetryClient(),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient),
    mTimeUtils(timeUtils)
{
    qDebug() << "Create GrpcRetryClient";
}

GrpcRetryClient::~GrpcRetryClient()
{
    qDebug() << "Destroy GrpcRetryClient";
}

std::shared_ptr<tinkoff::GetOperationsByCursorResponse> GrpcRetryClient::getValidOperations(
    QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor
)
{
    mUserStorage->readLock();
    const float commission = mUserStorage->getCommission();
    mUserStorage->readUnlock();

    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> tinkoffOperations =
        mGrpcClient->getOperations(parentThread, accountId, from, to, cursor);

    if (!parentThread->isInterruptionRequested() && tinkoffOperations != nullptr)
    {
        for (int i = 0; i < tinkoffOperations->items_size(); ++i)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
            tinkoff::OperationItem&       tinkoffOperation = const_cast<tinkoff::OperationItem&>(tinkoffOperations->items(i));
            const tinkoff::OperationType  operationType    = tinkoffOperation.type();
            const tinkoff::InstrumentType instrumentKind   = tinkoffOperation.instrument_kind();

            if (instrumentKind == tinkoff::INSTRUMENT_TYPE_SHARE &&
                (operationType == tinkoff::OPERATION_TYPE_BUY || operationType == tinkoff::OPERATION_TYPE_SELL))
            {
                if (tinkoffOperation.commission().units() == 0 && tinkoffOperation.commission().nano() == 0)
                {
                    const double    payment             = quotationToDouble(tinkoffOperation.payment());
                    const Quotation commissionQuotation = quotationFromDouble(
                        qRound64(((-qAbs(payment) * commission) / HUNDRED_PERCENT) * ROUND_2_DECIMALS_DOUBLE) /
                        ROUND_2_DECIMALS_DOUBLE
                    );

                    tinkoff::MoneyValue* commissionMoney = new tinkoff::MoneyValue(); // tinkoffOperation will take ownership

                    commissionMoney->set_units(commissionQuotation.units);
                    commissionMoney->set_nano(
                        qRound64(static_cast<double>(commissionQuotation.nano) / ROUND_2_DECIMALS_NANO) * ROUND_2_DECIMALS_NANO
                    );

                    tinkoffOperation.set_allocated_commission(commissionMoney);
                }
            }
        }
    }

    return tinkoffOperations;
}

std::shared_ptr<tinkoff::PortfolioResponse> GrpcRetryClient::getValidPortfolio(QThread* parentThread, const QString& accountId)
{
    std::shared_ptr<tinkoff::PortfolioResponse> res = nullptr;

    while (!parentThread->isInterruptionRequested() && res == nullptr)
    {
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio = mGrpcClient->getPortfolio(parentThread, accountId);

        if (!parentThread->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            if (validatePortfolioResponse(*tinkoffPortfolio))
            {
                res = tinkoffPortfolio;
            }
            else
            {
                qDebug() << "Invalid portfolio received. Try one more time";

                if (mTimeUtils->interruptibleSleep(SLEEP_BETWEEN_REQUEST, parentThread))
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    return res;
}

bool GrpcRetryClient::validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    bool res = true;

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId = QString::fromStdString(position.instrument_uid());

        if (instrumentId != RUBLE_UID)
        {
            if ((position.average_position_price_fifo().units() <= 0 && position.average_position_price_fifo().nano() <= 0) ||
                (position.average_position_price().units() <= 0 && position.average_position_price().nano() <= 0))
            {
                res = false;

                break;
            }
        }
    }

    return res;
}
