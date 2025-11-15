#pragma once
#include <vector>
#include <string>

enum class FaultType {
    SAF0, SAF1,
    TF_0to1, TF_1to0,
    CF_INVERT,
    CF_FORCE0,
    CF_FORCE1,
    CF_IDEM,
    ADF_ALIAS,
    ADF_BLOCK,
    ADF_MIRROR,
    RDF_FLIP,
    RDF_RESET0,
    WDF_FLIP
};

struct Fault {
    FaultType type;
    int addr;
    int aux;
};

class Memory {
public:
    Memory(int size);

    void clear();
    void addFault(Fault f);
    void clearFaults();
    void setSize(int newSize);

    int  read(int addr);
    void write(int addr, int value);

    std::vector<Fault> getFaultsAt(int addr) const;

    int size() const { return mem.size(); }
    const std::vector<int>& data() const { return mem; }

private:
    std::vector<int> mem;
    std::vector<Fault> faults;

    bool hasFault(int addr, FaultType t) const;
    int resolveAddress(int addr) const;
};
