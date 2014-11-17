#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QWidget>
#include <QMap>
#include <QString>

#include "ui_notebook.h"

class Notebook : public QWidget, public Ui::Notebook
{
    Q_OBJECT

public:
    explicit Notebook(QWidget *parent = 0);

public slots:
    void addNote();
    void submitNote();
    void cancel();

private:
    QMap<QString,QString> content;
    QString oldTitle;
    QString oldContent;

};

#endif // NOTEBOOK_H
