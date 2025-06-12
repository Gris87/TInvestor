#pragma once



#include <QAbstractTableModel>



class ILogsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ILogsTableModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~ILogsTableModel() override = default;

    ILogsTableModel(const ILogsTableModel& another)            = delete;
    ILogsTableModel& operator=(const ILogsTableModel& another) = delete;
};
