#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QDebug>
#include "textextractor.h"
#include "imageprocessor.h"

struct ImageData {
    uchar *imageData;
    int width;
    int height;
    int bytesPerLine;
    int bytesPerPixel;
};

static ImageData extractImageDataFromQImage(QImage &image) {
    uchar *imageData = image.bits();
    int width = image.width();
    int height = image.height();
    int bytesPerLine = image.bytesPerLine();
    int bytesPerPixel = image.depth() / 8;
    return {imageData, width, height, bytesPerLine, bytesPerPixel};
}

TextExtractor::TextExtractor(TextExtractor::TextLanguage language) : m_baseApi(new tesseract::TessBaseAPI()) {

    const char *lang;
    switch (language) {
    case TextLanguage::Russian:
        lang = "rus";
        break;
    case TextLanguage::English:
        lang = "eng";
        break;
    }
    auto result = m_baseApi->Init(NULL, lang);
    if (result == -1) {
        throw TextExtractorException{"Failed to initialize tesseract"};
    }
}

TextExtractor::~TextExtractor() {
    m_baseApi->End();
}

QString TextExtractor::extractTextFromImage(const QString &imagePath, QRect area = QRect()) {
    Pix *image = pixRead(imagePath.toLocal8Bit().data());
    m_baseApi->SetImage(image);
    if (!area.isEmpty() && !area.isNull()) {
        m_baseApi->SetRectangle(area.left(), area.top(), area.width(), area.height());
    }
    char *extractedText = m_baseApi->GetUTF8Text();
    QString extractionResult = QString{extractedText};

    delete [] extractedText;
    pixDestroy(&image);

    return extractionResult;
}

QString TextExtractor::extractTextFromImage(QImage &image, QRect area) {
    auto imageDataStruct = extractImageDataFromQImage(image);
    m_baseApi->SetImage(imageDataStruct.imageData,
                        imageDataStruct.width,
                        imageDataStruct.height,
                        imageDataStruct.bytesPerPixel,
                        imageDataStruct.bytesPerLine);
//    Pix *inputImage = NULL;
//    try {
//        auto imageProcessor = ImageProcessor(image);
////        Pix *grayScale = imageProcessor.performGrayscaleTransform();
//        inputImage = imageProcessor.performBinarizationOtsu();
//        m_baseApi->SetImage(inputImage);
//    } catch (ImageProcessorException &exc) {
//        qDebug() << "Failed to init image processor: " << exc.message << endl;
//        return QString();
//    }
    if (!area.isEmpty() && !area.isNull()) {
        m_baseApi->SetRectangle(area.left(), area.top(), area.width(), area.height());
    }
    char *extractedText = m_baseApi->GetUTF8Text();
    QString extractionResult = QString{extractedText};
    delete [] extractedText;

//    pixDestroy(&inputImage);

    return extractionResult;
}
