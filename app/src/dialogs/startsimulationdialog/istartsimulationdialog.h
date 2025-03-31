#pragma once



#include <QDialog>



class IStartSimulationDialog : public QDialog
{
public:
    explicit IStartSimulationDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    virtual ~IStartSimulationDialog() = default;

    IStartSimulationDialog(const IStartSimulationDialog& another)            = delete;
    IStartSimulationDialog& operator=(const IStartSimulationDialog& another) = delete;
};
