#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <gtest/gtest.h>

#include "src/widgets/tablemodels/modelroles.h"



class Test_LogsTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new LogsTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    LogsTableModel* model;
};



TEST_F(Test_LogsTableModel, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsTableModel, Test_rowCount)
{
    ASSERT_EQ(model->rowCount(), 0);

    QList<LogEntry> entries;

    entries.resize(3);

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    entries.resize(5);

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 5);

    entries.clear();

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 0);
}

TEST_F(Test_LogsTableModel, Test_columnCount)
{
    ASSERT_EQ(model->columnCount(), LOGS_COLUMN_COUNT);
}

TEST_F(Test_LogsTableModel, Test_headerData)
{
    // clang-format off
    ASSERT_EQ(model->headerData(LOGS_TIME_COLUMN,    Qt::Horizontal, Qt::DisplayRole), QVariant("Time"));
    ASSERT_EQ(model->headerData(LOGS_LEVEL_COLUMN,   Qt::Horizontal, Qt::DisplayRole), QVariant("L"));
    ASSERT_EQ(model->headerData(LOGS_NAME_COLUMN,    Qt::Horizontal, Qt::DisplayRole), QVariant("Name"));
    ASSERT_EQ(model->headerData(LOGS_MESSAGE_COLUMN, Qt::Horizontal, Qt::DisplayRole), QVariant("Message"));
    ASSERT_EQ(model->headerData(0,                   Qt::Vertical,   Qt::DisplayRole), QVariant(1));
    ASSERT_EQ(model->headerData(1,                   Qt::Vertical,   Qt::DisplayRole), QVariant(2));
    ASSERT_EQ(model->headerData(2,                   Qt::Vertical,   Qt::DisplayRole), QVariant(3));
    ASSERT_EQ(model->headerData(0,                   Qt::Horizontal, Qt::ToolTipRole), QVariant());
    // clang-format on
}

TEST_F(Test_LogsTableModel, Test_data)
{
    Logo logo;

    QList<LogEntry> entries;

    LogEntry entry1;
    LogEntry entry2;
    LogEntry entry3;

    entry1.timestamp        = 1704056520000;
    entry1.level            = LOG_LEVEL_VERBOSE;
    entry1.instrumentId     = "aaaaa";
    entry1.instrumentLogo   = nullptr;
    entry1.instrumentTicker = "ABBA";
    entry1.instrumentName   = "Abstract Basics";
    entry1.message          = "A basket full";

    entry2.timestamp        = 1704056460000;
    entry2.level            = LOG_LEVEL_DEBUG;
    entry2.instrumentId     = "bbbbb";
    entry2.instrumentLogo   = nullptr;
    entry2.instrumentTicker = "BASE";
    entry2.instrumentName   = "Basketball enhancement";
    entry2.message          = "Ball shall levitate";

    entry3.timestamp        = 1704056400000;
    entry3.level            = LOG_LEVEL_INFO;
    entry3.instrumentId     = "ccccc";
    entry3.instrumentLogo   = &logo;
    entry3.instrumentTicker = "CODE";
    entry3.instrumentName   = "Code base";
    entry3.message          = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_VERBOSE));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Code base"));
    ASSERT_EQ(model->data(model->index(0, 0),                   Qt::ToolTipRole),      QVariant());
    // clang-format on
}

