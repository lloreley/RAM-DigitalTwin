#include "MainWindow.h"
#include "TestWindow.h"
#include "AddFaultDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QSpinBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), memory(32), testWindow(nullptr), faultDialog(nullptr)
{
    auto *w = new QWidget();
    auto *v = new QVBoxLayout(w);

    table = new QTableWidget(memory.size(), 2);
    table->setHorizontalHeaderLabels({"Адрес", "Ошибки"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    v->addWidget(table);

    auto *h = new QHBoxLayout();
    
    auto *btnAdd = new QPushButton("Добавить ошибку");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addFault);
    h->addWidget(btnAdd);
    h->addStretch();

    v->addLayout(h);

    auto *h2 = new QHBoxLayout();
    auto *sizeLabel = new QLabel("Размер ОЗУ:");
    sizeBox = new QSpinBox();
    sizeBox->setRange(1, 1024*1024);
    sizeBox->setValue(memory.size());
    resizeBtn = new QPushButton("Изменить размер");
    clearFaultsBtn = new QPushButton("Очистить ошибки");

    connect(resizeBtn, &QPushButton::clicked, this, &MainWindow::changeMemorySize);
    connect(clearFaultsBtn, &QPushButton::clicked, this, &MainWindow::clearAllFaults);

    h2->addWidget(sizeLabel);
    h2->addWidget(sizeBox);
    h2->addWidget(resizeBtn);
    h2->addWidget(clearFaultsBtn);
    v->addLayout(h2);

    auto *testBtn = new QPushButton("Открыть окно тестирования");
    connect(testBtn, &QPushButton::clicked, this, &MainWindow::openTestWindow);
    v->addWidget(testBtn);

    logView = new QListWidget();
    v->addWidget(logView);

    refreshTable();
    setCentralWidget(w);
    
    setWindowTitle("Симулятор ОЗУ с дефектами");
    resize(600, 600);
    
    raise();
    activateWindow();
}

void MainWindow::refreshTable() {
    int rows = memory.size();
    table->setRowCount(rows);

    for (int i = 0; i < rows; ++i) {
        auto *addrItem = new QTableWidgetItem(QString::number(i));

        auto faults = memory.getFaultsAt(i);
        QStringList fstrs;
        for (auto &f : faults) {
            QString s = faultTypeToString(f.type);
            
            bool needsAux = false;
            switch (f.type) {
            case FaultType::CF_INVERT:
            case FaultType::CF_FORCE0:
            case FaultType::CF_FORCE1:
            case FaultType::CF_IDEM:
            case FaultType::ADF_ALIAS:
            case FaultType::ADF_MIRROR:
            case FaultType::WDF_FLIP:
                needsAux = true;
                break;
            default:
                needsAux = false;
                break;
            }
            
            if (needsAux && f.aux >= 0) {
                s += QString("(%1)").arg(f.aux);
            }
            fstrs << s;
        }
        auto *faultItem = new QTableWidgetItem(fstrs.join(", "));

        table->setItem(i, 0, addrItem);
        table->setItem(i, 1, faultItem);

        if (!faults.empty()) {
            QBrush brush(QColor(255, 230, 230));
            addrItem->setBackground(brush);
            faultItem->setBackground(brush);
        }
    }
}

void MainWindow::addFault() {
    if (!faultDialog) {
        faultDialog = new AddFaultDialog(&memory, this);
    }
    
    if (faultDialog->exec() == QDialog::Accepted) {
        Fault f = faultDialog->getFault();
        memory.addFault(f);
        logView->addItem(QString("Ошибка добавлена: %1 на адресе %2").arg(faultTypeToString(f.type)).arg(f.addr));
        refreshTable();
    }
}

QString MainWindow::faultTypeToString(FaultType t) {
    switch (t) {
    case FaultType::SAF0: return "SAF0";
    case FaultType::SAF1: return "SAF1";
    case FaultType::TF_0to1: return "TF_0to1";
    case FaultType::TF_1to0: return "TF_1to0";
    case FaultType::CF_INVERT: return "CF_INVERT";
    case FaultType::CF_FORCE0: return "CF_FORCE0";
    case FaultType::CF_FORCE1: return "CF_FORCE1";
    case FaultType::CF_IDEM: return "CF_IDEM";
    case FaultType::ADF_ALIAS: return "ADF_ALIAS";
    case FaultType::ADF_BLOCK: return "ADF_BLOCK";
    case FaultType::ADF_MIRROR: return "ADF_MIRROR";
    case FaultType::RDF_FLIP: return "RDF_FLIP";
    case FaultType::RDF_RESET0: return "RDF_RESET0";
    case FaultType::WDF_FLIP: return "WDF_FLIP";
    default: return "UNKNOWN";
    }
}

void MainWindow::clearAllFaults() {
    memory.clearFaults();
    logView->addItem("Все ошибки очищены");
    refreshTable();
}

void MainWindow::changeMemorySize() {
    int newSize = sizeBox->value();
    memory.setSize(newSize);
    logView->addItem(QString("Размер ОЗУ изменён на %1 ячеек").arg(newSize));
    refreshTable();
}

void MainWindow::openTestWindow() {
    if (!testWindow) {
        testWindow = new TestWindow(&memory, this);
    }
    testWindow->show();
    testWindow->raise();
    testWindow->activateWindow();
}
