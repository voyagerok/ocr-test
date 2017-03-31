#include <memory>
#import <QDebug>
#include "imageprocessor.h"

struct ImageData {
    uchar *imageData;
    int width;
    int height;
    int bytesPerLine;
    int depth;
};

static ImageData extractImageDataFromQImage(QImage &image) {
    uchar *imageData = image.bits();
    int width = image.width();
    int height = image.height();
    int bytesPerLine = image.bytesPerLine();
    int depth = image.depth();
    return {imageData, width, height, bytesPerLine, depth};
}

static void freeExtractedImageData(ImageData imageData) {
    if (imageData.imageData) {
        delete imageData.imageData;
    }
}

ImageProcessor::ImageProcessor(const QString &imagePath)
{
    image = pixRead(imagePath.toLocal8Bit().data());
}

ImageProcessor::ImageProcessor(QImage &qtImage) {
    auto imageDataStruct = extractImageDataFromQImage(qtImage);
    image = pixCreateHeader(imageDataStruct.width, imageDataStruct.height, imageDataStruct.depth);
    if (image == NULL) {
        freeExtractedImageData(imageDataStruct);
        throw ImageProcessorException {"Failed to create pix image"};
    }
    pixSetData(image, reinterpret_cast<l_uint32*>(imageDataStruct.imageData));
}

Pix *ImageProcessor::performBinarizationOtsu() {
    int tileHoriz = pixGetWidth(image) / 2;
    int tileVert = pixGetHeight(image) / 2;
    Pix *gray = performGrayscaleTransform();
    Pix *binarized = NULL;
    auto result = pixOtsuAdaptiveThreshold(gray, tileHoriz, tileVert, 0, 0, 0.0, NULL, &binarized);
    pixDestroy(&gray);
    if (result != 0) {
        qDebug() << "Otsu binarization failed" << endl;
        return NULL;
    }
    if (binarized == NULL) {
        qDebug() << "Binarized image is NULL" << endl;
        return NULL;
    }

    return binarized;
}

Pix *ImageProcessor::performGrayscaleTransform() {
    return pixConvertRGBToGray(image, 0, 0, 0);
}

ImageProcessor::~ImageProcessor() {
    pixDestroy(&image);
}
