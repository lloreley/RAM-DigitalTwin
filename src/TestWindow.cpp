#include "TestWindow.h"
#include "InfoWindow.h"
#include "Tests.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <Qt>
#include <QDateTime>
#include <QSplitter>

TestWindow::TestWindow(Memory *mem, QWidget *parent)
    : QMainWindow(parent), memory(mem), infoWindow(nullptr)
{
    auto *w = new QWidget();
    auto *mainLayout = new QVBoxLayout(w);

    auto *controlLayout = new QHBoxLayout();

    auto *testLabel = new QLabel("Выбери тест:");
    testComboBox = new QComboBox();
    testComboBox->addItem("Zero-One");
    testComboBox->addItem("Checkerboard");
    testComboBox->addItem("March C-");
    testComboBox->addItem("Read-Disturb");

    infoBtn = new QPushButton("Инфо");
    infoBtn->setMaximumWidth(100);
    connect(infoBtn, &QPushButton::clicked, this, &TestWindow::showTestInfo);

    runBtn = new QPushButton("Запустить Тест");
    runBtn->setMaximumWidth(150);
    connect(runBtn, &QPushButton::clicked, this, &TestWindow::runSelectedTest);

    controlLayout->addWidget(testLabel);
    controlLayout->addWidget(testComboBox, 1);
    controlLayout->addWidget(infoBtn);
    controlLayout->addWidget(runBtn);

    mainLayout->addLayout(controlLayout);

    logView = new QListWidget();
    logView->setStyleSheet("QListWidget { font-family: Courier; font-size: 10pt; }");
    mainLayout->addWidget(logView);

    historyBtn = new QPushButton("История Тестов");
    historyBtn->setMaximumHeight(30);
    connect(historyBtn, &QPushButton::clicked, this, &TestWindow::showTestHistory);
    mainLayout->addWidget(historyBtn);

    testHistory = new QListWidget();
    testHistory->setVisible(false);

    setWindowTitle("Тестирование Памяти");
    resize(500, 400);
    setCentralWidget(w);
}

void TestWindow::addLogItem(const QString &text)
{
    logView->addItem(text);
    logView->scrollToBottom();
}

void TestWindow::addHistoryItem(const QString &testName, bool passed, int errors)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString status = passed ? "✅ ПРОШЁЛ" : "❌ НЕ ПРОШЁЛ";
    QString item = QString("%1 | %2 | %3 ошибок").arg(time, testName).arg(errors);

    if (passed)
    {
        item = QString("%1 | %2 | ОК").arg(time, testName);
    }

    testHistory->insertItem(0, item);
}

void TestWindow::showTestHistory()
{
    if (testHistory->count() == 0)
    {
        addLogItem("История пуста. Запустите хотя бы один тест.");
        return;
    }

    if (infoWindow)
    {
        infoWindow->close();
        infoWindow->deleteLater();
    }

    QString historyText;
    for (int i = 0; i < testHistory->count(); ++i)
    {
        historyText += testHistory->item(i)->text() + "\n";
    }

    infoWindow = new InfoWindow("История всех тестов", historyText, this);
    infoWindow->show();
}

void TestWindow::showTestInfo()
{
    QString testName = testComboBox->currentText();
    QString title = getTestTitle(testName);
    QString description = getTestDescription(testName);

    if (infoWindow)
    {
        infoWindow->close();
        infoWindow->deleteLater();
    }

    infoWindow = new InfoWindow(title, description, this);
    infoWindow->show();
}

QString TestWindow::getTestTitle(const QString &testName)
{
    if (testName == "Zero-One")
        return "Тест Zero-One";
    if (testName == "Checkerboard")
        return "Тест Checkerboard";
    if (testName == "March C-")
        return "Тест March C-";
    if (testName == "Read-Disturb")
        return "Тест Read-Disturb";
    return testName;
}

