#include "src/threads/stockcollect/stockcollectthread.h"

#include <QAtomicInt>
#include <QCoreApplication>
#include <QDebug>
#include <QUrlQuery>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

const char* const NEW_VLHZ_UID  = "784f9e60-4214-4730-ba21-ddb2dd677a8f";
const char* const NEW_KBSB_UID  = "104c0979-1f16-4e13-9787-dc767814c4a6";
const char* const NEW_RZSB_UID  = "a2284e69-0e1e-4fdd-8cc9-5d991d936b90";
const char* const NEW_KCHEP_UID = "ef832fa3-1d62-4a18-b3ca-776d76a29cb1";
const char* const NEW_TASB_UID  = "54e856d4-ba8a-4365-8fd1-245a8888b306";
const char* const NEW_BISVP_UID = "56452fa0-1e58-457e-bb41-90acaabf0767";
const char* const NEW_MFGS_UID  = "df35ac3e-3d77-454f-b446-cdbb9d867b02";
const char* const NEW_RUSI_UID  = "01fd2cc9-489c-4756-b267-72c3ccc61b8f";
const char* const NEW_DZRDP_UID = "c4304c3c-4106-47ea-aabc-e1ae39930791";
const char* const NEW_VGSB_UID  = "2bd81af0-b389-4638-ac9b-35a9938be0cc";
const char* const NEW_DIOD_UID  = "464c9ca5-2ba5-4b66-90e2-005f30bff134";
const char* const NEW_HIMCP_UID = "c53f1d8f-78e0-46e5-ac31-fdc08f05c8e2";
const char* const NEW_KMEZ_UID  = "e3e10315-c9e8-452a-9113-2f14805679ce";
const char* const NEW_ZILL_UID  = "a257d925-305e-43e2-a9d5-1174c1190dee";
const char* const NEW_KRSB_UID  = "d8de3aca-92e5-4135-8f1d-c3d545cbc041";
const char* const NEW_JNOSP_UID = "e79f65d6-1bd0-42c6-943e-5060bbeabc45";
const char* const NEW_TNSE_UID  = "703e944b-d4e6-4697-bea5-844c2c8be554";
const char* const NEW_ZVEZ_UID  = "c35cfe95-ad55-44ab-84d4-2e9754bb55b4";
const char* const NEW_NAUK_UID  = "700671c4-624e-43d0-8377-95de3956b111";
const char* const NEW_MAGE_UID  = "1214a841-5a72-42ac-b911-5c05a7eaa48e";
const char* const NEW_GEMA_UID  = "0f446a59-c78e-44fc-8235-7b097096769a";
const char* const NEW_SVETP_UID = "edc6b309-746d-4697-9752-b7697b0ba38a";
const char* const NEW_ASSB_UID  = "578506fc-187d-488c-a10e-2651530da4b9";
const char* const NEW_LVHK_UID  = "76558b5d-9875-4c92-82ca-44f2d0b39e1b";
const char* const NEW_TASBP_UID = "d61f5ab1-ec03-4dc8-9f10-2f746b0a20c1";
const char* const NEW_VJGZ_UID  = "32822b7c-b13c-45df-b1af-cb6e3d676399";
const char* const NEW_BRZL_UID  = "5a6c9d26-10db-4a21-9c0e-2b7b063c69f8";
const char* const NEW_RDRB_UID  = "8d83880d-722a-45cd-b4e3-1a7c95112ed5";
const char* const NEW_MRSB_UID  = "1037b19b-46f4-4ebf-bb87-53401f8d2148";
const char* const NEW_NFAZ_UID  = "7e13d581-bbbf-41bb-89c9-e2c4a85a9bde";
const char* const NEW_SLEN_UID  = "813fac1c-eb17-4ccb-9ec3-72a80814fc85";
const char* const NEW_VRSBP_UID = "806938cd-172e-477f-b00c-c8e731cfafa7";
const char* const NEW_URKZ_UID  = "9ec09704-0912-4ab8-9ebf-3fdad6fd3944";
const char* const NEW_STSBP_UID = "6bffd7f3-4c01-495b-9a4d-e1f8c9e1774f";
const char* const NEW_MRKK_UID  = "49b8a647-e740-4c29-b390-4d1d6829d13a";
const char* const NEW_KRKOP_UID = "c961e321-503a-442c-8c4a-5dce95d75513";
const char* const NEW_KUZB_UID  = "879b2e35-edfb-42f4-baa0-8a00cf1edaac";
const char* const NEW_OMZZP_UID = "f26aade9-d33a-4cdf-867e-1c18c753e63a";
const char* const NEW_RGSS_UID  = "e3312603-d109-443a-86b5-329eede6c853";
const char* const NEW_YRSBP_UID = "97015e2f-adae-4133-9ce0-cd79015d13a2";
const char* const NEW_EELT_UID  = "1c181853-2fd6-4a20-a1c6-cb923a7c7919";
const char* const NEW_SVET_UID  = "6fdba807-39e5-41d8-a272-ca2e7e35223d";
const char* const NEW_VSYDP_UID = "c6fd4e8e-5bb4-4f8a-9727-5d8dddff2e7d";
const char* const NEW_STSB_UID  = "e268b9d4-7c8c-44b4-b2b2-11c0d55155e8";
const char* const NEW_NKSH_UID  = "2ff42bea-3ea9-4af9-a06e-19fe0874b7a1";
const char* const NEW_CHKZ_UID  = "5f858c64-ba9a-4d66-86bf-73b5880cf30f";
const char* const NEW_KFBA_UID  = "0bdca049-f06d-4a27-b46c-f89236a44727";
const char* const NEW_APRI_UID  = "1c5fd815-eb3e-41b1-a5b7-9cc7c905bd78";
const char* const NEW_RTSBP_UID = "2113c648-6459-49c5-bdfc-3c608d2afaa0";
const char* const NEW_RTSB_UID  = "deab934f-7e5e-4467-a465-3624c1110e7b";
const char* const NEW_UTAR_UID  = "2ca51950-2047-4090-b6ba-6ecd486291a2";
const char* const NEW_AVAN_UID  = "e027aaaa-cb20-4d48-be23-6d2d4b0edfb8";
const char* const NEW_TUZA_UID  = "4744e309-4b10-48c7-8dc8-c6ad1ccf5c3b";
const char* const NEW_ROST_UID  = "e53026a3-5585-478a-8ae7-59698864f681";
const char* const NEW_NNSB_UID  = "9b5366b0-2a2e-4074-ace6-d844fefee42b";
const char* const NEW_SARE_UID  = "b8c992db-6cb8-449e-8602-bb4252a28ea4";
const char* const NEW_YRSB_UID  = "7e669c8c-ed3e-4f86-a255-8af223f9b824";
const char* const NEW_KROTP_UID = "205b66cd-fa09-4fbd-85c3-46eefc9eff40";
const char* const NEW_WTCM_UID  = "f06a87cb-6a2f-42be-85ea-6698d3a62037";
const char* const NEW_KCHE_UID  = "caa615ad-99c1-499c-b592-56368f2ebd66";
const char* const NEW_UKUZ_UID  = "cf07cbaa-5ef6-41f1-abc3-8d12d4bc6edf";
const char* const NEW_YKENP_UID = "d69ebc99-fafb-4f82-af9c-b52aa49da613";
const char* const NEW_SAREP_UID = "9bb7b964-285f-43c6-ba61-1dc829f25e30";
const char* const NEW_KOGK_UID  = "b4d1ec34-82ef-4a47-a639-67a8ab8b683e";
const char* const NEW_ARSA_UID  = "786c1d68-0864-4804-b3ca-2166766cd917";
const char* const NEW_VJGZP_UID = "1b3363dd-8c0e-4d67-abf2-0666ce5a23f5";
const char* const NEW_DZRD_UID  = "47dc8658-0d02-4429-b75e-02bdf465c22f";
const char* const NEW_KRKN_UID  = "3d83aedf-b48f-4791-9370-c0569170a82d";
const char* const NEW_MGTS_UID  = "d614f6be-89a7-4fda-86f4-580f8d7463ae";
const char* const NEW_LMBZ_UID  = "ee64af27-d3f9-4a9a-a50a-7143c6087371";
const char* const NEW_KRSBP_UID = "241dc583-9e35-4b58-8bee-632691c8b4b8";
const char* const NEW_SAGO_UID  = "fb46349d-a7ef-413b-917f-edec1a1529cc";
const char* const NEW_MISB_UID  = "bd685d6d-715f-499b-a50d-3b47c04a65a6";
const char* const NEW_GAZAP_UID = "09c5f10f-b671-4acc-bc17-db491a1a66eb";
const char* const NEW_TORS_UID  = "414928c5-2ae2-4dff-9420-5f1c85146a99";
const char* const NEW_IGSTP_UID = "0c275361-5f48-4138-a592-6da7d275075d";
const char* const NEW_IGST_UID  = "641576a5-9abd-47fa-99c5-7380c8890355";
const char* const NEW_SAGOP_UID = "335cb2e2-6922-42a3-803b-60024cf79ce1";
const char* const NEW_VSYD_UID  = "4c5774d2-a1c6-4069-860b-3a93db0b4931";
const char* const NEW_MISBP_UID = "97b4c319-1a9e-4550-a9f1-c39b489713ba";
const char* const NEW_WTCMP_UID = "d6461eb6-6557-4993-8d26-6202a1820667";
const char* const NEW_MFGSP_UID = "1c0d371b-1b75-4185-8877-45da35cbd692";
const char* const NEW_PAZA_UID  = "5a4b9636-2445-43db-a298-9dada56ba3e8";
const char* const NEW_YKEN_UID  = "1ed313a9-c6d6-4359-9cf2-e2d7011de00e";
const char* const NEW_USBN_UID  = "71e9e3f7-60b6-4ec5-ae49-d4eba51d648e";
const char* const NEW_PRMB_UID  = "c0545aeb-b6b1-4ccb-b37e-3eaa427b4717";
const char* const NEW_RTGZ_UID  = "210ef34f-e815-47a2-85c9-d2a033fc642f";
const char* const NEW_VGSBP_UID = "7b1e51e2-9de2-47d6-bf3d-ab502ee54df8";
const char* const NEW_BSPBP_UID = "e71d6238-71ea-400c-9451-edc7b3338a2a";
const char* const NEW_JNOS_UID  = "5fdf1499-288c-4b0e-8d4a-10c5f5fefb57";
const char* const NEW_CHGZ_UID  = "5d06ba3a-408a-410e-aea1-41d1adf81db3";
const char* const NEW_MAGEP_UID = "b4959236-ef80-4eec-a5e0-44cc423b6ab0";
const char* const NEW_NNSBP_UID = "9e16009b-ec7a-499a-b0ce-9d75f9b69117";
const char* const NEW_TORSP_UID = "2d98aa0a-dfdb-42e9-8762-9db3eab40152";
const char* const NEW_LPSB_UID  = "b8d26943-a3c1-465a-8c58-518466e2ab26";
const char* const NEW_GAZA_UID  = "fe427a08-8878-493d-b4cf-8ba65dfb8454";

