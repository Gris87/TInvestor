#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialog.h"



namespace Ui
{
class StartSimulationDialog;
}



class StartSimulationDialog : public IStartSimulationDialog
{
    Q_OBJECT

public:
    explicit StartSimulationDialog(QWidget* parent = nullptr);
    ~StartSimulationDialog();

    StartSimulationDialog(const StartSimulationDialog& another)            = delete;
    StartSimulationDialog& operator=(const StartSimulationDialog& another) = delete;

    Ui::StartSimulationDialog* ui;
};
