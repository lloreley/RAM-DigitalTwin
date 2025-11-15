#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>

#include "Memory.h"
#include "Tests.h"

class TestWindow;
class AddFaultDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addFault();
    void openTestWindow();
    void clearAllFaults();
    void changeMemorySize();

private:
    Memory memory;
    TestWindow *testWindow;
    AddFaultDialog *faultDialog;

    QTableWidget *table;
    QListWidget  *logView;

    QSpinBox *sizeBox;
    QPushButton *resizeBtn;
    QPushButton *clearFaultsBtn;

    void refreshTable();
    QString faultTypeToString(FaultType t);
};
