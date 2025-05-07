#pragma once



#include <QDialog>

#include <QDate>



class IStartSimulationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IStartSimulationDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    ~IStartSimulationDialog() override = default;

    IStartSimulationDialog(const IStartSimulationDialog& another)            = delete;
    IStartSimulationDialog& operator=(const IStartSimulationDialog& another) = delete;

    [[nodiscard]]
    virtual int startMoney() const = 0;

    [[nodiscard]]
    virtual QString mode() const = 0;

    [[nodiscard]]
    virtual QDate fromDate() const = 0;

    [[nodiscard]]
    virtual QDate toDate() const = 0;

    [[nodiscard]]
    virtual bool bestConfig() const = 0;
};
