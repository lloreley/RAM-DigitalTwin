#pragma once

#include <QDialog>
#include <QListWidget>

class InfoWindow : public QDialog {
    Q_OBJECT

public:
    InfoWindow(const QString &title, const QString &content, QWidget *parent = nullptr);

private:
    QListWidget *contentView;
};
