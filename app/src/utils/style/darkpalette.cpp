#include "src/utils/style/darkpalette.h"

#include <QDebug>



DarkPalette::DarkPalette() :
    QPalette()
{
    qDebug() << "Create DarkPalette";

    // clang-format off
    setColor(QPalette::Active, QPalette::Window,          QColor("#1E2C39"));
    setColor(QPalette::Active, QPalette::WindowText,      QColor("#AFC2D7"));
    setColor(QPalette::Active, QPalette::Base,            QColor("#2C3C4B"));
    setColor(QPalette::Active, QPalette::AlternateBase,   QColor("#344759"));
    setColor(QPalette::Active, QPalette::ToolTipBase,     QColor("#334657"));
    setColor(QPalette::Active, QPalette::ToolTipText,     QColor("#A0B3C7"));
    setColor(QPalette::Active, QPalette::PlaceholderText, QColor("#718799"));
    setColor(QPalette::Active, QPalette::Text,            QColor("#AFC2D7"));
    setColor(QPalette::Active, QPalette::Button,          QColor("#243442"));
    setColor(QPalette::Active, QPalette::ButtonText,      QColor("#819BB1"));
    setColor(QPalette::Active, QPalette::BrightText,      QColor("#FFFFFF"));
    setColor(QPalette::Active, QPalette::Light,           QColor("#314659"));
    setColor(QPalette::Active, QPalette::Midlight,        QColor("#2A3C4C"));
    setColor(QPalette::Active, QPalette::Dark,            QColor("#151E26"));
    setColor(QPalette::Active, QPalette::Mid,             QColor("#1C2833"));
    setColor(QPalette::Active, QPalette::Shadow,          QColor("#000000"));
    setColor(QPalette::Active, QPalette::Highlight,       QColor("#4C6882"));
    setColor(QPalette::Active, QPalette::Accent ,         QColor("#4C6882"));
    setColor(QPalette::Active, QPalette::HighlightedText, QColor("#FFFFFF"));
    setColor(QPalette::Active, QPalette::Link,            QColor("#0078D7"));
    setColor(QPalette::Active, QPalette::LinkVisited,     QColor("#002642"));
    // clang-format on

    // clang-format off
    setColor(QPalette::Inactive, QPalette::Window,          QColor("#1E2C39"));
    setColor(QPalette::Inactive, QPalette::WindowText,      QColor("#AFC2D7"));
    setColor(QPalette::Inactive, QPalette::Base,            QColor("#2C3C4B"));
    setColor(QPalette::Inactive, QPalette::AlternateBase,   QColor("#344759"));
    setColor(QPalette::Inactive, QPalette::ToolTipBase,     QColor("#334657"));
    setColor(QPalette::Inactive, QPalette::ToolTipText,     QColor("#A0B3C7"));
    setColor(QPalette::Inactive, QPalette::PlaceholderText, QColor("#718799"));
    setColor(QPalette::Inactive, QPalette::Text,            QColor("#AFC2D7"));
    setColor(QPalette::Inactive, QPalette::Button,          QColor("#243442"));
    setColor(QPalette::Inactive, QPalette::ButtonText,      QColor("#819BB1"));
    setColor(QPalette::Inactive, QPalette::BrightText,      QColor("#FFFFFF"));
    setColor(QPalette::Inactive, QPalette::Light,           QColor("#314659"));
    setColor(QPalette::Inactive, QPalette::Midlight,        QColor("#2A3C4C"));
    setColor(QPalette::Inactive, QPalette::Dark,            QColor("#151E26"));
    setColor(QPalette::Inactive, QPalette::Mid,             QColor("#1C2833"));
    setColor(QPalette::Inactive, QPalette::Shadow,          QColor("#000000"));
    setColor(QPalette::Inactive, QPalette::Highlight,       QColor("#4C6882"));
    setColor(QPalette::Inactive, QPalette::Accent ,         QColor("#4C6882"));
    setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("#000000"));
    setColor(QPalette::Inactive, QPalette::Link,            QColor("#0078D7"));
    setColor(QPalette::Inactive, QPalette::LinkVisited,     QColor("#002642"));
    // clang-format on

    // clang-format off
    setColor(QPalette::Disabled, QPalette::Window,          QColor("#1E2C39"));
    setColor(QPalette::Disabled, QPalette::WindowText,      QColor("#151E26"));
    setColor(QPalette::Disabled, QPalette::Base,            QColor("#1E2C39"));
    setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#344759"));
    setColor(QPalette::Disabled, QPalette::ToolTipBase,     QColor("#334657"));
    setColor(QPalette::Disabled, QPalette::ToolTipText,     QColor("#A0B3C7"));
    setColor(QPalette::Disabled, QPalette::PlaceholderText, QColor("#718799"));
    setColor(QPalette::Disabled, QPalette::Text,            QColor("#151E26"));
    setColor(QPalette::Disabled, QPalette::Button,          QColor("#243442"));
    setColor(QPalette::Disabled, QPalette::ButtonText,      QColor("#151E26"));
    setColor(QPalette::Disabled, QPalette::BrightText,      QColor("#FFFFFF"));
    setColor(QPalette::Disabled, QPalette::Light,           QColor("#314659"));
    setColor(QPalette::Disabled, QPalette::Midlight,        QColor("#2A3C4C"));
    setColor(QPalette::Disabled, QPalette::Dark,            QColor("#151E26"));
    setColor(QPalette::Disabled, QPalette::Mid,             QColor("#1C2833"));
    setColor(QPalette::Disabled, QPalette::Shadow,          QColor("#000000"));
    setColor(QPalette::Disabled, QPalette::Highlight,       QColor("#4C6882"));
    setColor(QPalette::Disabled, QPalette::Accent ,         QColor("#4C6882"));
    setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("#000000"));
    setColor(QPalette::Disabled, QPalette::Link,            QColor("#0078D7"));
    setColor(QPalette::Disabled, QPalette::LinkVisited,     QColor("#002642"));
    // clang-format on
}

DarkPalette::~DarkPalette()
{
    qDebug() << "Destroy DarkPalette";
}
