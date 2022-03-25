#include "photoshop.h"
#include "ui_photoshop.h"

#include <QFileDialog>
#include <QColorDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QImageReader>
#include <QDebug>
#include <QSizeGrip>
#include <QCursor>


Photoshop::Photoshop(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Photoshop)
{
    ui->setupUi(this);
    _scribbleArea = new ScribbleArea;
    _is_action_open = false;
    _create_menu_bar();
    _create_shortcuts();
    _create_tool_tip_names();
    _create_tools_connections();
    _merge_layouts();
}

void Photoshop::_menu_names()
{   /* Files */
    _names.push_back("&New...");
    _names.push_back("&Open...");
    _names.push_back("&Raw Camera");
    _names.push_back("&Save");
    _names.push_back("Clear Screen");
    _names.push_back("&Print");
    _names.push_back("&Close");
    /* Edit */
    _names.push_back("&Undo");
    _names.push_back("&Step Forward");
    _names.push_back("&Step Backward");
    _names.push_back("&Cut");
    _names.push_back("&Copy");
    _names.push_back("&Past");
    _names.push_back("&Keyboard shortcuts");
    /* Image */
    _names.push_back("&Image Size");
    _names.push_back("&Canvas Size");
    _names.push_back("&Image Rotation");
    _names.push_back("&Crop");
    /* Filter */
    _names.push_back("&Blur");
    _names.push_back("&Noise");
    _names.push_back("&Distort");
    _names.push_back("&Pixelate");
    /* Help */
    _names.push_back("&Photoshop help");
    _names.push_back("&System info");
}

void Photoshop::_create_tool_tip_names()
{
    _tool_tips.push_back("Brush tool");
    _tool_tips.push_back("Move tool");
    _tool_tips.push_back("Marquee tool");
    _tool_tips.push_back("Lasso tool");
    _tool_tips.push_back("Crop tool");
    _tool_tips.push_back("Eyedropper tool");
    _tool_tips.push_back("Eraser tool");
    _tool_tips.push_back("Type tool");
    _tool_tips.push_back("Converting image format");
    _upload_icons();
    _create_tools();
}

/* main function for MenuBar */
void Photoshop::_create_menu_bar()
{
    _menu_names();
    _create_menu_File();
    _create_menu_connections();
    _create_menu_Edit();
    _create_menu_connections();
    _create_menu_Image();
    _create_menu_connections();
    _create_menu_Filter();
    _create_menu_connections();
    _create_menu_Help();
    _create_menu_connections();
    _create_actions_optional();
}
/* File slots */
void Photoshop::action_new()
{
   if(maybe_save())
   {
      _scribbleArea->clear_image();
   }
}

void Photoshop::action_open()
{
    QString file_name = "";
    if(maybe_save())
    {
      file_name = QFileDialog::getOpenFileName(this,
              tr("Open File"), QDir::homePath());
      _image = cv::imread(file_name.toUtf8().constData());
      if(!file_name.isEmpty())
       {
           _scribbleArea->open_image(&_image);
       }
    }
    _ext_name = new QFileInfo(file_name);
    _is_action_open = true;
}

void Photoshop::action_raw_camera()
{
    const std::string file_name{"Capture"};
    cv::VideoCapture _cap(CAMERA_ID);
    cv::Mat tmp_img;
    const int width = 400;
    const int height = 300;
    _cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    _cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    while(1)
    {
        _cap >> tmp_img;
        cv::imshow(file_name, tmp_img);
        if(13 == cv::waitKey(1) % 255)
        {
            cv::imwrite(file_name + ".png", tmp_img);
            _cap.release();
            cv::destroyWindow(file_name);
            break;
        }
        if(32 == cv::waitKey(1) % 255)
        {
            _cap.release();
            cv::destroyWindow(file_name);
            return;
        }
    }
    cv::resize(tmp_img, _image, cv::Size(800, 600));
    _scribbleArea->open_image(&_image);
}

