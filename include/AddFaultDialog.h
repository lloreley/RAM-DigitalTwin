#pragma once

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "Memory.h"

class InfoWindow;

class AddFaultDialog : public QDialog {
    Q_OBJECT

public:
    AddFaultDialog(Memory* mem, QWidget *parent = nullptr);
    Fault getFault() const { return currentFault; }

private slots:
    void onFaultTypeChanged(int index);
    void onAddClicked();
    void showFaultInfo();

private:
    Memory* memory;
    FaultType currentType;
    Fault currentFault;

    QComboBox *faultTypeBox;
    QSpinBox *addrBox;
    QSpinBox *auxBox;
    QLabel *auxLabel;
    QPushButton *infoBtn;
    QPushButton *addBtn;
    InfoWindow *infoWindow;

    void updateFields();
    QString getFaultDescription(FaultType type);
};
