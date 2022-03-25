#include <QtWidgets>

#include "scribblearea.h"


ScribbleArea::ScribbleArea(QWidget *parent) : _widget{new QWidget(parent)}
{
    _modified = false;
    _scribbling = false;
    _my_pen_width = 1;
    _my_pen_color = Qt::black;
}

ScribbleArea::~ScribbleArea()
{
    delete _widget;
}

bool ScribbleArea::open_image(cv::Mat *image)
{    
    if(!image->data)
    {
        return false;
    }

    cv::resize(*image, _cv_image, cv::Size(width(), height()));
    _image = cvMatToQImage(*image);
    update();
    _modified = true;
    return true;
}

void ScribbleArea::set_text(QTextEdit *text)
{
    QPainter painter(&_image);
    painter.setPen(QPen(text->textColor()));
    painter.setFont(text->font());
    painter.drawText(_image.rect(), Qt::AlignCenter, text->toPlainText());
    _cv_image = QImageToCvMat(_image);
    _modified = true;
}

bool ScribbleArea::save_image(const QString &file_name, const char *file_format)
{
    QImage visible_image = _image;
    resize_image(size());
    if(visible_image.save(file_name, file_format))
    {
        _modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

void ScribbleArea::set_pen_color(const QColor &new_color)
{
    _my_pen_color = new_color;
}

void ScribbleArea::set_pen_width(int new_width)
{
    _my_pen_width = new_width;
}
void ScribbleArea::clear_image()
{
    _image.fill(qRgb(255, 255, 255));
    _modified = true;
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        _last_point = event->pos();
        _scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && _scribbling)
    {
        draw_to_line(event->pos());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() & Qt::LeftButton) && _scribbling)
    {
        draw_to_line(event->pos());
        _scribbling = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirty_rect = event->rect();
    painter.drawImage(dirty_rect, _image, dirty_rect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if(!_cv_image.data)
    {
        _cv_image = QImageToCvMat(_image);
        // if image exists, resizeing fails "TBD".
    }
    if(event->size() != _image.size())
    {
        resize_image(event->size());
    }
    update();
    QWidget::resizeEvent(event);
}

void ScribbleArea::draw_to_line(const QPoint &end_point)
{
    QPainter painter(&_image);
    painter.setPen(QPen(_my_pen_color, _my_pen_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(_last_point, end_point);
    _modified = true;
    int rad = (_my_pen_width / 2) + 2;
    update(QRect(_last_point, end_point).normalized().adjusted(-rad, -rad, +rad, +rad));
    _last_point = end_point;
}

void ScribbleArea::resize_image(const QSize &new_size)
{
    int c_width = _cv_image.rows;
    int c_height = _cv_image.cols;
    if(c_width == new_size.width() && c_height == new_size.height()) { return; }

    int n_width = new_size.width() + 1;
    int n_height = new_size.height() + 1;
    cv::Mat resized_image;
    QImage new_image(new_size, QImage::Format_ARGB32);
    new_image.fill(qRgb(255, 255, 255));
    if(_cv_image.data)
    {
        cv::resize(_cv_image, resized_image, cv::Size(n_width, n_height));
        new_image = cvMatToQImage(resized_image);
    }
    _image = new_image;
}

/* Convert from cvMat to QImage and and vice versa */
QImage ScribbleArea::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type())
    {
        case CV_8UC4:
        {
            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_ARGB32 );

            return image;
         }
         case CV_8UC3:
         {
            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         case CV_8UC1:
         {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_Grayscale8 );
#else
            static QVector<QRgb>  sColorTable;

            // only create our color table the first time
            if ( sColorTable.isEmpty() )
            {
               sColorTable.resize( 256 );

               for ( int i = 0; i < 256; ++i )
               {
                  sColorTable[i] = qRgb( i, i, i );
               }
            }

            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );
#endif

            return image;
         }

         default:
            break;
      }

      return QImage();
   }

cv::Mat ScribbleArea::QImageToCvMat(const QImage &inImage, bool inCloneImageData)
{
    switch ( inImage.format() )
    {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        {
        cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC4,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

         return (inCloneImageData ? mat.clone() : mat);
         }

         // 8-bit, 3 channel
         case QImage::Format_RGB32:
         {
            if ( !inCloneImageData )
            {
               qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
            }

            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC4,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            cv::Mat  matNoAlpha;

            cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

            return matNoAlpha;
         }

         // 8-bit, 3 channel
         case QImage::Format_RGB888:
         {
            if ( !inCloneImageData )
            {
               qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
            }

            QImage   swapped = inImage.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(),
                            CV_8UC3,
                            const_cast<uchar*>(swapped.bits()),
                            static_cast<size_t>(swapped.bytesPerLine())
                            ).clone();
         }

         // 8-bit, 1 channel
         case QImage::Format_Indexed8:
         {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC1,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
         }
        default:
            //qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
      }
    return cv::Mat();
}