void Photoshop::action_save()
{
   if(!_is_action_open) { save_file("png"); }
   else
   {
       QByteArray name;
       name += _ext_name->completeSuffix();
       save_file(name);
   }
}

void Photoshop::action_save_as()
{
    QAction *action = qobject_cast<QAction*>(sender());
    QByteArray file_format = action->data().toByteArray();
    save_file(file_format);
}

void Photoshop::action_clear_screen()
{
    _scribbleArea->clear_image();
}

void Photoshop::action_print()
{
    // in progress
}
void Photoshop::action_close()
{
    if(maybe_save())
    {
        QApplication::quit();
    }
}
/* Edit slots */
void Photoshop::action_undo()
{
    QMessageBox::information(this, "", "undo");
}
void Photoshop::action_step_forward()
{
    QMessageBox::information(this, "", "sForward");

}
void Photoshop::action_step_backward()
{
    QMessageBox::information(this, "", "sBackward");
}
void Photoshop::action_cut()
{
    QMessageBox::information(this, "", "cut");

}
void Photoshop::action_copy()
{
    QMessageBox::information(this, "", "copy");

}
void Photoshop::action_past()
{
    QMessageBox::information(this, "", "past");
}
void Photoshop::action_keyboard_shortcuts()
{
    QMessageBox::information(this, "", "keyboard_shortcuts");
}
/* Image slots */
void Photoshop::action_image_size()
{
    QMessageBox::information(this, "", "image_size");
}
void Photoshop::action_canvas_size()
{
     QMessageBox::information(this, "", "canvas_size");
}
void Photoshop::action_image_rotation()
{
     QMessageBox::information(this, "", "image_rotation");
}
void Photoshop::action_crop()
{
     QMessageBox::information(this, "", "crop");
}
/* Filter slots */
void Photoshop::action_blur()
{
    QMessageBox::information(this, "", "Blur");
}
void Photoshop::action_noise()
{
    QMessageBox::information(this, "", "Noise");

}
void Photoshop::action_distort()
{
    QMessageBox::information(this, "", "Distort");

}
void Photoshop::action_pixelate()
{
    QMessageBox::information(this, "", "Pixelate");

}
/* Help slots */
void Photoshop::action_Photoshop_help()
{
    QMessageBox::information(this, "", "Photoshop help");
}
void Photoshop::action_system_info()
{
    QMessageBox::information(this, tr("My Photoshop")
                             , tr("<p> The <b> Photoshop </b> example is awesome</p>"));
}

/* Drawing slots */

void Photoshop::closeEvent(QCloseEvent *event)
{
    if(maybe_save()) { event->accept(); }
    else { event->ignore(); }
}

void Photoshop::action_pen_color()
{
    QColor new_color = QColorDialog::getColor();
    if(new_color.isValid())
    {
        _scribbleArea->set_pen_color(new_color);
    }
}

void Photoshop::action_pen_width()
{
    bool ok;
    int new_width = QInputDialog::getInt(this, tr("")
                                         , tr("Select pen width: ")
                                         , _scribbleArea->pen_width()
                                         , 1, 50, 1, &ok);
    if(ok)
    {
        _scribbleArea->set_pen_width(new_width);
    }
}

/* Actions */

void Photoshop::_create_menu_connections()
{
    for(int i = _meta_info[0]; i < _meta_info[1]; ++i)
    {
        if(4 == i) { _menubar[0]->addMenu(_save_as_menu); }
        _actions.push_back(new QAction(_names[i]));
        _menubar.at(_meta_info[2])->addAction(_actions[i]);
        connect(_actions[i], &QAction::triggered, this, _functions[i]);
    }
}

