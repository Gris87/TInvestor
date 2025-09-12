#include "src/grpc/grpcretryclient.h"

#include <QDebug>



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

std::shared_ptr<tinkoff::PortfolioResponse>
GrpcRetryClient::getValidPortfolio(QThread* /*parentThread*/, const QString& /*accountId*/)
{
    return nullptr;
}

std::shared_ptr<tinkoff::GetOperationsByCursorResponse> GrpcRetryClient::getValidOperations(
    QThread* /*parentThread*/, const QString& /*accountId*/, qint64 /*from*/, qint64 /*to*/, const QString& /*cursor*/
)
{
    return nullptr;
}