const char* const GTRK_UID = "9e69afb6-4561-4fc2-b63b-b181e3f9ecdc";
const char* const JETL_UID = "34f51a49-919b-4eb1-9ed0-59c26f4cdc1e";

constexpr qint64 MS_IN_SECOND                       = 1000LL;
constexpr qint64 ONE_MINUTE                         = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                           = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                            = 24LL * ONE_HOUR;
constexpr qint64 ONE_WEEK                           = 7LL * ONE_DAY;
constexpr qint64 ONE_MONTH                          = 31LL * ONE_DAY;
constexpr qint64 SLEEP_DELAY                        = 5LL * MS_IN_SECOND; // 5 seconds
constexpr qint64 MOSCOW_TIME                        = 3 * ONE_HOUR;       // 3 hours
constexpr qint64 MAX_GRPC_TIME_LIMIT                = ONE_MONTH;          // 1 month
constexpr int    LOGO_SIZE                          = 24;
constexpr int    HTTP_STATUS_CODE_OK                = 200;
constexpr int    HTTP_STATUS_CODE_TOO_MANY_REQUESTS = 429;

// clang-format off
static const QSet<QString> LOCKED_STOCKS{ // clazy:exclude=non-pod-global-static
    NEW_VLHZ_UID,
    NEW_KBSB_UID,
    NEW_RZSB_UID,
    NEW_KCHEP_UID,
    NEW_TASB_UID,
    NEW_BISVP_UID,
    NEW_MFGS_UID,
    NEW_RUSI_UID,
    NEW_DZRDP_UID,
    NEW_VGSB_UID,
    NEW_DIOD_UID,
    NEW_HIMCP_UID,
    NEW_KMEZ_UID,
    NEW_ZILL_UID,
    NEW_KRSB_UID,
    NEW_JNOSP_UID,
    NEW_TNSE_UID,
    NEW_ZVEZ_UID,
    NEW_NAUK_UID,
    NEW_MAGE_UID,
    NEW_GEMA_UID,
    NEW_SVETP_UID,
    NEW_ASSB_UID,
    NEW_LVHK_UID,
    NEW_TASBP_UID,
    NEW_VJGZ_UID,
    NEW_BRZL_UID,
    NEW_RDRB_UID,
    NEW_MRSB_UID,
    NEW_NFAZ_UID,
    NEW_SLEN_UID,
    NEW_VRSBP_UID,
    NEW_URKZ_UID,
    NEW_STSBP_UID,
    NEW_MRKK_UID,
    NEW_KRKOP_UID,
    NEW_KUZB_UID,
    NEW_OMZZP_UID,
    NEW_RGSS_UID,
    NEW_YRSBP_UID,
    NEW_EELT_UID,
    NEW_SVET_UID,
    NEW_VSYDP_UID,
    NEW_STSB_UID,
    NEW_NKSH_UID,
    NEW_CHKZ_UID,
    NEW_KFBA_UID,
    NEW_APRI_UID,
    NEW_RTSBP_UID,
    NEW_RTSB_UID,
    NEW_UTAR_UID,
    NEW_AVAN_UID,
    NEW_TUZA_UID,
    NEW_ROST_UID,
    NEW_NNSB_UID,
    NEW_SARE_UID,
    NEW_YRSB_UID,
    NEW_KROTP_UID,
    NEW_WTCM_UID,
    NEW_KCHE_UID,
    NEW_UKUZ_UID,
    NEW_YKENP_UID,
    NEW_SAREP_UID,
    NEW_KOGK_UID,
    NEW_ARSA_UID,
    NEW_VJGZP_UID,
    NEW_DZRD_UID,
    NEW_KRKN_UID,
    NEW_MGTS_UID,
    NEW_LMBZ_UID,
    NEW_KRSBP_UID,
    NEW_SAGO_UID,
    NEW_MISB_UID,
    NEW_GAZAP_UID,
    NEW_TORS_UID,
    NEW_IGSTP_UID,
    NEW_IGST_UID,
    NEW_SAGOP_UID,
    NEW_VSYD_UID,
    NEW_MISBP_UID,
    NEW_WTCMP_UID,
    NEW_MFGSP_UID,
    NEW_PAZA_UID,
    NEW_YKEN_UID,
    NEW_USBN_UID,
    NEW_PRMB_UID,
    NEW_RTGZ_UID,
    NEW_VGSBP_UID,
    NEW_BSPBP_UID,
    NEW_JNOS_UID,
    NEW_CHGZ_UID,
    NEW_MAGEP_UID,
    NEW_NNSBP_UID,
    NEW_TORSP_UID,
    NEW_LPSB_UID,
    NEW_GAZA_UID
};
// clang-format on

