#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QTextEdit>
#include <QImage>

#include <opencv2/imgproc.hpp>


class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);
    ~ScribbleArea();

    bool open_image(cv::Mat *image);
    bool save_image(const QString &file_name, const char *file_format);
    void set_pen_color(const QColor &new_color);
    void set_pen_width(int new_width);
    void set_text(QTextEdit *text);
    bool is_modified() const { return _modified; }
    QColor pen_color() const { return _my_pen_color; }
    int pen_width() const { return _my_pen_width; }

public slots:
    void clear_image();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    void draw_to_line(const QPoint &end_point);
    void resize_image(const QSize &new_size);
    QImage cvMatToQImage(const cv::Mat &inMat);
    cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData = true);

private:
    QWidget *_widget;
    bool _modified;
    bool _scribbling;
    QColor _my_pen_color;
    QPoint _last_point;
    int _my_pen_width;
    QImage _image;
    cv::Mat *_cv_image;
public:
    static bool draw_access;
};

#endif // SCRIBBLEAREA_H
