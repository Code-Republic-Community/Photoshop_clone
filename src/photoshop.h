#ifndef PHOTOSHOP_H
#define PHOTOSHOP_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QVector>
#include <QList>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QByteArray>
#include <QFileInfo>
#include <QPixmap>

#include <scribblearea.h>
#include <text_editor.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Photoshop; }
QT_END_NAMESPACE

class Photoshop : public QMainWindow
{
    Q_OBJECT

public:
    Photoshop(QWidget *parent = nullptr);
    ~Photoshop();
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
/* File slots */
    void action_new();
    void action_open();
    void action_raw_camera();
    void action_save();
    void action_save_as(); /* like a helper function */
    void action_print();
    void action_close();
/* Edit slots */
    void action_undo();
    void action_redo();
    void action_cut();
    void action_copy();
    void action_paste();
    void action_clear_screen();
    void action_keyboard_shortcuts();
/* Image slots */
    void action_image_size();
    void action_canvas_size();
    void action_image_left_rotation();
    void action_image_right_rotation();
    void action_crop();
/* Paint slots */
    void action_paint();
    void action_pen_color();
    void action_pen_width();
/* Filter slots */
    void action_blur();
    void action_noise();
    void action_distort();
    void action_pixelate();
/* Help slots */
    void action_Photoshop_help();
    void action_system_info();
/* Tools */
    void _create_brush_tool_optional();
    void pushed_button_erase_tool();
    void pushed_button_type_tool();
    void receive();

private:
/* Menu names,
 * Menu bar,
 * File, Edit, Image, Filter, Help
 * fuctionalities
 */
    void _menu_names();
    void _create_menu_bar();
    void _create_menu_File();
    void _create_menu_Edit();
    void _create_menu_Image();
    void _create_menu_Filter();
    void _create_menu_Help();
    void _create_shortcuts();
    void _create_menu_connections();
    void _create_tool_tip_names();
    void _create_tools();
    void _create_tools_connections();
    void _upload_icons();
    void _merge_layouts();
/* Memory deallocation functionality */
    void _delete_menues();
    void _delete_file_actions();
    void _delete_save_as_action();
    void _delete_tools();
/* Scribble functionality */
    bool save_file(const QByteArray &file_format);
    bool maybe_save();

/* OpenCV functionality */
private:
    using menubar_actions_trigged = void(Photoshop::*)();
    const int BUTTONS_COUNT = 9;
    const int CAMERA_ID = 0;
    Ui::Photoshop *ui;
/* Optional(like menubar and etc.) members */
    int _meta_info[3];
    QVector<menubar_actions_trigged> _functions;
    QVector<QString> _tools_icons;
    QVector<QString> _tool_tips;
    QVector<QMenu*> _menubar;
    QVector<QString> _names;
    QVector<QAction*> _actions;
    QVBoxLayout *_left_menu;
    QHBoxLayout *_horizonal;
    QVector<QPushButton*> _tools;
    QSpacerItem *_verticalSpacer;
/* Members for scribble area */
    ScribbleArea *_scribbleArea;
    cv::Mat _image;
    QAction *_print_act;
    QMenu *_save_as_menu;
    QAction *_open_action;
    QList<QAction*> _save_as_action;
    QFileInfo *_ext_name;
/* Tools and buttons */
    QMenu *_brush_tool_menu;
    QAction *_pen_color_act;
    QAction *_pen_width_act;
    QAction *_paint;
    QDialog *_dialog;
    TextEditor *_editor;
    QTextEdit *_text;
    QLabel *_text_label;
/* Flags */
    bool _is_action_open;
    bool _is_merge_layouts;

};
#endif /* PHOTOSHOP_H */