QString TestWindow::getTestDescription(const QString &testName)
{
    if (testName == "Zero-One")
    {
        return "Тест «нулей и единиц» (Zero-One Test)\n\n"
               "Назначение:\n"
               "Обнаружение SAF, части TF, а также выявление некоторых ошибок адресации.\n\n"
               "Последовательность:\n"
               "1. Записать во все ячейки 0.\n"
               "2. Прочитать и проверить.\n"
               "3. Записать во все ячейки 1.\n"
               "4. Прочитать и проверить.\n\n"
               "Покрытие ошибок:\n"
               "- Stuck-At Fault (SAF)\n"
               "- Часть Transition Fault (TF)\n"
               "- Простейшие Address Decoder Faults (ADF)";
    }
    else if (testName == "Checkerboard")
    {
        return "Checkerboard-тест («шахматная маска»)\n\n"
               "Назначение:\n"
               "Выявление межъячеечных неисправностей, особенно coupling-эффектов.\n\n"
               "Последовательность:\n"
               "1. Записать чередующийся паттерн 101010…\n"
               "2. Проверить.\n"
               "3. Записать инверсию 010101…\n"
               "4. Проверить.\n\n"
               "Покрытие ошибок:\n"
               "- Coupling faults (CF)\n"
               "- Ошибки соседства\n"
               "- Некоторые ADF и динамические ошибки";
    }
    else if (testName == "March C-")
    {
        return "Маршевый тест March C- (сокращённая версия)\n\n"
               "Назначение:\n"
               "Эффективная проверка памяти на широкий спектр дефектов с относительно простой последовательностью.\n\n"
               "Последовательность (упрощённая):\n"
               "1. Проход вперёд: записать 0.\n"
               "2. Проход вперёд: прочитать 0, затем записать 1.\n"
               "3. Проход назад: прочитать 1, затем записать 0.\n"
               "4. Проход назад: прочитать 0.\n\n"
               "Покрытие ошибок:\n"
               "- SAF\n"
               "- TF\n"
               "- Несколько типов CF\n"
               "- ADF\n"
               "- Частично динамические ошибки";
    }
    else if (testName == "Read-Disturb")
    {
        return "Тест на многократное чтение (Read-Disturb Test)\n\n"
               "Назначение:\n"
               "Обнаружение динамических неисправностей.\n\n"
               "Описание:\n"
               "Для выбранных адресов выполняется серия последовательных чтений, после которых проверяется, сохранилось ли значение.\n\n"
               "Покрытие:\n"
               "- Read-disturb\n"
               "- Частично coupling faults";
    }
    return "Неизвестный тест";
}

void TestWindow::runSelectedTest()
{
    logView->clear();

    int index = testComboBox->currentIndex();
    QString testName = testComboBox->itemText(index);

    addLogItem("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    addLogItem(QString("%1 | %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss"), testName));
    addLogItem("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

    TestResult r;
    if (testName == "Zero-One")
    {
        r = Tests::ZeroOne(*memory);
    }
    else if (testName == "Checkerboard")
    {
        r = Tests::Checkerboard(*memory);
    }
    else if (testName == "March C-")
    {
        r = Tests::MarchC(*memory);
    }
    else if (testName == "Read-Disturb")
    {
        r = Tests::ReadDisturb(*memory);
    }

    addLogItem("");
    addLogItem(QString("   Результат теста: %1").arg(testName));
    addLogItem(QString("   Размер памяти: %1 ячеек").arg(memory->size()));
    addLogItem("");

    if (r.errors == 0)
    {
        addLogItem("✅ ТЕСТ ПРОЙДЕН УСПЕШНО!");
        addLogItem("   Неисправности не обнаружены");
        addLogItem("   Память работает корректно");
        addHistoryItem(testName, true, 0);
    }
    else
    {
        addLogItem(QString("❌ ТЕСТ НЕ ПРОЙДЕН - Обнаружено %1 ошибок").arg(r.errors));
        addLogItem("");
        addLogItem("Подробности ошибок:");
        for (int i = 0; i < r.log.size() && i < 20; ++i)
        {
            addLogItem(QString("   [%1] %2").arg(i + 1).arg(QString::fromStdString(r.log[i])));
        }
        if (r.log.size() > 20)
        {
            addLogItem(QString("   ... и ещё %1 ошибок").arg(r.log.size() - 20));
        }
        addHistoryItem(testName, false, r.errors);
    }

    addLogItem("");
    addLogItem("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    addLogItem("");
}