// clang-format off
static const QSet<QString> IGNORE_STOCKS{ // clazy:exclude=non-pod-global-static
    GTRK_UID,
    JETL_UID
};
// clang-format on



enum CsvField : qint8
{
    CSV_FIELD_FIGI,
    CSV_FIELD_TIMESTAMP,
    CSV_FIELD_OPEN_PRICE,
    CSV_FIELD_CLOSE_PRICE,
    CSV_FIELD_HIGH_PRICE,
    CSV_FIELD_LOW_PRICE,
    CSV_FIELD_VOLUME,
};



StockCollectThread::StockCollectThread(
    IConfig*             config,
    IUserStorage*        userStorage,
    IStocksStorage*      stocksStorage,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    IBiDirInfosStorage*  biDirInfosStorage,
    IDirFactory*         dirFactory,
    IFileFactory*        fileFactory,
    IQZipFactory*        qZipFactory,
    IQZipFileFactory*    qZipFileFactory,
    ITimeUtils*          timeUtils,
    IHttpClient*         httpClient,
    IGrpcClient*         grpcClient,
    QObject*             parent
) :
    IStockCollectThread(parent),
    mConfig(config),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mBiDirInfosStorage(biDirInfosStorage),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
    mQZipFactory(qZipFactory),
    mQZipFileFactory(qZipFileFactory),
    mTimeUtils(timeUtils),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mDayStartTimestamp()
{
    qDebug() << "Create StockCollectThread";
}

