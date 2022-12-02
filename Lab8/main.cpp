#include <iostream>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <iomanip>

union ticks{
    unsigned long long t64;
    struct s32{ long th,
                     tl;
    }t32;
} start, end;

void directArrayTraversal(int sizeArray, std::shared_ptr<int[]> array) {
    for(int i = 0; i < sizeArray-1; ++i) {
        array[i] = i+1;
    }
    array[sizeArray-1] = 0;
}

void reverseArrayTraversal(int sizeArray, std::shared_ptr<int[]> array) {
    for(int i = sizeArray-1; i >= 1; --i) {
        array[i] = i-1;
    }
    array[0] = sizeArray-1;
}


void randomArrayTraversal(int sizeArray, std::shared_ptr<int[]> array) {
    std::unordered_map<int , int> mapIndex;
    for(int i = 0; i < sizeArray-1; ++i) {
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
}

void printArray(int sizeArray, std::shared_ptr<int[]> array) {
    for(int i = 0; i < sizeArray; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
}

int getTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
            std::chrono::time_point<std::chrono::high_resolution_clock> end) {
    std::chrono::duration<double> durationTime(end - start);
    return  durationTime.count();
}

void printTime(int size, unsigned long long first, unsigned long long second, unsigned long long third) {
    std::cout  << std::setw(4) << first << std::setw(9) << second  << std::setw(11) << third <<  std::setw(12) << size << std::endl;
}

void warmUpProcessor(int sizeArray, std::shared_ptr<int[]> array) {
    int count = 0;
    while (count++ != 5) {
        for (int i = 0; i < sizeArray; ++i) {
            array[i] = i;
        }
    }
}

void accessTimeElement(int sizeArray, std::shared_ptr<int[]> array) {
    int value;
    for(int i = 0; i < sizeArray; ++i) {
        value = array[i];
    }
}

unsigned long long tackTime(int sizeArray, std::shared_ptr<int[]> array) {
    unsigned long long minTime = 100;
    for(int i = 0; i < 4; ++i) {
        asm("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl));
        accessTimeElement(sizeArray, array);
        asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
        int curTime = (end.t64 - start.t64) / sizeArray;
        minTime = (curTime > minTime) ? minTime : curTime;
    }
    return minTime;
}

void threeTypesArrayTraversal(int beginSizeArray, int endSizeArray) {
    auto array = std::shared_ptr<int[]>(new int[endSizeArray]);
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime,
                                                                endTime;
    unsigned long long  first, second, third;
    std::cout << std::setw(6) << "Direct" << std::setw(10) << "Reverse" << std::setw(10) << "Random" << std::setw(10) << " Size"  << std::endl;
    ticks time;
    for(int size = beginSizeArray; size <= endSizeArray; size *= 2) {
        warmUpProcessor(size, array);

        directArrayTraversal(size, array);
        first = tackTime(size, array);

        reverseArrayTraversal(size, array);
        second = tackTime(size, array);

        randomArrayTraversal(size, array);
        third = tackTime(size, array);

        printTime(size, first, second, third);
    }
}

int main(int argc,  char *argv[]) {
    if(argc != 3) {
        std::cerr << "Not enough arguments";
        return EXIT_FAILURE;
    }

    threeTypesArrayTraversal(static_cast<int>(strtol(argv[1], nullptr, 10)),
                             static_cast<int>(strtol(argv[2], nullptr, 10)));

    return 0;
}

