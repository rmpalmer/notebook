#include <QtGui>
#include "notebook.h"
#include "ui_notebook.h"
#include <iostream>

Notebook::Notebook(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    titleLine->setReadOnly(true);
    contentText->setReadOnly(true);
    cancelButton->hide();
    submitButton->hide();
    nextButton->setEnabled(false);
    previousButton->setEnabled(false);
    editButton->setEnabled(false);
    removeButton->setEnabled(false);

    dialog = new FindDialog;

    connect(addButton, SIGNAL(clicked()), this, SLOT(addNote()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitNote()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editContent()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeContent()));
    connect(findButton, SIGNAL(clicked()), this, SLOT(findTitle()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
}

void Notebook::addNote()
{
    oldTitle = titleLine->text();
    oldContent = contentText->toPlainText();

    titleLine->clear();
    contentText->clear();

    updateInterface(AddingMode);
}

void Notebook::submitNote()
{
    QString title = titleLine->text();
    QString newContent = contentText->toPlainText();

    if (title.isEmpty() || newContent.isEmpty()) {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a title and content."));
        return;
    }
    if (currentMode == AddingMode) {

        if (!content.contains(title)) {
            content.insert(title, newContent);
            QMessageBox::information(this, tr("Add Successful"),
                tr("\"%1\" has been added to your notebook.").arg(title));
        } else {
            QMessageBox::information(this, tr("Add Unsuccessful"),
                tr("Sorry, \"%1\" is already in your notebook.").arg(title));
        }
    } else if (currentMode == EditingMode) {

        if (oldTitle != title) {
            if (!content.contains(title)) {
                QMessageBox::information(this, tr("Edit Successful"),
                    tr("\"%1\" has been edited in your notebook.").arg(oldTitle));
                content.remove(oldTitle);
                content.insert(title, newContent);
            } else {
                QMessageBox::information(this, tr("Edit Unsuccessful"),
                    tr("Sorry, \"%1\" is already in your notebook.").arg(title));
            }
        } else if (oldContent != newContent) {
            QMessageBox::information(this, tr("Edit Successful"),
                tr("\"%1\" has been edited in your notebook.").arg(title));
            content[title] = newContent;
        }
    }

    updateInterface(NavigationMode);

}

void Notebook::cancel()
{
    titleLine->setText(oldTitle);
    contentText->setText(oldContent);
    updateInterface(NavigationMode);
}

void Notebook::next()
{
    QString title = titleLine->text();
    QMap<QString,QString>::iterator i = content.find(title);
    if (i != content.end())
        i++;
    if (i == content.end())
        i = content.begin();
    titleLine->setText(i.key());
    contentText->setText(i.value());
}

void Notebook::previous()
{
    QString title = titleLine->text();
    QMap<QString,QString>::iterator i = content.find(title);
    if (i == content.end()) {
        titleLine->clear();
        contentText->clear();
        return;
    }

    if (i == content.begin())
        i = content.end();

    i--;
    titleLine->setText(i.key());
    contentText->setText(i.value());
}

void Notebook::editContent()
{
    oldTitle = titleLine->text();
    oldContent = contentText->toPlainText();

    updateInterface(EditingMode);
}

void Notebook::removeContent()
{
    QString title = titleLine->text();
    QString theContent = contentText->toPlainText();

    if (content.contains(title)) {
        int button = QMessageBox::question(this,
                       tr("Confirm Remove"),
                       tr("Are you Sure you want to remove \"%1\"?").arg(title),
                       QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes) {
            previous();
            content.remove(title);
            QMessageBox::information(this, tr("Remove Successful"),
                       tr("\"%1\" has been removed from your notebook.").arg(title));
        }

    }
    updateInterface(NavigationMode);

}

void Notebook::updateInterface(Mode mode)
{
    currentMode = mode;

    switch (currentMode) {
    case AddingMode:
    case EditingMode:
        titleLine->setReadOnly(false);
        titleLine->setFocus(Qt::OtherFocusReason);
        contentText->setReadOnly(false);

        addButton->setEnabled(false);
        editButton->setEnabled(false);
        removeButton->setEnabled(false);

        nextButton->setEnabled(false);
        previousButton->setEnabled(false);

        submitButton->show();
        cancelButton->show();
        break;
    case NavigationMode:
        if (content.isEmpty()) {
            titleLine->clear();
            contentText->clear();
        }
        titleLine->setReadOnly(true);
        contentText->setReadOnly(true);
        addButton->setEnabled(true);

        int number = content.size();
        editButton->setEnabled(number >= 1);
        removeButton->setEnabled(number >= 1);
        nextButton->setEnabled(number > 1);
        previousButton->setEnabled(number > 1);

        submitButton->hide();
        cancelButton->hide();
        break;
    }
}

void Notebook::findTitle()
{
    dialog->show();

    if (dialog->exec() == QDialog::Accepted) {
        QString titleName = dialog->getFindText();

        if (content.contains(titleName)) {
            titleLine->setText(titleName);
            contentText->setText(content.value(titleName));
        } else {
            QMessageBox::information(this, tr("Title Nof found"),
              tr("Sorry, \%1\" is not in your notebook.").arg(titleName));
            return;
        }
    }
    updateInterface(NavigationMode);
}

void Notebook::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
          tr("Save Notebook"), "",
          tr("Notebook (*.nbk);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this,tr("Unable to open file"),
               file.errorString());
            return;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_8);
        out << content;
    }
}

void Notebook::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
       tr("Open Notebook"), "",
       tr("Notebook (*.nbk);;All Files(*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this,tr("Unable to open file"),
               file.errorString());
            return;
        }

        QDataStream in (&file);
        in.setVersion(QDataStream::Qt_4_8);
        content.empty();
        in >> content;

        if (content.isEmpty()) {
            QMessageBox::information(this, tr("No content to save"),
                                     tr("The file has no content"));
        } else {
            QMap<QString,QString>::iterator i = content.begin();
            titleLine->setText(i.key());
            contentText->setText(i.value());
        }
    }
    updateInterface(NavigationMode);

}





















