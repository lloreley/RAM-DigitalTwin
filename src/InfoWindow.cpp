#include "InfoWindow.h"
#include <QVBoxLayout>
#include <QLabel>

InfoWindow::InfoWindow(const QString &title, const QString &content, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(title);
    setModal(false);
    resize(600, 400);

    auto *layout = new QVBoxLayout(this);

    contentView = new QListWidget();
    contentView->setStyleSheet("QListWidget { font-family: Arial; font-size: 10pt; }");

    for (const QString &line : content.split("\n")) {
        contentView->addItem(line);
    }

    layout->addWidget(contentView);
    setLayout(layout);
}
