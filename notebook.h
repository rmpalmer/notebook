#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QWidget>
#include <QMap>
#include <QString>

#include "ui_notebook.h"
#include "finddialog.h"

class Notebook : public QWidget, public Ui::Notebook
{
    Q_OBJECT

public:
    explicit Notebook(QWidget *parent = 0);
    enum Mode {NavigationMode, AddingMode, EditingMode};

public slots:
    void addNote();
    void submitNote();
    void cancel();
    void next();
    void previous();
    void editContent();
    void removeContent();
    void findTitle();
    void saveToFile();
    void loadFromFile();

private:
    void updateInterface(Mode mode);

    Mode currentMode;
    QMap<QString,QString> content;
    QString oldTitle;
    QString oldContent;
    FindDialog *dialog;

};

#endif // NOTEBOOK_H
