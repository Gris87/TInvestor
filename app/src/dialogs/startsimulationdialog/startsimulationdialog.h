#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialog.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class StartSimulationDialog;
}



class StartSimulationDialog : public IStartSimulationDialog
{
    Q_OBJECT

public:
    explicit StartSimulationDialog(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~StartSimulationDialog() override;

    StartSimulationDialog(const StartSimulationDialog& another)            = delete;
    StartSimulationDialog& operator=(const StartSimulationDialog& another) = delete;

    [[nodiscard]]
    int startMoney() const override;

    [[nodiscard]]
    QString mode() const override;

    [[nodiscard]]
    QDate fromDate() const override;

    [[nodiscard]]
    QDate toDate() const override;

    [[nodiscard]]
    bool bestConfig() const override;

    Ui::StartSimulationDialog* ui;

private:
    void saveWindowState();
    void loadWindowState();

    ISettingsEditor* mSettingsEditor;

private slots:
    void on_dateRangeRadioButton_toggled(bool checked) const;
    void on_fromDateEdit_dateChanged(const QDate& date) const;
    void on_toDateEdit_dateChanged(const QDate& date) const;
    void on_startButton_clicked();
};
