#include "src/grpc/grpcretryclient.h"

#include <QDebug>



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr qint64 MS_IN_SECOND          = 1000LL;
constexpr qint64 SLEEP_BETWEEN_REQUEST = 1LL * MS_IN_SECOND; // 1 second



GrpcRetryClient::GrpcRetryClient(IGrpcClient* grpcClient, ITimeUtils* timeUtils, QObject* parent) :
    IGrpcRetryClient(parent),
    mGrpcClient(grpcClient),
    mTimeUtils(timeUtils)
{
    qDebug() << "Create GrpcRetryClient";
}

GrpcRetryClient::~GrpcRetryClient()
{
    qDebug() << "Destroy GrpcRetryClient";
}

std::shared_ptr<tinkoff::PortfolioResponse> GrpcRetryClient::getValidPortfolio(QThread* parentThread, const QString& accountId)
{
    std::shared_ptr<tinkoff::PortfolioResponse> res = nullptr;

    while (!parentThread->isInterruptionRequested() && res == nullptr)
    {
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio = mGrpcClient->getPortfolio(parentThread, accountId);

        if (!parentThread->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            bool valid = true;

            for (int i = 0; i < tinkoffPortfolio->positions_size(); ++i)
            {
                const tinkoff::PortfolioPosition& position = tinkoffPortfolio->positions(i);

                const QString instrumentId = QString::fromStdString(position.instrument_uid());

                if (instrumentId != RUBLE_UID)
                {
                    if ((position.average_position_price_fifo().units() <= 0 && position.average_position_price_fifo().nano() <= 0
                        ) ||
                        (position.average_position_price().units() <= 0 && position.average_position_price().nano() <= 0))
                    {
                        valid = false;

                        break;
                    }
                }
            }

            if (valid)
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

std::shared_ptr<tinkoff::GetOperationsByCursorResponse> GrpcRetryClient::getValidOperations(
    QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor
)
{
    std::shared_ptr<tinkoff::GetOperationsByCursorResponse> res = nullptr;

    while (!parentThread->isInterruptionRequested() && res == nullptr)
    {
        const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> tinkoffOperations =
            mGrpcClient->getOperations(parentThread, accountId, from, to, cursor);

        if (!parentThread->isInterruptionRequested() && tinkoffOperations != nullptr)
        {
            bool valid = true;

            for (int i = 0; i < tinkoffOperations->items_size(); ++i)
            {
                const tinkoff::OperationItem& tinkoffOperation = tinkoffOperations->items(i);
                const tinkoff::OperationType  operationType    = tinkoffOperation.type();
                const tinkoff::InstrumentType instrumentKind   = tinkoffOperation.instrument_kind();

                if (instrumentKind == tinkoff::INSTRUMENT_TYPE_SHARE &&
                    (operationType == tinkoff::OPERATION_TYPE_BUY || operationType == tinkoff::OPERATION_TYPE_SELL))
                {
                    if (tinkoffOperation.commission().units() == 0 && tinkoffOperation.commission().nano() == 0)
                    {
                        valid = false;

                        break;
                    }
                }
            }

            if (valid)
            {
                res = tinkoffOperations;
            }
            else
            {
                qDebug() << "Invalid operations received. Try one more time";

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
