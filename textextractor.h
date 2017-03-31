#ifndef TEXTEXTRACTOR_H
#define TEXTEXTRACTOR_H

#include <tesseract/baseapi.h>
#include <QString>
#include <QRect>
#include <QImage>
#include <memory>

struct TextExtractorException {
    QString message;
    TextExtractorException(const QString &message): message{message} {}
};

class TextExtractor
{
public:
    enum class TextLanguage {English, Russian};

    TextExtractor(TextLanguage lang);
    ~TextExtractor();
    QString extractTextFromImage(const QString &imagePath, QRect area);
    QString extractTextFromImage(QImage &image, QRect area);
private:
    std::unique_ptr<tesseract::TessBaseAPI> m_baseApi;
};

#endif // TEXTEXTRACTOR_H
