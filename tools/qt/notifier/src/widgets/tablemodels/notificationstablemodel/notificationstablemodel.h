#pragma once



#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel.h"



class NotificationsTableModel : public INotificationsTableModel
{
    Q_OBJECT

public:
    explicit NotificationsTableModel(QObject* parent = nullptr);
    ~NotificationsTableModel() override;

    NotificationsTableModel(const NotificationsTableModel& another)            = delete;
    NotificationsTableModel& operator=(const NotificationsTableModel& another) = delete;

    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void setFilter(const Filter& filter) override;
    void notificationsRead(const QList<NotificationInfo>& notifications) override;
    void notificationsAdded(const QList<NotificationInfo>& notifications) override;
    void refreshBackground() override;

private:
    void sortEntries();
    void reverseEntries();
    int  indexOfSortedInsert(QList<NotificationInfo>* entries, const NotificationInfo& entry);
    void insertRow(QList<NotificationInfo>* entries, int row, const NotificationInfo& entry);
    void filterAll();

    QStringList                              mHeader;
    QStringList                              mMessageTypes;
    Filter                                   mFilter;
    std::shared_ptr<QList<NotificationInfo>> mEntriesUnfiltered;
    std::shared_ptr<QList<NotificationInfo>> mEntries;
    int                                      mSortColumn;
    Qt::SortOrder                            mSortOrder;
};
