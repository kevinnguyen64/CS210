#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

bool isSorted(const std::vector<int>& values) {
    for (int i = 1; i < static_cast<int>(values.size()); ++i) {
        if (values[i - 1] > values[i]) return false;
    }
    return true;
}

void bubbleSort(std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    for (int end = n - 1; end > 0; --end) {
        bool swapped = false;
        for (int j = 0; j < end; ++j) {
            if (values[j] > values[j + 1]) {
                std::swap(values[j], values[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void selectionSort(std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (values[j] < values[minIndex]) minIndex = j;
        }
        std::swap(values[i], values[minIndex]);
    }
}

void insertionSort(std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    for (int i = 1; i < n; ++i) {
        int key = values[i];
        int j = i - 1;
        while (j >= 0 && values[j] > key) {
            values[j + 1] = values[j];
            --j;
        }
        values[j + 1] = key;
    }
}

int partition(std::vector<int>& values, int low, int high) {
    int pivot = values[high];
    int i = low;
    for (int j = low; j < high; ++j) {
        if (values[j] <= pivot) {
            std::swap(values[i], values[j]);
            ++i;
        }
    }
    std::swap(values[i], values[high]);
    return i;
}

void quickSort(std::vector<int>& values, int low, int high) {
    if (low >= high) return;
    int pivotIndex = partition(values, low, high);
    quickSort(values, low, pivotIndex - 1);
    quickSort(values, pivotIndex + 1, high);
}

void quickSortAll(std::vector<int>& values) {
    quickSort(values, 0, static_cast<int>(values.size()) - 1);
}

using SortFunction = void (*)(std::vector<int>&);

struct Algorithm {
    std::string name;
    SortFunction sort;
};

// Make the sorted input without using std::sort.
std::vector<int> makeSorted(std::vector<int> values) {
    insertionSort(values);
    return values;
}

std::vector<int> makeReverseSorted(std::vector<int> values) {
    insertionSort(values);
    int n = static_cast<int>(values.size());
    for (int i = 0; i < n / 2; ++i) {
        std::swap(values[i], values[n - 1 - i]);
    }
    return values;
}

double benchmark(const std::vector<int>& original, const std::vector<int>& expected, SortFunction sort, int repeats) {
    double totalMilliseconds = 0.0;

    for (int run = 0; run < repeats; ++run) {
        std::vector<int> values = original; // Copy before timing.

        auto start = std::chrono::steady_clock::now();
        sort(values);
        auto stop = std::chrono::steady_clock::now();

        // Verify outside the timed section.
        if (!isSorted(values) || values != expected) {
            throw std::runtime_error("A sorting result was incorrect.");
        }

        totalMilliseconds += std::chrono::duration<double, std::milli>(stop - start).count();
    }

    return totalMilliseconds / repeats;
}

int main() {
    const std::vector<Algorithm> algorithms{
        {"Bubble", bubbleSort},
        {"Selection", selectionSort},
        {"Insertion", insertionSort},
        {"Quick", quickSortAll}
    };

    const std::vector<int> sizes{250, 500, 1000};
    const int repeats = 5;
    std::mt19937 generator(42); // Fixed seed: repeatable random inputs.
    std::uniform_int_distribution<int> number(-100000, 100000);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Average sorting time in milliseconds (" << repeats << " runs per case)\n";
    std::cout << std::left << std::setw(9) << "Size" << std::setw(11) << "Input";
    for (const Algorithm& algorithm : algorithms) {
        std::cout << std::right << std::setw(13) << algorithm.name;
    }
    std::cout << '\n';

    for (int n : sizes) {
        std::vector<int> randomInput;
        randomInput.reserve(n);
        for (int i = 0; i < n; ++i) {
            randomInput.push_back(number(generator));
        }

        std::vector<int> sortedInput = makeSorted(randomInput);
        std::vector<int> reverseInput = makeReverseSorted(randomInput);

        const std::vector<std::pair<std::string, std::vector<int>>> inputs{
            {"Random", randomInput},
            {"Sorted", sortedInput},
            {"Reverse", reverseInput}
        };

        for (const auto& input : inputs) {
            // Prepare the correct answer before timing any algorithm.
            std::vector<int> expected = makeSorted(input.second);

            std::cout << std::left << std::setw(9) << n << std::setw(11) << input.first;

            for (const Algorithm& algorithm : algorithms) {
                double ms = benchmark(input.second, expected, algorithm.sort, repeats);
                std::cout << std::right << std::setw(13) << ms;
            }
            std::cout << '\n';
        }
    }
}