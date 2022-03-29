#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QEvent>

class TextEditor : public QWidget
{
   Q_OBJECT

public:
    TextEditor(QWidget *parent = 0);
     QTextEdit* get_text() const;
    ~TextEditor();

protected:
     bool eventFilter(QObject *object, QEvent *event) override;

private slots:
    void action_fonts();
    void action_color_pallette();
    void action_cancel();
    void action_ok();

signals:
    void send_text();

private:
    void _create_default_canvas();
    void _create_buttons();
    void _create_design();
    void _deallocation();
    void _conections();

private:
    bool _is_opened;
    QWidget *_widget;
    QTextEdit *_text_edit;
    QPushButton *_fonts;
    QPushButton *_color_pallete;
    QPushButton *_cancel;
    QPushButton *_ok;
    QVBoxLayout *_vertical_layout;
    QHBoxLayout *_horizonal_layout;
    QSpacerItem *_horizonal_spacer;
};

#endif // TEXT_EDITOR_H