void Photoshop::_create_actions_optional()
{
    _open_action = new QAction(tr("&Open"), this);
    connect(_open_action, &QAction::triggered, this, &Photoshop::action_open);

    _pen_color_act = new QAction(tr("&Pen Color..."), this);
    connect(_pen_color_act, &QAction::triggered, this, &Photoshop::action_pen_color);

    _pen_width_act = new QAction(tr("&Pen Width"), this);
    connect(_pen_width_act, &QAction::triggered, this, &Photoshop::action_pen_width);

    _paint = new QAction(tr("&Paint"), this);
    connect(_paint, &QAction::triggered, this, &Photoshop::action_paint);

    _brush_tool_menu = new QMenu(tr("&Brush tool"));
    _brush_tool_menu->addAction(_paint);
    _brush_tool_menu->addAction(_pen_color_act);
    _brush_tool_menu->addAction(_pen_width_act);
}

/* Menu */
void Photoshop::_create_menu_File()
{
    _meta_info[2] = _menubar.size();
    _menubar.push_back(menuBar()->addMenu(tr("&File")));
    _meta_info[0] = _functions.size();
    _functions.push_back(&Photoshop::action_new);
    _functions.push_back(&Photoshop::action_open);
    _functions.push_back(&Photoshop::action_raw_camera);
    _functions.push_back(&Photoshop::action_save);
    _functions.push_back(&Photoshop::action_clear_screen);
    _functions.push_back(&Photoshop::action_print);
    _functions.push_back(&Photoshop::action_close);
    _meta_info[1] = _functions.size();

    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = (tr("%1 ...").arg(QString(format).toUpper()));
        QAction *action = new QAction(text, this);
        action->setData(format);
        _save_as_action.append(action);
        connect(action, &QAction::triggered, this, &Photoshop::action_save_as);
    }
    _save_as_menu = new QMenu(tr("Save as..."), this);
    foreach(QAction *tmp_action, _save_as_action)
    {
        _save_as_menu->addAction(tmp_action);
    }
}

void Photoshop::_create_menu_Edit()
{
    _meta_info[2] = _menubar.size();
    _menubar.push_back(menuBar()->addMenu(tr("&Edit")));
    _meta_info[0] = _functions.size();
    _functions.push_back(&Photoshop::action_undo);
    _functions.push_back(&Photoshop::action_step_forward);
    _functions.push_back(&Photoshop::action_step_backward);
    _functions.push_back(&Photoshop::action_cut);
    _functions.push_back(&Photoshop::action_copy);
    _functions.push_back(&Photoshop::action_past);
    _functions.push_back(&Photoshop::action_keyboard_shortcuts);
    _meta_info[1] = _functions.size();
}

void Photoshop::_create_shortcuts()
{
    QString shortcuts_names = {"NORSEPWZ><XCVKLJTYMBGFDI"};
    char i = 0;
    QByteArray arr;
    for(auto &elem : _actions)
    {
        QString str = "Ctrl+"+ shortcuts_names[i];
        arr = (str).toLocal8Bit();
        elem->setShortcut(tr(arr.data()));
        ++i;
    }
}

void Photoshop::_create_menu_Image()
{
    _meta_info[2] = _menubar.size();
    _menubar.push_back(menuBar()->addMenu(tr("&Image")));
    _meta_info[0] = _functions.size();
    _functions.push_back(&Photoshop::action_image_size);
    _functions.push_back(&Photoshop::action_canvas_size);
    _functions.push_back(&Photoshop::action_image_rotation);
    _functions.push_back(&Photoshop::action_crop);
    _meta_info[1] = _functions.size();
}

void Photoshop::_create_menu_Filter()
{
    _meta_info[2] = _menubar.size();
    _menubar.push_back(menuBar()->addMenu(tr("&Filter")));
    _meta_info[0] = _functions.size();
    _functions.push_back(&Photoshop::action_blur);
    _functions.push_back(&Photoshop::action_noise);
    _functions.push_back(&Photoshop::action_distort);
    _functions.push_back(&Photoshop::action_pixelate);
    _meta_info[1] = _functions.size();
}