TEST_F(Test_LogsTableModel, Test_sort)
{
    Logo logo;

    QList<LogEntry> entries;

    LogEntry entry1;
    LogEntry entry2;
    LogEntry entry3;

    entry1.timestamp        = 1704056520000;
    entry1.level            = LOG_LEVEL_VERBOSE;
    entry1.instrumentId     = "aaaaa";
    entry1.instrumentLogo   = nullptr;
    entry1.instrumentTicker = "ABBA";
    entry1.instrumentName   = "Abstract Basics";
    entry1.message          = "A basket full";

    entry2.timestamp        = 1704056460000;
    entry2.level            = LOG_LEVEL_DEBUG;
    entry2.instrumentId     = "bbbbb";
    entry2.instrumentLogo   = nullptr;
    entry2.instrumentTicker = "BASE";
    entry2.instrumentName   = "Basketball enhancement";
    entry2.message          = "Ball shall levitate";

    entry3.timestamp        = 1704056400000;
    entry3.level            = LOG_LEVEL_INFO;
    entry3.instrumentId     = "ccccc";
    entry3.instrumentLogo   = &logo;
    entry3.instrumentTicker = "CODE";
    entry3.instrumentName   = "Code base";
    entry3.message          = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_VERBOSE));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Code base"));
    // clang-format on

    model->sort(LOGS_TIME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));

    model->sort(LOGS_TIME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->sort(LOGS_LEVEL_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_VERBOSE));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_INFO));

    model->sort(LOGS_LEVEL_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_VERBOSE));

    model->sort(LOGS_NAME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("CODE"));

    model->sort(LOGS_NAME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("ABBA"));

    model->sort(LOGS_MESSAGE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));

    model->sort(LOGS_MESSAGE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("A basket full"));

    model->sort(LOGS_TIME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->sort(LOGS_LEVEL_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN), Qt::DisplayRole), QVariant(LOG_LEVEL_VERBOSE));

    model->sort(LOGS_NAME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN), Qt::DisplayRole), QVariant("ABBA"));

    model->sort(LOGS_MESSAGE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole), QVariant("A basket full"));

    model->sort(LOGS_TIME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
}

TEST_F(Test_LogsTableModel, Test_setFilter)
{
    Logo logo;

    QList<LogEntry> entries;

    LogEntry entry1;
    LogEntry entry2;
    LogEntry entry3;

    entry1.timestamp        = 1704056520000;
    entry1.level            = LOG_LEVEL_VERBOSE;
    entry1.instrumentId     = "aaaaa";
    entry1.instrumentLogo   = nullptr;
    entry1.instrumentTicker = "ABBA";
    entry1.instrumentName   = "Abstract Basics";
    entry1.message          = "A basket full";

    entry2.timestamp        = 1704056460000;
    entry2.level            = LOG_LEVEL_DEBUG;
    entry2.instrumentId     = "bbbbb";
    entry2.instrumentLogo   = nullptr;
    entry2.instrumentTicker = "BASE";
    entry2.instrumentName   = "Basketball enhancement";
    entry2.message          = "Ball shall levitate";

    entry3.timestamp        = 1704056400000;
    entry3.level            = LOG_LEVEL_INFO;
    entry3.instrumentId     = "ccccc";
    entry3.instrumentLogo   = &logo;
    entry3.instrumentTicker = "CODE";
    entry3.instrumentName   = "Code base";
    entry3.message          = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_VERBOSE));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Code base"));
    // clang-format on

    LogFilter filter;
    filter.level = LOG_LEVEL_DEBUG;

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Code base"));
    // clang-format on
}

TEST_F(Test_LogsTableModel, Test_logAdded)
{
    Logo logo;

    QList<LogEntry> entries;

    LogEntry entry1;
    LogEntry entry2;
    LogEntry entry3;

    entry1.timestamp        = 1704056520000;
    entry1.level            = LOG_LEVEL_VERBOSE;
    entry1.instrumentId     = "aaaaa";
    entry1.instrumentLogo   = nullptr;
    entry1.instrumentTicker = "ABBA";
    entry1.instrumentName   = "Abstract Basics";
    entry1.message          = "A basket full";

    entry2.timestamp        = 1704056460000;
    entry2.level            = LOG_LEVEL_DEBUG;
    entry2.instrumentId     = "bbbbb";
    entry2.instrumentLogo   = nullptr;
    entry2.instrumentTicker = "BASE";
    entry2.instrumentName   = "Basketball enhancement";
    entry2.message          = "Ball shall levitate";

    entry3.timestamp        = 1704056400000;
    entry3.level            = LOG_LEVEL_INFO;
    entry3.instrumentId     = "ccccc";
    entry3.instrumentLogo   = &logo;
    entry3.instrumentTicker = "CODE";
    entry3.instrumentName   = "Code base";
    entry3.message          = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_VERBOSE));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Code base"));
    // clang-format on

    LogEntry newEntry;

    newEntry.timestamp        = 1704056580000;
    newEntry.level            = LOG_LEVEL_WARNING;
    newEntry.instrumentId     = "ddddd";
    newEntry.instrumentLogo   = &logo;
    newEntry.instrumentTicker = "DAGA";
    newEntry.instrumentName   = "Da gangster";
    newEntry.message          = "Do you like pizdyuli?";

    model->logAdded(newEntry);
    ASSERT_EQ(model->rowCount(), 4);

    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:03:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(3, LOGS_TIME_COLUMN), Qt::DisplayRole), QVariant("2024-01-01 00:00:00"));

    model->sort(LOGS_TIME_COLUMN, Qt::AscendingOrder);

    // TODO: Finish
}

