#include "src/utils/style/darkpalette.h"

#include <QDebug>



DarkPalette::DarkPalette() :
    QPalette()
{
    qDebug() << "Create DarkPalette";

    // clang-format off
    setColor(QPalette::Active, QPalette::Window,          QColor("#1E2C39"));
    setColor(QPalette::Active, QPalette::WindowText,      QColor("#AFC2D7"));
    setColor(QPalette::Active, QPalette::Base,            QColor("#21313F"));
    setColor(QPalette::Active, QPalette::AlternateBase,   QColor("#1E2C39"));
    setColor(QPalette::Active, QPalette::ToolTipBase,     QColor("#334657"));
    setColor(QPalette::Active, QPalette::ToolTipText,     QColor("#A0B3C7"));
    setColor(QPalette::Active, QPalette::PlaceholderText, QColor("#7B8C9E"));
    setColor(QPalette::Active, QPalette::Text,            QColor("#A0B8CF"));
    setColor(QPalette::Active, QPalette::Button,          QColor("#243442"));
    setColor(QPalette::Active, QPalette::ButtonText,      QColor("#8BA5BC"));
    setColor(QPalette::Active, QPalette::BrightText,      QColor("#FFFFFF"));
    setColor(QPalette::Active, QPalette::Light,           QColor("#6CA0CC"));
    setColor(QPalette::Active, QPalette::Midlight,        QColor("#5E8CB2"));
    setColor(QPalette::Active, QPalette::Dark,            QColor("#43647F"));
    setColor(QPalette::Active, QPalette::Mid,             QColor("#517899"));
    setColor(QPalette::Active, QPalette::Shadow,          QColor("#1E2A33"));
    setColor(QPalette::Active, QPalette::Highlight,       QColor("#4C6882"));
    setColor(QPalette::Active, QPalette::Accent ,         QColor("#2C3C4B"));
    setColor(QPalette::Active, QPalette::HighlightedText, QColor("#FFFFFF"));
    setColor(QPalette::Active, QPalette::Link,            QColor("#0078D7"));
    setColor(QPalette::Active, QPalette::LinkVisited,     QColor("#FF00FF"));
    // clang-format on

    // clang-format off
    setColor(QPalette::Disabled, QPalette::Window,          QColor("#1e2c39"));
    setColor(QPalette::Disabled, QPalette::WindowText,      QColor("#afc2d7"));
    setColor(QPalette::Disabled, QPalette::Base,            QColor("#21313f"));
    setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#1e2c39"));
    setColor(QPalette::Disabled, QPalette::ToolTipBase,     QColor("#334657"));
    setColor(QPalette::Disabled, QPalette::ToolTipText,     QColor("#a0b3c7"));
    setColor(QPalette::Disabled, QPalette::PlaceholderText, QColor("#7b8c9e"));
    setColor(QPalette::Disabled, QPalette::Text,            QColor("#a0b8cf"));
    setColor(QPalette::Disabled, QPalette::Button,          QColor("#243442"));
    setColor(QPalette::Disabled, QPalette::ButtonText,      QColor("#8ba5bc"));
    setColor(QPalette::Disabled, QPalette::BrightText,      QColor("#ffffff"));
    setColor(QPalette::Disabled, QPalette::Light,           QColor("#6ca0cc"));
    setColor(QPalette::Disabled, QPalette::Midlight,        QColor("#5e8cb2"));
    setColor(QPalette::Disabled, QPalette::Dark,            QColor("#43647f"));
    setColor(QPalette::Disabled, QPalette::Mid,             QColor("#517899"));
    setColor(QPalette::Disabled, QPalette::Shadow,          QColor("#1e2a33"));
    setColor(QPalette::Disabled, QPalette::Highlight,       QColor("#4c6882"));
    setColor(QPalette::Disabled, QPalette::Accent ,         QColor("#2c3c4b"));
    setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("#000000"));
    setColor(QPalette::Disabled, QPalette::Link,            QColor("#0078d7"));
    setColor(QPalette::Disabled, QPalette::LinkVisited,     QColor("#002642"));
    // clang-format on

    // clang-format off
    setColor(QPalette::Inactive, QPalette::Window,          QColor("#1e2c39"));
    setColor(QPalette::Inactive, QPalette::WindowText,      QColor("#43647f"));
    setColor(QPalette::Inactive, QPalette::Base,            QColor("#1e2c39"));
    setColor(QPalette::Inactive, QPalette::AlternateBase,   QColor("#1e2c39"));
    setColor(QPalette::Inactive, QPalette::ToolTipBase,     QColor("#334657"));
    setColor(QPalette::Inactive, QPalette::ToolTipText,     QColor("#a0b3c7"));
    setColor(QPalette::Inactive, QPalette::PlaceholderText, QColor("#7b8c9e"));
    setColor(QPalette::Inactive, QPalette::Text,            QColor("#43647f"));
    setColor(QPalette::Inactive, QPalette::Button,          QColor("#243442"));
    setColor(QPalette::Inactive, QPalette::ButtonText,      QColor("#43647f"));
    setColor(QPalette::Inactive, QPalette::BrightText,      QColor("#ffffff"));
    setColor(QPalette::Inactive, QPalette::Light,           QColor("#6ca0cc"));
    setColor(QPalette::Inactive, QPalette::Midlight,        QColor("#5e8cb2"));
    setColor(QPalette::Inactive, QPalette::Dark,            QColor("#43647f"));
    setColor(QPalette::Inactive, QPalette::Mid,             QColor("#517899"));
    setColor(QPalette::Inactive, QPalette::Shadow,          QColor("#1e2a33"));
    setColor(QPalette::Inactive, QPalette::Highlight,       QColor("#0078d7"));
    setColor(QPalette::Inactive, QPalette::Accent ,         QColor("#2c3c4b"));
    setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("#ffffff"));
    setColor(QPalette::Inactive, QPalette::Link,            QColor("#0078d7"));
    setColor(QPalette::Inactive, QPalette::LinkVisited,     QColor("#002642"));
    // clang-format on
}

DarkPalette::~DarkPalette()
{
    qDebug() << "Destroy DarkPalette";
}