StockCollectThread::~StockCollectThread()
{
    qDebug() << "Destroy StockCollectThread";
}

void StockCollectThread::run()
{
    qDebug() << "Running StockCollectThread";

    blockSignals(false);
    emit notifyInstrumentsProgress(tr("Downloading metadata"));

    const std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks =
        mGrpcClient->findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_BASE);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffStocks != nullptr)
    {
        storeNewInstrumentsInfo();
        reloadBiDirInfos();
        const bool needStocksUpdate = storeNewStocksInfo(tinkoffStocks);
        obtainStocksData();
        copyDataToOperational();
        cleanupOperationalData();
        const bool needPricesUpdate = obtainStocksDayStartPrice();
        obtainLastTradeTime();
        obtainTurnover();
        obtainPayback();
        writeStocksMeta();

        notifyAboutChanges(needStocksUpdate, needPricesUpdate);
    }

    qDebug() << "Finish StockCollectThread";
}

void StockCollectThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

static void
obtainInstrumentsFromShares(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::SharesResponse> tinkoffShares =
        grpcClient->findStocks(parentThread, tinkoff::INSTRUMENT_STATUS_ALL);

    if (!parentThread->isInterruptionRequested() && tinkoffShares != nullptr)
    {
        for (int i = 0; i < tinkoffShares->instruments_size(); ++i)
        {
            const tinkoff::Share& tinkoffShare = tinkoffShares->instruments(i);

            if (tinkoffShare.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffShare.uid());

                Instrument instrument;
                instrument.ticker            = QString::fromStdString(tinkoffShare.ticker());
                instrument.name              = QString::fromStdString(tinkoffShare.name());
                instrument.lot               = tinkoffShare.lot();
                instrument.pricePrecision    = quotationPrecision(tinkoffShare.min_price_increment());
                instrument.minPriceIncrement = quotationConvert(tinkoffShare.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffShare.brand().logo_name())));
            }
        }
    }
}

static void
obtainInstrumentsFromBonds(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::BondsResponse> tinkoffBonds = grpcClient->findBonds(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffBonds != nullptr)
    {
        for (int i = 0; i < tinkoffBonds->instruments_size(); ++i)
        {
            const tinkoff::Bond& tinkoffBond = tinkoffBonds->instruments(i);

            if (tinkoffBond.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffBond.uid());

                Instrument instrument;
                instrument.ticker            = QString::fromStdString(tinkoffBond.ticker());
                instrument.name              = QString::fromStdString(tinkoffBond.name());
                instrument.lot               = tinkoffBond.lot();
                instrument.pricePrecision    = quotationPrecision(tinkoffBond.min_price_increment());
                instrument.minPriceIncrement = quotationConvert(tinkoffBond.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffBond.brand().logo_name())));
            }
        }
    }
}

static void obtainInstrumentsFromCurrencies(
    QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos
)
{
    const std::shared_ptr<tinkoff::CurrenciesResponse> tinkoffCurrencies = grpcClient->findCurrencies(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffCurrencies != nullptr)
    {
        for (int i = 0; i < tinkoffCurrencies->instruments_size(); ++i)
        {
            const tinkoff::Currency& tinkoffCurrency = tinkoffCurrencies->instruments(i);

            if (tinkoffCurrency.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffCurrency.uid());

                Instrument instrument;
                instrument.ticker            = QString::fromStdString(tinkoffCurrency.ticker());
                instrument.name              = QString::fromStdString(tinkoffCurrency.name());
                instrument.lot               = tinkoffCurrency.lot();
                instrument.pricePrecision    = quotationPrecision(tinkoffCurrency.min_price_increment());
                instrument.minPriceIncrement = quotationConvert(tinkoffCurrency.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffCurrency.brand().logo_name())));
            }
        }
    }

    // Use ruble name instead of ruble ticker
    Instrument& rubleInstrument = res[RUBLE_UID];
    rubleInstrument.ticker      = rubleInstrument.name;
}

static void
obtainInstrumentsFromEtfs(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::EtfsResponse> tinkoffEtfs = grpcClient->findEtfs(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffEtfs != nullptr)
    {
        for (int i = 0; i < tinkoffEtfs->instruments_size(); ++i)
        {
            const tinkoff::Etf& tinkoffEtf = tinkoffEtfs->instruments(i);

            if (tinkoffEtf.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffEtf.uid());

                Instrument instrument;
                instrument.ticker            = QString::fromStdString(tinkoffEtf.ticker());
                instrument.name              = QString::fromStdString(tinkoffEtf.name());
                instrument.lot               = tinkoffEtf.lot();
                instrument.pricePrecision    = quotationPrecision(tinkoffEtf.min_price_increment());
                instrument.minPriceIncrement = quotationConvert(tinkoffEtf.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffEtf.brand().logo_name())));
            }
        }
    }
}

static void
obtainInstrumentsFromFutures(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::FuturesResponse> tinkoffFutures = grpcClient->findFutures(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffFutures != nullptr)
    {
        for (int i = 0; i < tinkoffFutures->instruments_size(); ++i)
        {
            const tinkoff::Future& tinkoffFuture = tinkoffFutures->instruments(i);

            if (tinkoffFuture.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffFuture.uid());

                Instrument instrument;
                instrument.ticker            = QString::fromStdString(tinkoffFuture.ticker());
                instrument.name              = QString::fromStdString(tinkoffFuture.name());
                instrument.lot               = tinkoffFuture.lot();
                instrument.pricePrecision    = quotationPrecision(tinkoffFuture.min_price_increment());
                instrument.minPriceIncrement = quotationConvert(tinkoffFuture.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffFuture.brand().logo_name())));
            }
        }
    }
}

using InstrumentHandler =
    void (*)(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos);

