#ifndef ATTACHMENTCHOOSER_H
#define ATTACHMENTCHOOSER_H

#include <QWidget>

namespace Ui {
class AttachmentChooser;
}

class AttachmentChooser : public QWidget
{
    Q_OBJECT

public:
    explicit AttachmentChooser(QWidget *parent = 0);
    ~AttachmentChooser();

private:
    Ui::AttachmentChooser *ui;
};

#endif // ATTACHMENTCHOOSER_H
