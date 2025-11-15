#include "Memory.h"

Memory::Memory(int size) : mem(size, 0) {}

void Memory::clear() {
    std::fill(mem.begin(), mem.end(), 0);
}

void Memory::addFault(Fault f) {
    faults.push_back(f);
}

void Memory::clearFaults() {
    faults.clear();
}

void Memory::setSize(int newSize) {
    if (newSize < 0) return;

    std::vector<Fault> kept;
    for (auto &f : faults) {
        if (f.addr >= 0 && f.addr < newSize && (f.aux < 0 || f.aux < newSize))
            kept.push_back(f);
    }
    faults.swap(kept);

    mem.resize(newSize, 0);
}

bool Memory::hasFault(int addr, FaultType t) const {
    for (auto &f : faults)
        if (f.addr == addr && f.type == t)
            return true;
    return false;
}

std::vector<Fault> Memory::getFaultsAt(int addr) const {
    std::vector<Fault> out;
    for (auto& f : faults)
        if (f.addr == addr)
            out.push_back(f);
    return out;
}

int Memory::resolveAddress(int addr) const {
    for (auto &f : faults) {
        if (f.type == FaultType::ADF_ALIAS && f.addr == addr)
            return f.aux;
    }
    return addr;
}

int Memory::read(int addr) {
    addr = resolveAddress(addr);

    if (hasFault(addr, FaultType::ADF_BLOCK))
        return 0;

    if (hasFault(addr, FaultType::SAF0)) return 0;
    if (hasFault(addr, FaultType::SAF1)) return 1;

    int val = mem[addr];

    for (auto& f : faults) {
        if (f.addr == addr) {
            if (f.type == FaultType::RDF_FLIP)
                mem[addr] ^= 1;
            else if (f.type == FaultType::RDF_RESET0)
                mem[addr] = 0;
        }
    }

    return val;
}

void Memory::write(int addr, int value) {
    int resolved = resolveAddress(addr);

    if (hasFault(resolved, FaultType::ADF_BLOCK))
        return;

    if (value == 1 && hasFault(resolved, FaultType::TF_0to1))
        return;
    if (value == 0 && hasFault(resolved, FaultType::TF_1to0))
        return;

    mem[resolved] = value;

    for (auto& f : faults)
        if (f.type == FaultType::ADF_MIRROR && f.addr == resolved)
            mem[f.aux] = value;

    for (auto& f : faults) {
        if (f.addr != resolved) continue;

        switch (f.type) {

        case FaultType::CF_INVERT:
        case FaultType::WDF_FLIP:
            mem[f.aux] ^= 1;
            break;

        case FaultType::CF_FORCE0:
            mem[f.aux] = 0;
            break;

        case FaultType::CF_FORCE1:
            mem[f.aux] = 1;
            break;

        case FaultType::CF_IDEM:
            mem[f.aux] = value;
            break;

        default:
            break;
        }
    }
}
