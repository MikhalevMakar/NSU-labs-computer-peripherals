#include <iostream>
#include <memory>
#include <unordered_map>
#include <iomanip>
#include <limits.h>
#include <vector>
#include "Matrix.h"
#include <set>

uint64_t GetRdtsc(){
    uint64_t lo,hi;__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

uint64_t GetTSC() {
    uint64_t highPart, lowPart;
    asm volatile("rdtsc\n":"=a"(lowPart), "=d"(highPart));
    return (highPart << 32) | (lowPart);
}

int findZero(size_t size, int* array) {
    for(size_t i = 0; i < size; ++i) {
        if(!array[i]) return i;
    }
    return 0;
}

void Swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void generateTruthRandomByPass(size_t size, int* array) {
    std::vector<bool> checkArray;
    checkArray.reserve(size);
    int counter = 0;
    int value = 0;

    for (size_t i = 0; i < size; ++i) {
        value = array[value];
        if (checkArray[value] == true) break;
        checkArray[value] = true;
        ++counter;
    }

    if (counter == size) return;

    int indexSwap;
    for (int i = 0; i < size; ++i) {
        if (!checkArray[i]) {
            indexSwap = i;
            break;
        }
    }

    int tmpIndex = findZero(size, array);
    Swap(reinterpret_cast<int *>(&array[tmpIndex]), reinterpret_cast<int *>(&array[indexSwap]));
    generateTruthRandomByPass(size, array);
}

void directArrayTraversal(size_t sizeArray, int* array) {
    for(size_t i = 0; i < sizeArray-1; ++i) {
        array[i] = i+1;
    }
    array[sizeArray-1] = 0;
}

void reverseArrayTraversal(size_t sizeArray, int* array) {
    for(size_t i = sizeArray-1; i >= 1; --i) {
        array[i] = i-1;
    }
    array[0] = sizeArray-1;
}

void randomArrayTraversal(size_t sizeArray, int* array) {
    std::unordered_map<int , int> mapIndex;
    for(size_t i = 0; i < sizeArray-1; ++i) {
        mapIndex.insert({i+1, i+1});
    }

    srand(time(0));
    int indexArray = 0;
    int getRandomValue;
    while(!mapIndex.empty()) {
        getRandomValue = rand() % sizeArray;
        if(mapIndex.find(getRandomValue) != mapIndex.end()) {
            array[indexArray++] = getRandomValue;
            mapIndex.erase(getRandomValue);
        }
    }
    array[sizeArray-1] = 0;
    generateTruthRandomByPass(sizeArray, array);
}

void printTime(size_t size, uint64_t firstTime, uint64_t secondTime, uint64_t thirdTime) {
    std::cout  << std::setw(4) << firstTime
               << std::setw(15) << secondTime
               << std::setw(15) << thirdTime
               << std::setw(20) << size*sizeof(int) << std::endl;
}

void warmCPU() {
    Matrix A(100);
    Matrix B(100);
    A.fillIdentityMatrix();
    B.fillZero();
    Matrix C = A * B;
}

uint64_t tackTime(size_t sizeArray, int* array) {
    warmCPU();
    size_t countByPass = 100;

    uint64_t minTime = INT_MAX;
    uint64_t tick  = GetTSC();
    for (size_t i = 0, k = 0; i < sizeArray*countByPass; ++i) {
        k = array[k];
    }
    uint64_t tmpTime =  GetTSC() - tick;
    uint64_t totalTime = tmpTime / static_cast<uint64_t>(sizeArray*countByPass);
    return totalTime;
}

void threeTypesArrayTraversal(size_t beginSizeArray, size_t endSizeArray) {
    //auto array = std::shared_ptr<int[]>(new int[INT_MAX]);
    auto array = new int[endSizeArray];

    std::cout << std::setw(6) << "Direct/tick"
              <<  std::setw(15) << "Reverse/tick"
              << std::setw(15) << "Random/tick"
              << std::setw(15) << " Size\n";

    uint64_t  firstTimeTime, secondTime, thirdTime;
    for(size_t size = beginSizeArray; size <= endSizeArray; size *= 1.2) {
        directArrayTraversal(size, array);
        firstTimeTime = tackTime(size, array);

        reverseArrayTraversal(size, array);
        secondTime = tackTime(size, array);

        randomArrayTraversal(size, array);
        thirdTime= tackTime(size, array);

        printTime(size, firstTimeTime, secondTime, thirdTime);
    }
}

int main(int argc,  char *argv[]) {
    if(argc != 3) {
        std::cerr << "Not enough arguments";
        return EXIT_FAILURE;
    }

    threeTypesArrayTraversal(static_cast<size_t>(strtol(argv[1], nullptr, 10)),
                             static_cast<size_t>(strtol(argv[2], nullptr, 10)));

    return 0;
}