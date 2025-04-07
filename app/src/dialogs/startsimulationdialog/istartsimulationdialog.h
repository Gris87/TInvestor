#pragma once



#include <QDialog>



class IStartSimulationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IStartSimulationDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    virtual ~IStartSimulationDialog() = default;

    IStartSimulationDialog(const IStartSimulationDialog& another)            = delete;
    IStartSimulationDialog& operator=(const IStartSimulationDialog& another) = delete;
};
