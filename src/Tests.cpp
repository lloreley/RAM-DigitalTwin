#include "Tests.h"

TestResult Tests::ZeroOne(Memory& m) {
    TestResult r{"Zero-One"};

    for (int i = 0; i < m.size(); i++) m.write(i, 0);
    for (int i = 0; i < m.size(); i++) {
        if (m.read(i) != 0) {
            r.errors++;
            r.log.push_back("Проверка 0: ячейка " + std::to_string(i) + " содержит неверное значение");
        }
    }

    for (int i = 0; i < m.size(); i++) m.write(i, 1);
    for (int i = 0; i < m.size(); i++) {
        if (m.read(i) != 1) {
            r.errors++;
            r.log.push_back("Проверка 1: ячейка " + std::to_string(i) + " содержит неверное значение");
        }
    }

    return r;
}

TestResult Tests::Checkerboard(Memory& m) {
    TestResult r{"Checkerboard"};

    for (int i = 0; i < m.size(); i++) m.write(i, i % 2);
    for (int i = 0; i < m.size(); i++)
        if (m.read(i) != (i % 2)) {
            r.errors++;
            r.log.push_back("Паттерн 1010: ячейка " + std::to_string(i) + " содержит неверное значение");
        }

    for (int i = 0; i < m.size(); i++) m.write(i, (i % 2 ? 0 : 1));
    for (int i = 0; i < m.size(); i++)
        if (m.read(i) != (i % 2 ? 0 : 1)) {
            r.errors++;
            r.log.push_back("Паттерн 0101: ячейка " + std::to_string(i) + " содержит неверное значение");
        }

    return r;
}

TestResult Tests::MarchC(Memory& m) {
    TestResult r{"March C-"};

    for (int i = 0; i < m.size(); i++) m.write(i, 0);

    for (int i = 0; i < m.size(); i++) {
        int val = m.read(i);
        if (val != 0) {
            r.errors++;
            r.log.push_back("↑ read 0: ячейка " + std::to_string(i) + " содержит " + std::to_string(val));
        }
        m.write(i, 1);
    }

    for (int i = m.size() - 1; i >= 0; i--) {
        int val = m.read(i);
        if (val != 1) {
            r.errors++;
            r.log.push_back("↓ read 1: ячейка " + std::to_string(i) + " содержит " + std::to_string(val));
        }
        m.write(i, 0);
    }

    for (int i = m.size() - 1; i >= 0; i--) {
        int val = m.read(i);
        if (val != 0) {
            r.errors++;
            r.log.push_back("↓ read 0: ячейка " + std::to_string(i) + " содержит " + std::to_string(val));
        }
    }

    return r;
}

TestResult Tests::ReadDisturb(Memory& m) {
    TestResult r{"Read-Disturb"};

    for (int i = 0; i < m.size(); i++) m.write(i, 1);

    for (int pass = 0; pass < 3; pass++) {
        for (int i = 0; i < m.size(); i++) {
            int val = m.read(i);
            if (val != 1) {
                r.errors++;
                r.log.push_back("Чтение 1, проход " + std::to_string(pass) +
                               ": ячейка " + std::to_string(i) + " содержит " + std::to_string(val));
            }
        }
    }

    for (int i = 0; i < m.size(); i++) m.write(i, 0);

    for (int pass = 0; pass < 3; pass++) {
        for (int i = 0; i < m.size(); i++) {
            int val = m.read(i);
            if (val != 0) {
                r.errors++;
                r.log.push_back("Чтение 0, проход " + std::to_string(pass) +
                               ": ячейка " + std::to_string(i) + " содержит " + std::to_string(val));
            }
        }
    }

    return r;
}
