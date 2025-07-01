#include "src/widgets/logsfilterwidget/logsfilterwidget.h"
#include "ui_logsfilterwidget.h"

#include <gtest/gtest.h>



class Test_LogsFilterWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsFilterWidget = new LogsFilterWidget();
    }

    void TearDown() override
    {
        delete logsFilterWidget;
    }

    LogsFilterWidget* logsFilterWidget;
};



TEST_F(Test_LogsFilterWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsFilterWidget, Test_on_verboseButton_clicked)
{
    logsFilterWidget->ui->debugButton->click();
    logsFilterWidget->ui->verboseButton->click();

    const LogFilter filter = logsFilterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.level,                                     LOG_LEVEL_VERBOSE);
    ASSERT_EQ(filter.ticker,                                    "");
    ASSERT_EQ(logsFilterWidget->ui->verboseButton->isChecked(), true);
    ASSERT_EQ(logsFilterWidget->ui->debugButton->isChecked(),   false);
    ASSERT_EQ(logsFilterWidget->ui->infoButton->isChecked(),    false);
    ASSERT_EQ(logsFilterWidget->ui->warningButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->errorButton->isChecked(),   false);
    // clang-format on
}

TEST_F(Test_LogsFilterWidget, Test_on_debugButton_clicked)
{
    logsFilterWidget->ui->debugButton->click();

    const LogFilter filter = logsFilterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.level,                                     LOG_LEVEL_DEBUG);
    ASSERT_EQ(filter.ticker,                                    "");
    ASSERT_EQ(logsFilterWidget->ui->verboseButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->debugButton->isChecked(),   true);
    ASSERT_EQ(logsFilterWidget->ui->infoButton->isChecked(),    false);
    ASSERT_EQ(logsFilterWidget->ui->warningButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->errorButton->isChecked(),   false);
    // clang-format on
}

TEST_F(Test_LogsFilterWidget, Test_on_infoButton_clicked)
{
    logsFilterWidget->ui->infoButton->click();

    const LogFilter filter = logsFilterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.level,                                     LOG_LEVEL_INFO);
    ASSERT_EQ(filter.ticker,                                    "");
    ASSERT_EQ(logsFilterWidget->ui->verboseButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->debugButton->isChecked(),   false);
    ASSERT_EQ(logsFilterWidget->ui->infoButton->isChecked(),    true);
    ASSERT_EQ(logsFilterWidget->ui->warningButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->errorButton->isChecked(),   false);
    // clang-format on
}

TEST_F(Test_LogsFilterWidget, Test_on_warningButton_clicked)
{
    logsFilterWidget->ui->warningButton->click();

    const LogFilter filter = logsFilterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.level,                                     LOG_LEVEL_WARNING);
    ASSERT_EQ(filter.ticker,                                    "");
    ASSERT_EQ(logsFilterWidget->ui->verboseButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->debugButton->isChecked(),   false);
    ASSERT_EQ(logsFilterWidget->ui->infoButton->isChecked(),    false);
    ASSERT_EQ(logsFilterWidget->ui->warningButton->isChecked(), true);
    ASSERT_EQ(logsFilterWidget->ui->errorButton->isChecked(),   false);
    // clang-format on
}

TEST_F(Test_LogsFilterWidget, Test_on_errorButton_clicked)
{
    logsFilterWidget->ui->errorButton->click();

    const LogFilter filter = logsFilterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.level,                                     LOG_LEVEL_ERROR);
    ASSERT_EQ(filter.ticker,                                    "");
    ASSERT_EQ(logsFilterWidget->ui->verboseButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->debugButton->isChecked(),   false);
    ASSERT_EQ(logsFilterWidget->ui->infoButton->isChecked(),    false);
    ASSERT_EQ(logsFilterWidget->ui->warningButton->isChecked(), false);
    ASSERT_EQ(logsFilterWidget->ui->errorButton->isChecked(),   true);
    // clang-format on
}

TEST_F(Test_LogsFilterWidget, Test_on_tickerLineEdit_textChanged_and_filterChangeDelayTimerTicked)
{
    logsFilterWidget->ui->tickerLineEdit->setText("aaaaa");

    const LogFilter filter = logsFilterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.level,                                        LOG_LEVEL_VERBOSE);
    ASSERT_EQ(filter.ticker,                                       "aaaaa");
    ASSERT_EQ(logsFilterWidget->filterChangeDelayTimer.interval(), 1000);
    ASSERT_EQ(logsFilterWidget->filterChangeDelayTimer.isActive(), true);
    // clang-format on

    logsFilterWidget->filterChangeDelayTimerTicked();

    // clang-format off
    ASSERT_EQ(logsFilterWidget->filterChangeDelayTimer.interval(), 1000);
    ASSERT_EQ(logsFilterWidget->filterChangeDelayTimer.isActive(), false);
    // clang-format on
}
