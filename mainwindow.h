#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imagearea.h>
#include <QBoxLayout>
#include "textextractor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile();
    void extractText();

private:
    Ui::MainWindow *ui;
    QImage *image = Q_NULLPTR;
    ImageArea *m_imageArea;
    QBoxLayout *m_scrollAreaLayout;
    TextExtractor *m_textExtractor = Q_NULLPTR;
};

#endif // MAINWINDOW_H
