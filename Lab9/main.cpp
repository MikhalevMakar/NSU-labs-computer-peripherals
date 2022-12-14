#include <iostream>
enum {
    CountFragments  = 32,
    CashSize = 24 * 1024 * 1024 / sizeof(int),
    Offset = 24 * 1024 * 1024 / sizeof(int)
};

uint64_t GetRdtsc() {
    uint64_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t) hi << 32) | lo;
}

void FillCycleArray(size_t fragmentSize, size_t numFragments, int* array) {
    int pos;
    for(size_t i = 0; i < fragmentSize; ++i) {
        pos = i;
        for(size_t j = 0; j < numFragments-1; ++j) {
            array[pos] = pos + Offset;
            pos += Offset;
        }
        array[pos] =  (i + 1) % fragmentSize;;
    }
}

uint64_t TackTime(size_t sizeArray, int* array) {
    uint64_t minTime = UINT64_MAX;
    size_t countBypass = 30;
    for(size_t j = 0; j < countBypass; ++j) {
        int k = 0;
        uint64_t tick = GetRdtsc(            );
        for (size_t i = 0; i < sizeArray; ++i) {
            k = array[k];
        }
        uint64_t tmpTime = GetRdtsc() - tick;
        minTime = (minTime > tmpTime) ? tmpTime : minTime;
    }
    return minTime / (uint64_t)sizeArray;
}

void ByPassCashLevel() {
    int* array = new int[CashSize*CountFragments];

    for(int i = 1; i <= CountFragments; ++i) {
        FillCycleArray(CashSize/i, i, array);
        std:: cout << i << " " << TackTime(CashSize*i, array) << std::endl;
    }
    delete[] array;
}

int main() {
    ByPassCashLevel();
    return 0;
}