// clang-format off
static const QMap<tinkoff::InstrumentType, InstrumentHandler> INSTRUMENT_TYPE_TO_HANDLER{ // clazy:exclude=non-pod-global-static
    {tinkoff::INSTRUMENT_TYPE_SHARE,    obtainInstrumentsFromShares},
    {tinkoff::INSTRUMENT_TYPE_BOND,     obtainInstrumentsFromBonds},
    {tinkoff::INSTRUMENT_TYPE_CURRENCY, obtainInstrumentsFromCurrencies},
    {tinkoff::INSTRUMENT_TYPE_ETF,      obtainInstrumentsFromEtfs},
    {tinkoff::INSTRUMENT_TYPE_FUTURES,  obtainInstrumentsFromFutures}
};
// clang-format on

struct ObtainInstrumentsInfo
{
    explicit ObtainInstrumentsInfo(IGrpcClient* _grpcClient, const QList<tinkoff::InstrumentType>& instrumentTypes) :
        grpcClient(_grpcClient)
    {
        results.resize(instrumentTypes.size());
        logos.resize(instrumentTypes.size());

        resultsArray = results.data();
        logosArray   = logos.data();
    }

    IGrpcClient*                      grpcClient;
    QList<Instruments>                results; // UID => Instrument
    QList<QList<InstrumentIdAndLogo>> logos;   // UID => Logo
    Instruments*                      resultsArray;
    QList<InstrumentIdAndLogo>*       logosArray;
};

static void obtainInstrumentsForParallel(
    QThread* parentThread,
    int /*threadId*/,
    tinkoff::InstrumentType* instrumentTypes,
    int /*size*/,
    int   start,
    int   end,
    void* additionalArgs
)
{
    const ObtainInstrumentsInfo* obtainInstrumentsInfo = reinterpret_cast<ObtainInstrumentsInfo*>(additionalArgs);

    IGrpcClient*                grpcClient   = obtainInstrumentsInfo->grpcClient;
    Instruments*                resultsArray = obtainInstrumentsInfo->resultsArray;
    QList<InstrumentIdAndLogo>* logosArray   = obtainInstrumentsInfo->logosArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        InstrumentHandler instrumentHandler = INSTRUMENT_TYPE_TO_HANDLER.value(instrumentTypes[i]);
        instrumentHandler(parentThread, grpcClient, resultsArray[i], logosArray[i]);
    }
}

