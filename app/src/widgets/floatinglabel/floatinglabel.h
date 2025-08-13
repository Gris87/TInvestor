#pragma once



#include <QLabel>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>



class FloatingLabel : public QLabel
{
    Q_OBJECT

public:
    explicit FloatingLabel(const QString& text, const QPoint& startPoint, const QPoint& endPoint, QWidget* parent = nullptr);
    ~FloatingLabel() override;

    FloatingLabel(const FloatingLabel& another)            = delete;
    FloatingLabel& operator=(const FloatingLabel& another) = delete;

private:
    QTimer             mDeathTimer;
    QPropertyAnimation mOpacityAnimation;
    QPropertyAnimation mPositionAnimation;

public slots:
    void deathTimerTicked();
};
