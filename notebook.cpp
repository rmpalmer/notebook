#include <QtGui>
#include "notebook.h"
#include "ui_notebook.h"

Notebook::Notebook(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    titleLine->setReadOnly(true);
    contentText->setReadOnly(true);
    cancelButton->hide();
    submitButton->hide();
    connect(addButton, SIGNAL(clicked()), this, SLOT(addNote()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitNote()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

}

void Notebook::addNote()
{
    oldTitle = titleLine->text();
    oldContent = contentText->toPlainText();

    titleLine->clear();
    contentText->clear();

    titleLine->setReadOnly(false);
    titleLine->setFocus(Qt::OtherFocusReason);
    contentText->setReadOnly(false);

    addButton->setEnabled(false);
    submitButton->show();
    cancelButton->show();
}

void Notebook::submitNote()
{
    QString title = titleLine->text();
    QString newContent = contentText->toPlainText();

    if (title.isEmpty() || newContent.isEmpty()) {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a title and content"));
        return;
    }

    if (!content.contains(title)) {
        content.insert(title, newContent);
        QMessageBox::information(this, tr("Add Successful"),tr("\"%1\" has been added to your notebook.").arg(title));
    } else {
        QMessageBox::information(this, tr("Add Unsuccessful"),tr("\%1\" is already in your notebook.").arg(title));
    }

    if (content.isEmpty()) {
       titleLine->clear();
       contentText->clear();
    }
    titleLine->setReadOnly(true);
    contentText->setReadOnly(true);
    addButton->setEnabled(true);
    submitButton->hide();
    cancelButton->hide();
}

void Notebook::cancel()
{
    titleLine->setText(oldTitle);
    titleLine->setReadOnly(true);

    contentText->setText(oldContent);
    contentText->setReadOnly(true);

    addButton->setEnabled(true);
    submitButton->hide();
    cancelButton->hide();

}
