#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialog.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class StartAutoPilotDialog;
}



class StartAutoPilotDialog : public IStartAutoPilotDialog
{
    Q_OBJECT

public:
    explicit StartAutoPilotDialog(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~StartAutoPilotDialog();

    StartAutoPilotDialog(const StartAutoPilotDialog& another)            = delete;
    StartAutoPilotDialog& operator=(const StartAutoPilotDialog& another) = delete;

    Ui::StartAutoPilotDialog* ui;

private:
    void saveWindowState();
    void loadWindowState();

    ISettingsEditor* mSettingsEditor;

private slots:
    void on_accountComboBox_currentIndexChanged(int index);
    void on_followRadioButton_toggled(bool checked);
    void on_startButton_clicked();
};
