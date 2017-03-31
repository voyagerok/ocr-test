#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <leptonica/allheaders.h>
#include <QImage>
#include <QString>

struct ImageData;

struct ImageProcessorException {
    const QString message;
};

class ImageProcessor
{
public:
    ImageProcessor(const QString &imagePath);
    ImageProcessor(QImage &qtImage);
    Pix *performGrayscaleTransform();
    Pix *performBinarizationOtsu();
    Pix *getPixImage() const { return image; }
    ~ImageProcessor();
private:
    Pix *image = NULL;
    ImageData *qtImageData = NULL;
};

#endif // IMAGEPROCESSOR_H