void Photoshop::_create_menu_Help()
{
    _meta_info[2] = _menubar.size();
    _menubar.push_back(menuBar()->addMenu(tr("&Help")));
    _meta_info[0] = _functions.size();
    _functions.push_back(&Photoshop::action_Photoshop_help);
    _functions.push_back(&Photoshop::action_system_info);
    _meta_info[1] = _functions.size();
}

bool Photoshop::maybe_save()

{
    if(_scribbleArea->is_modified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble")
                                   , tr("The message has been modified.\n"
                                        "Do you want save your changes?")
                                   , QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
        {
            return save_file("png");
        }
        else if(ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool Photoshop::save_file(const QByteArray &file_format)
{
    QString initial_path = QDir::homePath() + "/untitled." + file_format;
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save as")
                           ,initial_path
                           ,tr("%1 Files(*.%2);; All files(*)")
                           .arg(QString::fromLatin1(file_format.toUpper())
                           , QString::fromLatin1(file_format)));
    if(file_name.isEmpty())
    {
        return false;
    }
    else
    {
        return _scribbleArea->save_image(file_name, file_format.constData());
    }
}

void Photoshop::_merge_layouts()
{
    _scribbleArea = new ScribbleArea;
    _horizonal = new QHBoxLayout(ui->centralwidget);
    _horizonal->addLayout(_left_menu);
    _horizonal->addWidget(_scribbleArea);
    _verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Maximum, QSizePolicy::Expanding);
    _left_menu->addSpacerItem(_verticalSpacer);
}

/* Tools & Buttons */
void Photoshop::_upload_icons()
{
    for(int i = 0; i < BUTTONS_COUNT; ++i)
    {
        _tools_icons.push_back(":/new/prefix1/" + _tool_tips.at(i) + ".png");
    }
}

void Photoshop::_create_tools()
{
    _left_menu = new QVBoxLayout();
    for(int i {}; i < BUTTONS_COUNT; ++i)
    {
        _tools.push_back(new QPushButton(this));
        _tools[i]->setToolTip(_tool_tips.at(i));
        _tools[i]->setIcon(QIcon(_tools_icons[i]));
        _tools[i]->setFixedSize(QSize(41, 30));
        _left_menu->addWidget(_tools[i]);
    }
    _tools[0]->setMenu(_brush_tool_menu);
}
void Photoshop::action_paint()
{
    ScribbleArea::draw_access = true;
}

void Photoshop::_create_tools_connections()
{
    connect(_tools[7], &QPushButton::clicked, this, &Photoshop::pushed_brush_button);
}

/* Button slots */
void Photoshop::pushed_brush_button()
{
    _editor = new TextEditor;
    connect(_editor, &TextEditor::send_text, this, &Photoshop::receive);
}

void Photoshop::receive()
{
    _text = _editor->get_text();
    _scribbleArea->set_text(_text);
}

/* Memory deallocation functionality */

void Photoshop::_delete_menues()
{
    for(auto it = _menubar.begin(); it != _menubar.end(); ++it)
    {
        delete *it;
    }
}

void Photoshop::_delete_file_actions()
{
    for(auto it = _actions.begin(); it != _actions.end(); ++it)
    {
        delete *it;
    }
}

void Photoshop::_delete_save_as_action()
{
    for(auto it = _save_as_action.begin(); it != _save_as_action.end(); ++it)
    {
        delete *it;
    }
}

void Photoshop::_delete_tools()
{
    for(auto it = _tools.begin(); it != _tools.end(); ++it)
    {
        delete *it;
    }
}

/* Destructor */
Photoshop::~Photoshop()
{
    _delete_file_actions();
    _delete_save_as_action();
    _delete_menues();
    _delete_tools();


    delete _pen_color_act;
    delete _open_action;
    delete _brush_tool_menu;
    delete _save_as_menu;
    delete _pen_width_act;
    if(_is_action_open) { delete _ext_name; }
//    delete _verticalSpacer;
//    delete _left_menu;
//    delete _horizonal;
    delete _editor;
    delete _scribbleArea;
    delete ui;
}



