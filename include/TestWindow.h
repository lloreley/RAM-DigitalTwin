#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include "Memory.h"

class InfoWindow;

class TestWindow : public QMainWindow {
    Q_OBJECT

public:
    TestWindow(Memory* mem, QWidget *parent = nullptr);

private slots:
    void showTestInfo();
    void runSelectedTest();

private:
    Memory* memory;
    QComboBox *testComboBox;
    QListWidget *logView;
    QListWidget *testHistory;
    QPushButton *infoBtn;
    QPushButton *runBtn;
    QPushButton *historyBtn;
    InfoWindow *infoWindow;

    void addLogItem(const QString &text);
    void addHistoryItem(const QString &testName, bool passed, int errors);
    void showTestHistory();
    QString getTestDescription(const QString &testName);
    QString getTestTitle(const QString &testName);
};
