#pragma once



#include <QLabel>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>



class FloatingLabel : public QLabel
{
    Q_OBJECT

public:
    explicit FloatingLabel(const QString& text, QWidget* parent = nullptr);
    ~FloatingLabel() override;

    FloatingLabel(const FloatingLabel& another)            = delete;
    FloatingLabel& operator=(const FloatingLabel& another) = delete;

    void setStartPoint(const QPoint& point);
    void setEndPoint(const QPoint& point);

private:
    QTimer                 mDeathTimer;
    QGraphicsOpacityEffect mOpacityEffect;
    QPropertyAnimation     mOpacityAnimation;
    QPropertyAnimation     mPositionAnimation;

public slots:
    void deathTimerTicked();
};
