#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "ui_finddialog.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog, public Ui::FindDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

    QString getFindText();

public slots:
    void findClicked();

private:
    QString findText;
};

#endif // FINDDIALOG_H
