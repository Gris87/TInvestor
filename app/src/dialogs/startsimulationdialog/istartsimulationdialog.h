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
    ~IStartSimulationDialog() override = default;

    IStartSimulationDialog(const IStartSimulationDialog& another)            = delete;
    IStartSimulationDialog& operator=(const IStartSimulationDialog& another) = delete;
};
