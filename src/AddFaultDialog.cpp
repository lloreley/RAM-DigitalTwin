#include "AddFaultDialog.h"
#include "InfoWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

AddFaultDialog::AddFaultDialog(Memory *mem, QWidget *parent)
    : QDialog(parent), memory(mem), infoWindow(nullptr)
{
    setWindowTitle("Добавить неисправность");
    setModal(true);
    resize(500, 300);

    auto *mainLayout = new QVBoxLayout(this);

    auto *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Тип неисправности:"));
    faultTypeBox = new QComboBox();
    faultTypeBox->addItem("SAF0", (int)FaultType::SAF0);
    faultTypeBox->addItem("SAF1", (int)FaultType::SAF1);
    faultTypeBox->addItem("TF_0to1", (int)FaultType::TF_0to1);
    faultTypeBox->addItem("TF_1to0", (int)FaultType::TF_1to0);
    faultTypeBox->addItem("CF_INVERT", (int)FaultType::CF_INVERT);
    faultTypeBox->addItem("CF_FORCE0", (int)FaultType::CF_FORCE0);
    faultTypeBox->addItem("CF_FORCE1", (int)FaultType::CF_FORCE1);
    faultTypeBox->addItem("CF_IDEM", (int)FaultType::CF_IDEM);
    faultTypeBox->addItem("ADF_ALIAS", (int)FaultType::ADF_ALIAS);
    faultTypeBox->addItem("ADF_BLOCK", (int)FaultType::ADF_BLOCK);
    faultTypeBox->addItem("ADF_MIRROR", (int)FaultType::ADF_MIRROR);
    faultTypeBox->addItem("RDF_FLIP", (int)FaultType::RDF_FLIP);
    faultTypeBox->addItem("RDF_RESET0", (int)FaultType::RDF_RESET0);
    faultTypeBox->addItem("WDF_FLIP", (int)FaultType::WDF_FLIP);

    typeLayout->addWidget(faultTypeBox);

    infoBtn = new QPushButton("?");
    infoBtn->setMaximumWidth(40);
    connect(infoBtn, &QPushButton::clicked, this, &AddFaultDialog::showFaultInfo);
    typeLayout->addWidget(infoBtn);

    mainLayout->addLayout(typeLayout);

    auto *addrLayout = new QHBoxLayout();
    addrLayout->addWidget(new QLabel("Адрес ячейки:"));
    addrBox = new QSpinBox();
    addrBox->setRange(0, memory->size() - 1);
    addrLayout->addWidget(addrBox);
    addrLayout->addStretch();
    mainLayout->addLayout(addrLayout);

    auto *auxLayout = new QHBoxLayout();
    auxLabel = new QLabel("Адрес жертвы:");
    auxBox = new QSpinBox();
    auxBox->setRange(-1, memory->size() - 1);
    auxBox->setValue(-1);
    auxLayout->addWidget(auxLabel);
    auxLayout->addWidget(auxBox);
    auxLayout->addStretch();
    mainLayout->addLayout(auxLayout);

    auto *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton("Добавить");
    auto *cancelBtn = new QPushButton("Отмена");

    connect(addBtn, &QPushButton::clicked, this, &AddFaultDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(faultTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddFaultDialog::onFaultTypeChanged);

    btnLayout->addStretch();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    mainLayout->addStretch();
    onFaultTypeChanged(0);
}

void AddFaultDialog::onFaultTypeChanged(int index)
{
    currentType = (FaultType)faultTypeBox->itemData(index).toInt();
    updateFields();
}

void AddFaultDialog::updateFields()
{
    bool needsAux = false;

    switch (currentType)
    {
    case FaultType::CF_INVERT:
    case FaultType::CF_FORCE0:
    case FaultType::CF_FORCE1:
    case FaultType::CF_IDEM:
        needsAux = true;
        break;

    case FaultType::ADF_ALIAS:
    case FaultType::ADF_MIRROR:
        needsAux = true;
        break;

    case FaultType::WDF_FLIP:
        needsAux = true;
        break;

    default:
        needsAux = false;
        break;
    }

    auxLabel->setVisible(needsAux);
    auxBox->setVisible(needsAux);
}

void AddFaultDialog::showFaultInfo()
{
    QString faultName = faultTypeBox->currentText();
    QString description = getFaultDescription(currentType);

    if (infoWindow)
    {
        infoWindow->close();
        infoWindow->deleteLater();
    }

    infoWindow = new InfoWindow(faultName, description, this);
    infoWindow->show();
}

QString AddFaultDialog::getFaultDescription(FaultType type)
{
    switch (type)
    {
    case FaultType::SAF0:
        return "SAF0 — Stuck-At-Fault 0:\n"
               "\n"
               "Описание: Ячейка памяти всегда возвращает 0\n"
               "\n"
               "Как работает:\n"
               "Если ячейка имеет SAF0, чтение всегда возвращает застрявшее значение 0\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Zero-One — отлично ловит эту ошибку\n"
               "✓ Тест March C- — отлично ловит эту ошибку\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::SAF1:
        return "SAF1 — Stuck-At-Fault 1:\n"
               "\n"
               "Описание: Ячейка памяти всегда возвращает 1\n"
               "\n"
               "Как работает:\n"
               "Если ячейка имеет SAF1, чтение всегда возвращает застрявшее значение 1\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Zero-One — отлично ловит эту ошибку\n"
               "✓ Тест March C- — отлично ловит эту ошибку\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::TF_0to1:
        return "TF_0to1 — Transition Fault 0→1:\n"
               "\n"
               "Описание: Ячейка не может перейти из 0 в 1\n"
               "\n"
               "Как работает:\n"
               "При записи нового значения проверяется, разрешён ли переход.\n"
               "Если запись 1 запрещена, значение остаётся 0.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест March C- — обычно выявляет\n"
               "~ Тест Zero-One — может выявить, если запись противоречит переходу\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::TF_1to0:
        return "TF_1to0 — Transition Fault 1→0:\n"
               "\n"
               "Описание: Ячейка не может перейти из 1 в 0\n"
               "\n"
               "Как работает:\n"
               "При записи нового значения проверяется, разрешён ли переход.\n"
               "Если запись 0 запрещена, значение остаётся 1.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест March C- — обычно выявляет\n"
               "~ Тест Zero-One — может выявить, если запись противоречит переходу\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::CF_INVERT:
        return "CF_INVERT — Coupling Fault Invert:\n"
               "\n"
               "Описание: Запись в агрессор инвертирует жертву\n"
               "\n"
               "Как работает:\n"
               "Любая запись в агрессор (адрес) автоматически инвертирует жертву (адрес жертвы).\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Checkerboard — обычно выявляет\n"
               "✓ Тест March C- — обычно выявляет\n"
               "✗ Тест Zero-One — может не выявить, если агрессор не пишется\n"
               "\n"
               "Параметры: Адрес агрессора и адрес жертвы";

    case FaultType::CF_FORCE0:
        return "CF_FORCE0 — Coupling Fault Force to 0:\n"
               "\n"
               "Описание: Запись в агрессор делает жертву 0\n"
               "\n"
               "Как работает:\n"
               "Любая запись в агрессор (адрес) автоматически устанавливает жертву (адрес жертвы) в 0.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Checkerboard — обычно выявляет\n"
               "✓ Тест March C- — обычно выявляет\n"
               "✗ Тест Zero-One — может не выявить, если агрессор не пишется\n"
               "\n"
               "Параметры: Адрес агрессора и адрес жертвы";

    case FaultType::CF_FORCE1:
        return "CF_FORCE1 — Coupling Fault Force to 1:\n"
               "\n"
               "Описание: Запись в агрессор делает жертву 1\n"
               "\n"
               "Как работает:\n"
               "Любая запись в агрессор (адрес) автоматически устанавливает жертву (адрес жертвы) в 1.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Checkerboard — обычно выявляет\n"
               "✓ Тест March C- — обычно выявляет\n"
               "✗ Тест Zero-One — может не выявить, если агрессор не пишется\n"
               "\n"
               "Параметры: Адрес агрессора и адрес жертвы";

    case FaultType::CF_IDEM:
        return "CF_IDEM — Coupling Fault Idempotent:\n"
               "\n"
               "Описание: Жертва копирует значение агрессора\n"
               "\n"
               "Как работает:\n"
               "Любая запись в агрессор (адрес) автоматически делает жертву (адрес жертвы) равной значению агрессора.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Checkerboard — обычно выявляет\n"
               "✓ Тест March C- — обычно выявляет\n"
               "✗ Тест Zero-One — может не выявить, если агрессор не пишется\n"
               "\n"
               "Параметры: Адрес агрессора и адрес жертвы";

    case FaultType::ADF_ALIAS:
        return "ADF_ALIAS — Address Decoder Fault Alias:\n"
               "\n"
               "Описание: Два адреса указывают на одну и ту же ячейку\n"
               "\n"
               "Как работает:\n"
               "Запись по адресу фактически выполняется в адрес жертвы (оба адреса указывают на одну ячейку).\n"
               "Чтение любого из двух адресов возвращает значение из одной ячейки.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест March C- — может выявить при соответствующих паттернах\n"
               "✓ Тест Checkerboard — может выявить при соответствующих паттернах\n"
               "✗ Тест Zero-One — часто не видит, если совпадение случайное\n"
               "\n"
               "Параметры: Адрес 1 и Адрес 2 (оба указывают на одну ячейку)";

    case FaultType::ADF_BLOCK:
        return "ADF_BLOCK — Address Decoder Fault Block:\n"
               "\n"
               "Описание: Адрес недоступен для чтения/записи\n"
               "\n"
               "Как работает:\n"
               "Операции чтения и записи по этому адресу игнорируются.\n"
               "Чтение вернёт какое-то значение, но запись не будет выполнена.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест March C- — может выявить при соответствующих паттернах\n"
               "✓ Тест Checkerboard — может выявить при соответствующих паттернах\n"
               "✗ Тест Zero-One — часто не видит, если совпадение случайное\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::ADF_MIRROR:
        return "ADF_MIRROR — Address Decoder Fault Mirror:\n"
               "\n"
               "Описание: Запись в один адрес дублируется в другой\n"
               "\n"
               "Как работает:\n"
               "Запись по адресу автоматически пишет то же значение в адрес жертвы (зеркальный адрес).\n"
               "Чтение из адреса возвращает его собственное значение, но запись влияет на оба адреса.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест March C- — может выявить при соответствующих паттернах\n"
               "✓ Тест Checkerboard — может выявить при соответствующих паттернах\n"
               "✗ Тест Zero-One — часто не видит, если совпадение случайное\n"
               "\n"
               "Параметры: Адрес и адрес зеркала (жертва)";

    case FaultType::RDF_FLIP:
        return "RDF_FLIP — Read Disturb Fault Flip:\n"
               "\n"
               "Описание: Чтение инвертирует значение ячейки\n"
               "\n"
               "Как работает:\n"
               "Каждое чтение ячейки переворачивает её значение.\n"
               "Первое чтение вернёт исходное значение, второе — инвертированное, третье — снова исходное и т.д.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Read-Disturb — специально разработан для этого\n"
               "✗ Тесты Zero-One, Checkerboard, March C- — обычно не видят\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::RDF_RESET0:
        return "RDF_RESET0 — Read Disturb Fault Reset to 0:\n"
               "\n"
               "Описание: Чтение сбрасывает значение в 0\n"
               "\n"
               "Как работает:\n"
               "После нескольких чтений ячейка теряет заряд и сбрасывается в значение 0.\n"
               "Первое чтение может вернуть исходное значение, но последующие чтения вернут 0.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Read-Disturb — специально разработан для этого\n"
               "✗ Тесты Zero-One, Checkerboard, March C- — обычно не видят\n"
               "\n"
               "Параметры: Только адрес ячейки";

    case FaultType::WDF_FLIP:
        return "WDF_FLIP — Write Disturb Fault Flip:\n"
               "\n"
               "Описание: Запись в соседнюю ячейку инвертирует эту ячейку\n"
               "\n"
               "Как работает:\n"
               "Запись в агрессор (адрес) автоматически инвертирует жертву (адрес жертвы).\n"
               "Это похоже на CF_INVERT, но срабатывает при записи в соседнюю ячейку.\n"
               "\n"
               "Обнаружение:\n"
               "✓ Тест Checkerboard — обычно выявляет\n"
               "✓ Тест March C- — обычно выявляет\n"
               "✗ Тест Zero-One — может не выявить, если агрессор не пишется\n"
               "\n"
               "Параметры: Адрес агрессора (соседней ячейки) и адрес жертвы";

    default:
        return "Неизвестный тип неисправности";
    }
}

void AddFaultDialog::onAddClicked()
{
    currentFault = {currentType, addrBox->value(), auxBox->value()};
    accept();
}
