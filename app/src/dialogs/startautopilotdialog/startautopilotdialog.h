#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialog.h"

#include "src/storage/user/iuserstorage.h"
#include "src/utils/messagebox/imessagebox.h"
#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class StartAutoPilotDialog;
}



class StartAutoPilotDialog : public IStartAutoPilotDialog
{
    Q_OBJECT

public:
    explicit StartAutoPilotDialog(
        IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, ISettingsEditor* settingsEditor, QWidget* parent = nullptr
    );
    ~StartAutoPilotDialog() override;

    StartAutoPilotDialog(const StartAutoPilotDialog& another)            = delete;
    StartAutoPilotDialog& operator=(const StartAutoPilotDialog& another) = delete;

    Ui::StartAutoPilotDialog* ui;

private:
    QStringList getAccounts();
    void        saveWindowState();
    void        loadWindowState();

    IUserStorage*     mUserStorage;
    IMessageBoxUtils* mMessageBoxUtils;
    ISettingsEditor*  mSettingsEditor;

private slots:
    void on_accountComboBox_currentIndexChanged(int index) const;
    void on_followRadioButton_toggled(bool checked) const;
    void on_startButton_clicked();
};
