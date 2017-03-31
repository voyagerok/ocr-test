#include <QDebug>
#include <QMouseEvent>
#include <QScopedPointer>
#include "imagearea.h"


ImageArea::ImageArea(QWidget *parent) : QLabel(parent){
    qDebug() << "Successfully initialized" << endl;
}

QRect ImageArea::getSelectionArea() {
    if (m_RubberBand == Q_NULLPTR) {
        return QRect();
    }

    return m_RubberBand->geometry();
}

void ImageArea::mousePressEvent(QMouseEvent *ev) {
    if (m_RubberBand == Q_NULLPTR) {
        m_RubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    }
    m_Origin = ev->pos();
    m_RubberBand->setGeometry(QRect(m_Origin, QSize()));
    m_RubberBand->show();
}

void ImageArea::mouseReleaseEvent(QMouseEvent *ev) {
//    qDebug() << "Mouse released" << endl;
    //m_RubberBand->hide();
}

void ImageArea::mouseMoveEvent(QMouseEvent *ev) {
    m_RubberBand->setGeometry(QRect(m_Origin, ev->pos()).normalized());
}