TEST_F(Test_LogsTableModel, Test_exportToExcel)
{
    Logo logo;

    QList<LogEntry> entries;

    LogEntry entry1;
    LogEntry entry2;
    LogEntry entry3;

    entry1.timestamp        = 1704056520000;
    entry1.level            = LOG_LEVEL_VERBOSE;
    entry1.instrumentId     = "aaaaa";
    entry1.instrumentLogo   = nullptr;
    entry1.instrumentTicker = "ABBA";
    entry1.instrumentName   = "Abstract Basics";
    entry1.message          = "A basket full";

    entry2.timestamp        = 1704056460000;
    entry2.level            = LOG_LEVEL_DEBUG;
    entry2.instrumentId     = "bbbbb";
    entry2.instrumentLogo   = nullptr;
    entry2.instrumentTicker = "BASE";
    entry2.instrumentName   = "Basketball enhancement";
    entry2.message          = "Ball shall levitate";

    entry3.timestamp        = 1704056400000;
    entry3.level            = LOG_LEVEL_INFO;
    entry3.instrumentId     = "ccccc";
    entry3.instrumentLogo   = &logo;
    entry3.instrumentTicker = "CODE";
    entry3.instrumentName   = "Code base";
    entry3.message          = "Can you eat fish?";

    entries << entry1 << entry2 << entry3;

    model->logsRead(entries);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_VERBOSE));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(1, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_DEBUG));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(2, LOGS_TIME_COLUMN),    Qt::DisplayRole),      QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, LOGS_LEVEL_COLUMN),   Qt::DisplayRole),      QVariant(LOG_LEVEL_INFO));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    Qt::DisplayRole),      QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, LOGS_MESSAGE_COLUMN), Qt::DisplayRole),      QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(0));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_LOGO), QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, LOGS_NAME_COLUMN),    ROLE_INSTRUMENT_NAME), QVariant("Code base"));
    // clang-format on

    QXlsx::Document doc;

    model->exportToExcel(doc);

    // clang-format off
    ASSERT_EQ(doc.cellAt(2, LOGS_TIME_COLUMN + 1)->dateTime(), QVariant(QDateTime(QDate(2024, 1, 1), QTime(0, 2, 0))));
    ASSERT_EQ(doc.cellAt(2, LOGS_LEVEL_COLUMN + 1)->value(),   QVariant("VERBOSE"));
    ASSERT_EQ(doc.cellAt(2, LOGS_NAME_COLUMN + 1)->value(),    QVariant("Abstract Basics"));
    ASSERT_EQ(doc.cellAt(2, LOGS_MESSAGE_COLUMN + 1)->value(), QVariant("A basket full"));
    ASSERT_EQ(doc.cellAt(3, LOGS_TIME_COLUMN + 1)->dateTime(), QVariant(QDateTime(QDate(2024, 1, 1), QTime(0, 1, 0))));
    ASSERT_EQ(doc.cellAt(3, LOGS_LEVEL_COLUMN + 1)->value(),   QVariant("DEBUG"));
    ASSERT_EQ(doc.cellAt(3, LOGS_NAME_COLUMN + 1)->value(),    QVariant("Basketball enhancement"));
    ASSERT_EQ(doc.cellAt(3, LOGS_MESSAGE_COLUMN + 1)->value(), QVariant("Ball shall levitate"));
    ASSERT_EQ(doc.cellAt(4, LOGS_TIME_COLUMN + 1)->dateTime(), QVariant(QDate(2024, 1, 1)));
    ASSERT_EQ(doc.cellAt(4, LOGS_LEVEL_COLUMN + 1)->value(),   QVariant("INFO"));
    ASSERT_EQ(doc.cellAt(4, LOGS_NAME_COLUMN + 1)->value(),    QVariant("Code base"));
    ASSERT_EQ(doc.cellAt(4, LOGS_MESSAGE_COLUMN + 1)->value(), QVariant("Can you eat fish?"));
    // clang-format on
}
