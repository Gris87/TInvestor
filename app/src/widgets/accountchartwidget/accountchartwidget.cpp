#include "src/widgets/accountchartwidget/accountchartwidget.h"
#include "ui_accountchartwidget.h"

#include <QDebug>



AccountChartWidget::AccountChartWidget(IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent) :
    IAccountChartWidget(parent),
    ui(new Ui::AccountChartWidget),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mYieldSeries(),
    mYieldAxisX(),
    mYieldAxisY(),
    mMonthlyYieldSeries(),
    mMonthlyYieldPositiveBarSet("Positive"),
    mMonthlyYieldNegativeBarSet("Negative"),
    mMonthlyYieldPositivePoints(),
    mMonthlyYieldNegativePoints(),
    mMonthlyYieldAxisX(),
    mMonthlyYieldAxisY(),
    mDailyYieldSeries(),
    mDailyYieldPositiveBarSet("Positive"),
    mDailyYieldNegativeBarSet("Negative"),
    mDailyYieldPositivePoints(),
    mDailyYieldNegativePoints(),
    mDailyYieldAxisX(),
    mDailyYieldAxisY(),
    mTotalMoneySeries(),
    mTotalMoneyAxisX(),
    mTotalMoneyAxisY(),
    mMonthNames()
{
    qDebug() << "Create AccountChartWidget";

    ui->setupUi(this);

    mMonthNames << tr("Jan") << tr("Feb") << tr("Mar") << tr("Apr") << tr("May") << tr("Jun") << tr("Jul") << tr("Aug")
                << tr("Sep") << tr("Oct") << tr("Nov") << tr("Dec");

    initYieldChart();
    initMonthlyYieldChart();
    initDailyYieldChart();
    initTotalMoneyChart();

    ui->stackedWidget->setCurrentIndex(0);
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
}

void AccountChartWidget::setShowMoney(bool /*value*/)
{
}

void AccountChartWidget::switchChart(ChartType chartType)
{
    ui->stackedWidget->setCurrentIndex(chartType);
}

void AccountChartWidget::setTimeRange(TimeRange /*range*/)
{
}

void AccountChartWidget::operationsRead(const QList<Operation>& /*operations*/)
{
}

void AccountChartWidget::operationsAdded(const QList<Operation>& /*operations*/)
{
}

void AccountChartWidget::initYieldChart()
{
    QList<QObject *> seriesList;
    seriesList.append(&mYieldSeries);

    ui->yieldQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->yieldQuickWidget->setInitialProperties({
        {"seriesList", QVariant::fromValue(seriesList)},
        {"axisX", QVariant::fromValue(&mYieldAxisX)},
        {"axisY", QVariant::fromValue(&mYieldAxisY)}
    });
    ui->yieldQuickWidget->loadFromModule("QtGraphs", "GraphsView");
}

void AccountChartWidget::initMonthlyYieldChart()
{
    QList<QObject *> seriesList;
    seriesList.append(&mMonthlyYieldSeries);

    ui->monthlyYieldQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->monthlyYieldQuickWidget->setInitialProperties({
        {"seriesList", QVariant::fromValue(seriesList)},
        {"axisX", QVariant::fromValue(&mMonthlyYieldAxisX)},
        {"axisY", QVariant::fromValue(&mMonthlyYieldAxisY)}
    });
    ui->monthlyYieldQuickWidget->loadFromModule("QtGraphs", "GraphsView");
}

void AccountChartWidget::initDailyYieldChart()
{
    QList<QObject *> seriesList;
    seriesList.append(&mDailyYieldSeries);

    ui->dailyYieldQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->dailyYieldQuickWidget->setInitialProperties({
        {"seriesList", QVariant::fromValue(seriesList)},
        {"axisX", QVariant::fromValue(&mDailyYieldAxisX)},
        {"axisY", QVariant::fromValue(&mDailyYieldAxisY)}
    });
    ui->dailyYieldQuickWidget->loadFromModule("QtGraphs", "GraphsView");
}

void AccountChartWidget::initTotalMoneyChart()
{
    QList<QObject *> seriesList;
    seriesList.append(&mTotalMoneySeries);

    ui->totalMoneyQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->totalMoneyQuickWidget->setInitialProperties({
        {"seriesList", QVariant::fromValue(seriesList)},
        {"axisX", QVariant::fromValue(&mTotalMoneyAxisX)},
        {"axisY", QVariant::fromValue(&mTotalMoneyAxisY)}
    });
    ui->totalMoneyQuickWidget->loadFromModule("QtGraphs", "GraphsView");
}
