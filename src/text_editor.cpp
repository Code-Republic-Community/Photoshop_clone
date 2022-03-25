#include "text_editor.h"

#include <QColor>
#include <QColorDialog>
#include <QFont>
#include <QFontDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>

TextEditor::TextEditor(QWidget *parent)
    : _widget {new QWidget(parent)}
{

    _is_opened = false;
    _create_default_canvas();    
    _create_buttons();
    _create_design();
    _conections();
}

void TextEditor::_create_default_canvas()
{
    _text_edit = new QTextEdit();
    _text_edit->installEventFilter(this);
    QFont font;
    font.setPointSize(35);
    _text_edit->setFont(font);
    _text_edit->setTextColor(Qt::black);
    _widget->setWindowModality(Qt::ApplicationModal);
    _widget->setWindowTitle("Edit Text Layer");
    _widget->show();
}

void TextEditor::_create_buttons()
{
    _fonts = new QPushButton(tr("&Fonts"));
    _color_pallete = new QPushButton(tr("&Color"));
    _cancel = new QPushButton(tr("&Cancel"));
    _ok = new QPushButton(tr("&Ok"));
}

void TextEditor::_create_design()
{
    _vertical_layout = new QVBoxLayout;
    _horizonal_layout = new QHBoxLayout;
    _horizonal_spacer = new QSpacerItem(50, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    _horizonal_layout->addWidget(_fonts);
    _horizonal_layout->addWidget(_color_pallete);
    _horizonal_layout->addSpacerItem(_horizonal_spacer);
    _horizonal_layout->addWidget(_cancel);
    _horizonal_layout->addWidget(_ok);
    _vertical_layout->addWidget(_text_edit);
    _vertical_layout->addLayout(_horizonal_layout);
    _widget->setLayout(_vertical_layout);
}

void TextEditor::_conections()
{
    connect(_color_pallete, &QPushButton::clicked, this, &TextEditor::action_color_pallette);
    connect(_fonts, &QPushButton::clicked, this, &TextEditor::action_fonts);
    connect(_cancel, &QPushButton::clicked, this, &TextEditor::action_cancel);
    connect(_ok, &QPushButton::clicked, this, &TextEditor::action_ok);
}

/* Event filter for Keys */
bool TextEditor::eventFilter(QObject *object, QEvent *event)
{
    if(object == _text_edit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
        if(key_event->modifiers() & Qt::ShiftModifier)
        {
            if(key_event->key() == Qt::Key_Return)
            {
                action_ok();
                return true;
            }
            else { return false; }
        }
        if(key_event->key() == Qt::Key_Escape)
        {
            action_cancel();
            return true;
        }
    }
    return false;
}

/* Slots */
void TextEditor::action_fonts()
{
    bool ok;
    QFont new_font = QFontDialog::getFont(&ok, QFont("Arial", 35), this, "Fonts", QFontDialog::DontUseNativeDialog);
    if(ok)
    {
       _text_edit->setFont(new_font);
    }
}

void TextEditor::action_color_pallette()
{
    QColor new_color = QColorDialog::getColor();
    if(new_color.isValid())
    {
        _text_edit->setTextColor(new_color);
    }
}
void TextEditor::action_cancel()
{
    delete _widget;
    _widget = nullptr;
}

void TextEditor::action_ok()
{
    emit send_text();
    _widget->close();
}


QTextEdit* TextEditor::get_text() const
{
    return _text_edit;
}

/* Memory deallocation */
void TextEditor::_deallocation()
{
    delete _horizonal_layout;
    delete _horizonal_spacer;
    delete _vertical_layout;
    delete _ok;
    delete _cancel;
    delete _color_pallete;
    delete _fonts;
    delete _text_edit;
}

TextEditor::~TextEditor()
{
    if(_is_opened) { _deallocation(); }
    if(_widget) { delete _widget; }
}









