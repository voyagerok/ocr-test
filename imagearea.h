#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QWidget>
#include <QLabel>
#include <QRubberBand>

class ImageArea : public QLabel
{

    Q_OBJECT

public:
    ImageArea(QWidget *parent = Q_NULLPTR);
    QRect getSelectionArea();
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    QRubberBand *m_RubberBand{Q_NULLPTR};
    QPoint m_Origin;
};

#endif // IMAGEAREA_H