void StockCollectThread::downloadLogo(const QString& instrumentId, const QUrl& url)
{
    const IHttpClient::Headers headers;
    const HttpResult           httpResult = mHttpClient->get(url, headers);

    QPixmap logo;
    bool    good = false;

    if (httpResult.statusCode == HTTP_STATUS_CODE_OK && httpResult.body.size() > 0)
    {
        good = logo.loadFromData(httpResult.body, "PNG");

        if (good)
        {
            logo = logo.scaled(LOGO_SIZE, LOGO_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }

    if (!good)
    {
        const std::shared_ptr<IFile> noImageFile = mFileFactory->newInstance(":/assets/images/no_image.png");

        bool ok = noImageFile->open(QIODevice::ReadOnly);
        Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

        const QByteArray content = noImageFile->readAll();
        noImageFile->close();

        ok = logo.loadFromData(content, "PNG");
        Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");
    }

    mLogosStorage->writeLock();
    mLogosStorage->setLogo(instrumentId, logo);
    mLogosStorage->writeUnlock();
}

struct DownloadLogosInfo
{
    explicit DownloadLogosInfo(StockCollectThread* _thread, IFileFactory* _fileFactory, bool _forceToDownload) :
        thread(_thread),
        fileFactory(_fileFactory),
        forceToDownload(_forceToDownload),
        finished()
    {
    }

    StockCollectThread* thread;
    IFileFactory*       fileFactory;
    bool                forceToDownload;
    QAtomicInt          finished;
};

static void downloadLogosForParallel(
    QThread* parentThread, int /*threadId*/, InstrumentIdAndLogo* logos, int size, int start, int end, void* additionalArgs
)
{
    DownloadLogosInfo*  downloadLogosInfo = reinterpret_cast<DownloadLogosInfo*>(additionalArgs);
    StockCollectThread* thread            = downloadLogosInfo->thread;
    const IFileFactory* fileFactory       = downloadLogosInfo->fileFactory;
    const bool          forceToDownload   = downloadLogosInfo->forceToDownload;

    const QString appDir = qApp->applicationDirPath();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const std::shared_ptr<IFile> logoFile =
            fileFactory->newInstance(QString("%1/data/instruments/logos/%2.png").arg(appDir, logos[i].instrumentId));

        if (forceToDownload || !logoFile->exists())
        {
            const QString logoName = logos[i].logo.replace(".png", "x160.png"); // 160 pixels
            const QUrl    url      = QUrl(QString("https://invest-brands.cdn-tinkoff.ru/%1").arg(logoName));

            thread->downloadLogo(logos[i].instrumentId, url);
        }

        downloadLogosInfo->finished++;

        emit thread->notifyInstrumentsProgress(
            StockCollectThread::tr("Downloading logos") +
            QString(" (%1 / %2)").arg(QString::number(downloadLogosInfo->finished), QString::number(size))
        );
    }
}

void StockCollectThread::storeNewInstrumentsInfo()
{
    QList<tinkoff::InstrumentType> instrumentTypes{
        tinkoff::INSTRUMENT_TYPE_SHARE,
        tinkoff::INSTRUMENT_TYPE_BOND,
        tinkoff::INSTRUMENT_TYPE_CURRENCY,
        tinkoff::INSTRUMENT_TYPE_ETF,
        tinkoff::INSTRUMENT_TYPE_FUTURES,
    };

    ObtainInstrumentsInfo obtainInstrumentsInfo(mGrpcClient, instrumentTypes);
    processInParallel(QThread::currentThread(), instrumentTypes, obtainInstrumentsForParallel, &obtainInstrumentsInfo);

    Instruments                instruments; // UID => Instrument
    QList<InstrumentIdAndLogo> logos;

    for (int i = 0; i < instrumentTypes.size(); ++i)
    {
        instruments.insert(obtainInstrumentsInfo.resultsArray[i]);
        logos.append(obtainInstrumentsInfo.logosArray[i]);
    }

    emit notifyInstrumentsProgress(tr("Downloading logos"));

    static int lastDownloadHour = -1;
    const int  currentHour      = QDateTime::currentDateTime().time().hour();

    if (lastDownloadHour < 0)
    {
        lastDownloadHour = currentHour;
    }

    DownloadLogosInfo downloadLogosInfo(this, mFileFactory, lastDownloadHour == currentHour);
    processInParallel(QThread::currentThread(), logos, downloadLogosForParallel, &downloadLogosInfo);

    mInstrumentsStorage->writeLock();
    mInstrumentsStorage->mergeInstruments(instruments);
    mInstrumentsStorage->writeUnlock();
}

void StockCollectThread::reloadBiDirInfos()
{
    mBiDirInfosStorage->writeLock();
    mBiDirInfosStorage->readFromDatabase();
    mBiDirInfosStorage->writeUnlock();
}

bool StockCollectThread::storeNewStocksInfo(const std::shared_ptr<tinkoff::SharesResponse>& tinkoffStocks)
{
    QList<StockMeta> stocksMeta;

    stocksMeta.reserve(tinkoffStocks->instruments_size());

    mUserStorage->readLock();
    const bool qualifiedUser = mUserStorage->isQualified();
    mUserStorage->readUnlock();

    mLogosStorage->readLock();

    for (int i = 0; i < tinkoffStocks->instruments_size(); ++i)
    {
        const tinkoff::Share& tinkoffStock = tinkoffStocks->instruments(i);

        const QString instrumentId = QString::fromStdString(tinkoffStock.uid());

        if (tinkoffStock.currency() == "rub" && tinkoffStock.api_trade_available_flag() && tinkoffStock.buy_available_flag() &&
            tinkoffStock.sell_available_flag() && (qualifiedUser || !LOCKED_STOCKS.contains(instrumentId)))
        {
            StockMeta stockMeta;

            stockMeta.instrumentId        = instrumentId;
            stockMeta.instrumentLogo      = mLogosStorage->getLogo(instrumentId);
            stockMeta.instrumentTicker    = QString::fromStdString(tinkoffStock.ticker());
            stockMeta.instrumentName      = QString::fromStdString(tinkoffStock.name());
            stockMeta.forQualInvestorFlag = tinkoffStock.for_qual_investor_flag();
            stockMeta.lot                 = tinkoffStock.lot();
            stockMeta.pricePrecision      = quotationPrecision(tinkoffStock.min_price_increment());
            stockMeta.minPriceIncrement   = quotationToFloat(tinkoffStock.min_price_increment());
            stockMeta.ignore              = IGNORE_STOCKS.contains(instrumentId);

            stocksMeta.append(stockMeta);
        }
    }

    mLogosStorage->readUnlock();

    mStocksStorage->writeLock();
    const bool res = mStocksStorage->mergeStocksMeta(stocksMeta);
    mStocksStorage->writeUnlock();

    return res;
}

static void getCandlesWithGrpc(
    QThread*        parentThread,
    IStocksStorage* stocksStorage,
    IGrpcClient*    grpcClient,
    Stock*          stock,
    qint64          startTimestamp,
    qint64          endTimestamp
)
{
    // Round to 1 minute
    startTimestamp = (startTimestamp / ONE_MINUTE) * ONE_MINUTE;
    endTimestamp   = (endTimestamp / ONE_MINUTE + 1) * ONE_MINUTE;

    QList<StockData> data;
    data.resizeForOverwrite((endTimestamp - startTimestamp) / ONE_MINUTE);
    StockData* dataArray = data.data();

    int lastIndex = data.size() - 1;

    while (true)
    {
        const std::shared_ptr<tinkoff::GetCandlesResponse> tinkoffCandles =
            grpcClient->getCandles(parentThread, stock->meta.instrumentId, startTimestamp, endTimestamp);

        if (parentThread->isInterruptionRequested() || tinkoffCandles == nullptr || tinkoffCandles->candles_size() == 0)
        {
            if (tinkoffCandles != nullptr && tinkoffCandles->candles_size() == 0)
            {
                stocksStorage->appendStockData(stock, &dataArray[lastIndex + 1], data.size() - lastIndex - 1);
            }

            break;
        }

        for (int i = tinkoffCandles->candles_size() - 1; i >= 0; --i)
        {
            const tinkoff::HistoricCandle& candle = tinkoffCandles->candles(i);

            if (candle.is_complete())
            {
                StockData* stockData = &dataArray[lastIndex];

                stockData->timestamp = timeToTimestamp(candle.time());
                stockData->quantity  = candle.volume();
                stockData->price     = quotationToFloat(candle.close());

                --lastIndex;
            }
        }

        endTimestamp = dataArray[lastIndex + 1].timestamp;
    }
}

static int getCandlesFromZipFile(
    QThread*                      parentThread,
    IQZipFactory*                 qZipFactory,
    IQZipFileFactory*             qZipFileFactory,
    qint64                        startTimestamp,
    qint64                        endTimestamp,
    const std::shared_ptr<IFile>& stockDataFile,
    StockData*                    dataArray
)
{
    int indexOffset = 0;

    const std::shared_ptr<IQZip> stockDataZipFile = qZipFactory->newInstance(stockDataFile->getDevice());

    if (stockDataZipFile->open(QuaZip::mdUnzip))
    {
        const std::shared_ptr<IQZipFile> stockZippedFile = qZipFileFactory->newInstance(stockDataZipFile->getZip());

        QStringList zippedFiles = stockDataZipFile->getFileNameList();
        zippedFiles.sort();

        for (int i = 0; i < zippedFiles.size() && !parentThread->isInterruptionRequested(); ++i)
        {
            stockDataZipFile->setCurrentFile(zippedFiles.at(i));

            stockZippedFile->open(QIODevice::ReadOnly);
            const QString content = QString::fromUtf8(stockZippedFile->readAll());
            stockZippedFile->close();

            const QStringList csvLines = content.split('\n');

            for (const QString& csvLine : csvLines)
            {
                const QStringList csvFields = csvLine.split(';');

                if (csvFields.size() > CSV_FIELD_VOLUME)
                {
                    const qint64 timestamp =
                        QDateTime::fromString(csvFields.at(CSV_FIELD_TIMESTAMP), Qt::ISODate).toMSecsSinceEpoch();

                    if (timestamp >= startTimestamp && timestamp < endTimestamp)
                    {
                        StockData* stockData = &dataArray[indexOffset];

                        stockData->timestamp = timestamp;
                        stockData->quantity  = csvFields.at(CSV_FIELD_VOLUME).toLongLong();
                        stockData->price     = csvFields.at(CSV_FIELD_CLOSE_PRICE).toFloat();

                        ++indexOffset;
                    }
                }
            }
        }

        stockDataZipFile->close();
    }

    return indexOffset;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
static void getCandlesWithHttp(
    QThread*          parentThread,
    IStocksStorage*   stocksStorage,
    IFileFactory*     fileFactory,
    IQZipFactory*     qZipFactory,
    IQZipFileFactory* qZipFileFactory,
    ITimeUtils*       timeUtils,
    IHttpClient*      httpClient,
    Stock*            stock,
    qint64            startTimestamp,
    qint64            endTimestamp
)
{
    const QString appDir = qApp->applicationDirPath();

    // Round to 1 minute
    startTimestamp = (startTimestamp / ONE_MINUTE) * ONE_MINUTE;
    endTimestamp   = (endTimestamp / ONE_MINUTE + 1) * ONE_MINUTE;

    QList<StockData> data;
    data.resizeForOverwrite((endTimestamp - startTimestamp) / ONE_MINUTE);
    StockData* dataArray = data.data();

    int indexOffset = 0;

    const int startYear = QDateTime::fromMSecsSinceEpoch(startTimestamp).date().year();
    const int endYear   = QDateTime::fromMSecsSinceEpoch(endTimestamp).date().year();

    for (int year = startYear; year <= endYear && !parentThread->isInterruptionRequested(); ++year)
    {
        const QString zipFilePath =
            QString("%1/cache/stocks/%2_%3.zip").arg(appDir, stock->meta.instrumentId, QString::number(year));

        const std::shared_ptr<IFile> stockDataFile = fileFactory->newInstance(zipFilePath);

        if (year == endYear || !stockDataFile->exists())
        {
            QUrl url = QUrl("https://invest-public-api.tinkoff.ru/history-data");

            QUrlQuery query;

            query.addQueryItem("instrumentId", stock->meta.instrumentId);
            query.addQueryItem("year", QString::number(year));

            url.setQuery(query.query());

            IHttpClient::Headers headers;

            while (true)
            {
                const HttpResult httpResult = httpClient->get(url, headers);

                if (parentThread->isInterruptionRequested() ||
                    (httpResult.statusCode != HTTP_STATUS_CODE_OK && httpResult.statusCode != HTTP_STATUS_CODE_TOO_MANY_REQUESTS))
                {
                    break;
                }

                if (httpResult.statusCode == HTTP_STATUS_CODE_OK)
                {
                    const bool ok = stockDataFile->open(QIODevice::WriteOnly);
                    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

                    stockDataFile->write(httpResult.body);
                    stockDataFile->close();

                    break;
                }

                if (timeUtils->interruptibleSleep(SLEEP_DELAY, parentThread))
                {
                    break;
                }
            }
        }

        indexOffset += getCandlesFromZipFile(
            parentThread, qZipFactory, qZipFileFactory, startTimestamp, endTimestamp, stockDataFile, &dataArray[indexOffset]
        );
    }

    stocksStorage->appendStockData(stock, dataArray, indexOffset);
}
// NOLINTEND(readability-function-cognitive-complexity)

struct GetCandlesInfo
{
    explicit GetCandlesInfo(
        StockCollectThread* _thread,
        IConfig*            _config,
        IStocksStorage*     _stocksStorage,
        IFileFactory*       _fileFactory,
        IQZipFactory*       _qZipFactory,
        IQZipFileFactory*   _qZipFileFactory,
        ITimeUtils*         _timeUtils,
        IHttpClient*        _httpClient,
        IGrpcClient*        _grpcClient,
        qint64              _currentTimestamp
    ) :
        thread(_thread),
        config(_config),
        stocksStorage(_stocksStorage),
        fileFactory(_fileFactory),
        qZipFactory(_qZipFactory),
        qZipFileFactory(_qZipFileFactory),
        timeUtils(_timeUtils),
        httpClient(_httpClient),
        grpcClient(_grpcClient),
        currentTimestamp(_currentTimestamp),
        finished()
    {
    }

    StockCollectThread* thread;
    IConfig*            config;
    IStocksStorage*     stocksStorage;
    IFileFactory*       fileFactory;
    IQZipFactory*       qZipFactory;
    IQZipFileFactory*   qZipFileFactory;
    ITimeUtils*         timeUtils;
    IHttpClient*        httpClient;
    IGrpcClient*        grpcClient;
    qint64              currentTimestamp;
    QAtomicInt          finished;
};

static void
getCandlesForParallel(QThread* parentThread, int /*threadId*/, Stock** stocks, int size, int start, int end, void* additionalArgs)
{
    GetCandlesInfo*     getCandlesInfo   = reinterpret_cast<GetCandlesInfo*>(additionalArgs);
    StockCollectThread* thread           = getCandlesInfo->thread;
    IConfig*            config           = getCandlesInfo->config;
    IStocksStorage*     stocksStorage    = getCandlesInfo->stocksStorage;
    IFileFactory*       fileFactory      = getCandlesInfo->fileFactory;
    IQZipFactory*       qZipFactory      = getCandlesInfo->qZipFactory;
    IQZipFileFactory*   qZipFileFactory  = getCandlesInfo->qZipFileFactory;
    ITimeUtils*         timeUtils        = getCandlesInfo->timeUtils;
    IHttpClient*        httpClient       = getCandlesInfo->httpClient;
    IGrpcClient*        grpcClient       = getCandlesInfo->grpcClient;
    const qint64        currentTimestamp = getCandlesInfo->currentTimestamp;

    const qint64 storageMonthLimit = static_cast<qint64>(config->getStorageMonthLimit()) * ONE_MONTH;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        qint64 startTimestamp =
            qBound(currentTimestamp - storageMonthLimit, stock->operational.lastStoredTimestamp + ONE_MINUTE, currentTimestamp);

        if (currentTimestamp - startTimestamp >= MAX_GRPC_TIME_LIMIT)
        {
            getCandlesWithHttp(
                parentThread,
                stocksStorage,
                fileFactory,
                qZipFactory,
                qZipFileFactory,
                timeUtils,
                httpClient,
                stock,
                startTimestamp,
                currentTimestamp
            );

            startTimestamp = stock->operational.lastStoredTimestamp + ONE_MINUTE;
        }

        getCandlesWithGrpc(parentThread, stocksStorage, grpcClient, stock, startTimestamp, currentTimestamp);

        stock->writeUnlock();

        getCandlesInfo->finished++;

        emit thread->notifyInstrumentsProgress(
            StockCollectThread::tr("Obtain stocks data") +
            QString(" (%1 / %2)").arg(QString::number(getCandlesInfo->finished), QString::number(size))
        );
    }
}

void StockCollectThread::obtainStocksData()
{
    emit notifyInstrumentsProgress(tr("Obtain stocks data"));

    const std::shared_ptr<IDir> createDir = mDirFactory->newInstance();

    bool ok = createDir->mkpath(qApp->applicationDirPath() + "/cache/stocks");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    GetCandlesInfo getCandlesInfo(
        this,
        mConfig,
        mStocksStorage,
        mFileFactory,
        mQZipFactory,
        mQZipFileFactory,
        mTimeUtils,
        mHttpClient,
        mGrpcClient,
        QDateTime::currentMSecsSinceEpoch()
    );

    mStocksStorage->readLock();
    QList<Stock*> stocks = mStocksStorage->getStocks();
    mStocksStorage->readUnlock();

    processInParallel(QThread::currentThread(), stocks, getCandlesForParallel, &getCandlesInfo);

    const std::shared_ptr<IDir> deleteDir = mDirFactory->newInstance(qApp->applicationDirPath() + "/cache/stocks");

    ok = deleteDir->removeRecursively();
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to delete dir");
}

void StockCollectThread::copyDataToOperational()
{
    mStocksStorage->readLock();
    mStocksStorage->copyDataToOperational(QDateTime::currentMSecsSinceEpoch() - ONE_HOUR);
    mStocksStorage->readUnlock();
}

void StockCollectThread::cleanupOperationalData()
{
    mStocksStorage->readLock();
    mStocksStorage->cleanupOperationalData(QDateTime::currentMSecsSinceEpoch() - ONE_HOUR);
    mStocksStorage->readUnlock();
}

bool StockCollectThread::obtainStocksDayStartPrice()
{
    // Round to 1 day
    const qint64 newDayStartTimestamp = ((QDateTime::currentMSecsSinceEpoch() / ONE_DAY) * ONE_DAY) - MOSCOW_TIME;

    if (mDayStartTimestamp != newDayStartTimestamp)
    {
        mDayStartTimestamp = newDayStartTimestamp;

        mStocksStorage->readLock();
        mStocksStorage->obtainStocksDayStartPrice(mDayStartTimestamp);
        mStocksStorage->readUnlock();

        return true;
    }

    return false;
}

void StockCollectThread::obtainLastTradeTime()
{
    mStocksStorage->readLock();
    mStocksStorage->obtainLastTradeTime(QDateTime::currentMSecsSinceEpoch() - ONE_WEEK);
    mStocksStorage->readUnlock();
}

void StockCollectThread::obtainTurnover()
{
    mStocksStorage->readLock();
    mStocksStorage->obtainTurnover(QDateTime::currentMSecsSinceEpoch() - ONE_MONTH);
    mStocksStorage->readUnlock();
}

void StockCollectThread::obtainPayback()
{
    mStocksStorage->readLock();
    mStocksStorage->obtainPayback(QDateTime::currentMSecsSinceEpoch() - ONE_DAY);
    mStocksStorage->readUnlock();
}

void StockCollectThread::writeStocksMeta()
{
    mStocksStorage->readLock();
    mStocksStorage->writeStocksMeta();
    mStocksStorage->readUnlock();
}

void StockCollectThread::notifyAboutChanges(bool needStocksUpdate, bool needPricesUpdate)
{
    if (needStocksUpdate)
    {
        emit stocksChanged();
    }
    else
    {
        if (needPricesUpdate)
        {
            emit pricesChanged();
        }

        emit periodicDataChanged();
    }
}
