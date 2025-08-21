#include "src/widgets/floatinglabel/floatinglabel.h"

#include <QDateTime>
#include <QDebug>



constexpr qint64 MS_IN_SECOND   = 1000LL;
constexpr qint64 DEATH_INTERVAL = 5 * MS_IN_SECOND; // 5 seconds



FloatingLabel::FloatingLabel(const QString& text, const QPoint& startPoint, const QPoint& endPoint, QWidget* parent) :
    QLabel(text, parent),
    mDeathTimer()
{
    qDebug() << "Create FloatingLabel";

    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowTransparentForInput);

    mOpacityAnimation.setTargetObject(this);
    mOpacityAnimation.setPropertyName("windowOpacity");
    mOpacityAnimation.setDuration(DEATH_INTERVAL);
    mOpacityAnimation.setStartValue(1.0f);
    mOpacityAnimation.setEndValue(0.0f);

    mPositionAnimation.setTargetObject(this);
    mPositionAnimation.setPropertyName("pos");
    mPositionAnimation.setDuration(DEATH_INTERVAL);
    mPositionAnimation.setStartValue(startPoint);
    mPositionAnimation.setEndValue(endPoint);

    const QSize labelSizeHint = sizeHint();
    setGeometry(startPoint.x(), startPoint.y(), labelSizeHint.width(), labelSizeHint.height());

    connect(&mDeathTimer, SIGNAL(timeout()), this, SLOT(deathTimerTicked()));

    mOpacityAnimation.start();
    mPositionAnimation.start();
    mDeathTimer.start(DEATH_INTERVAL);
}

FloatingLabel::~FloatingLabel()
{
    qDebug() << "Destroy FloatingLabel";
}

void FloatingLabel::deathTimerTicked()
{
    deleteLater();
}
