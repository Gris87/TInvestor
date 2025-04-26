// http://stackoverflow.com/questions/9183050/vertical-qlabel-or-the-equivalent

#ifndef VERTICALLABEL_H
#define VERTICALLABEL_H

#include <QLabel>

class VerticalLabel : public QLabel
{
    Q_OBJECT

public:
    explicit VerticalLabel(QWidget* parent = nullptr);
    explicit VerticalLabel(const QString& text, QWidget* parent = nullptr);

    void setColor(const QColor& color);

    [[nodiscard]]
    const QColor& color() const;

protected:
    void paintEvent(QPaintEvent*) override;

    [[nodiscard]]
    QSize sizeHint() const override;

    [[nodiscard]]
    QSize minimumSizeHint() const override;

private:
    QColor mColor;
};

#endif // VERTICALLABEL_H
