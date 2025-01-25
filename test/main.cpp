#include <QApplication>

#include "src/config/test_config.h"
#include "src/config/test_settingsdialog.h"



#define CALL_TEST(TestClass) \
    if (run##TestClass(argc, argv) != 0) \
    { \
        return 1; \
    }



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CALL_TEST(Test_Config);
    CALL_TEST(Test_SettingsDialog);

    return 0;
}
