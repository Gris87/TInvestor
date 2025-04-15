#pragma once



#include "src/utils/autorunenabler/iautorunenabler.h"

#include "src/utils/fs/file/ifilefactory.h"
#include "src/utils/processrunner/iprocessrunnerfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"



class AutorunEnabler : public IAutorunEnabler
{
public:
    explicit AutorunEnabler(
        ISettingsEditor* autorunSettingsEditor, IProcessRunnerFactory* processRunnerFactory, IFileFactory* fileFactory
    );
    ~AutorunEnabler() override;

    AutorunEnabler(const AutorunEnabler& another)            = delete;
    AutorunEnabler& operator=(const AutorunEnabler& another) = delete;

    void setEnabled(bool enabled) override;

private:
    void enable();
    void disable();

    ISettingsEditor*       mAutorunSettingsEditor;
    IProcessRunnerFactory* mProcessRunnerFactory;
    IFileFactory*          mFileFactory;
};
