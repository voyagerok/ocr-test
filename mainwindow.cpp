#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "alerthelper.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

struct CoordinatesTransformer {
    static QRect transfromRectToRelative(QRect absoluteRect, QRect actualContentRect) {
        int newXValue = absoluteRect.x() - actualContentRect.x();
        int newYValue = absoluteRect.y() - actualContentRect.y();
        return QRect(newXValue, newYValue, absoluteRect.width(), absoluteRect.height());
    }

    static QRect transformRectToAbsolute(QRect parentRect, QRect contentsRectRelative, Qt::Alignment alignement) {
        if (alignement == Qt::AlignCenter) {
            int parentRectCenterX = parentRect.x() + parentRect.width() / 2;
            int parentRectCenterY = parentRect.y() + parentRect.height() / 2;
            int contentAbsX = parentRectCenterX - contentsRectRelative.width() / 2;
            int contentAbsY = parentRectCenterY - contentsRectRelative.height() / 2;
            return QRect(contentAbsX, contentAbsY, contentsRectRelative.width(), contentsRectRelative.height());
        }
        return QRect();
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_imageArea = new ImageArea();
    m_imageArea->setAlignment(Qt::AlignCenter);

    m_scrollAreaLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    m_scrollAreaLayout->addWidget(m_imageArea);
    ui->scrollAreaWidgetContents->setLayout(m_scrollAreaLayout);

    try {
        m_textExtractor = new TextExtractor(TextExtractor::TextLanguage::English);
    } catch (TextExtractorException &exc) {
        qDebug() << exc.message << endl;
    }
}

void MainWindow::openFile() {
    QString picturesDir = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first();
    QString selectedImagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), picturesDir, tr("Images (*.png *.xpm *.jpg)"));
    if (!selectedImagePath.isEmpty()) {
//        image.load(selectedImageName);
        if (image == Q_NULLPTR) {
            image = new QImage(selectedImagePath);
        } else {
            image->load(selectedImagePath);
        }
        m_imageArea->setPixmap(QPixmap::fromImage(*image));
//        m_selectedImagePath = selectedImageName;
    }
}

void MainWindow::extractText() {
//    if (m_selectedImagePath.isEmpty()) {
//        return;
//    }
    if (image == Q_NULLPTR) {
        qDebug() << "Failed to extract text: image is NULL" << endl;
        return;
    }
    QRect selectedArea = m_imageArea->getSelectionArea();
    QRect selectedAreaRelative = CoordinatesTransformer::transfromRectToRelative(selectedArea,
                                                                                 CoordinatesTransformer::transformRectToAbsolute(m_imageArea->rect(),
                                                                                                                                 image->rect(),
                                                                                                                                 m_imageArea->alignment()));
//    QString extractedText = m_textExtractor->extractTextFromImage(m_selectedImagePath, selectedAreaRelative);
    QString extractedText = m_textExtractor->extractTextFromImage(*image, selectedAreaRelative);
    AlertHelper::showMessageAlertWithText(extractedText);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_textExtractor;
    if (image != Q_NULLPTR) {
        delete image;
    }
}
