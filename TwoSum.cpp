#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::pair<int, int> twoSumBruteForce(
    const std::vector<int>& nums, int target) {

    // Check every possible pair of different elements.
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(nums.size()); ++j) {
            if (nums[i] + nums[j] == target) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

std::pair<int, int> twoSumHash(
    const std::vector<int>& nums, int target) {

    // This stores each value and the index where it appeared.
    std::unordered_map<int, int> seen;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int needed = target - nums[i];

        // Check whether the number we need was already seen.
        if (seen.find(needed) != seen.end()) {
            return {seen[needed], i};
        }

        // Add the current number after checking so we do not use
        // the same element twice.
        seen[nums[i]] = i;
    }

    return {-1, -1};
}

bool isValid(const std::vector<int>& nums,
             int target,
             std::pair<int, int> result) {
    int first = result.first;
    int second = result.second;

    // Make sure the indices are different and add to the target.
    return first >= 0 &&
           second >= 0 &&
           first != second &&
           nums[first] + nums[second] == target;
}

void printResult(const std::string& method,
                 const std::vector<int>& nums,
                 int target,
                 std::pair<int, int> result) {
    std::cout << method << ":\n";
    std::cout << "  Indices: " << result.first<< ", " << result.second << '\n';
    std::cout << "  Values: " << nums[result.first] << ", " << nums[result.second] << '\n';
    std::cout << "  Valid: "<< (isValid(nums, target, result) ? "yes" : "no")<< "\n";
}

void runTest(const std::string& name,
             const std::vector<int>& nums,
             int target) {
    std::cout << "\n" << name << " (target = " << target << ")\n";

    std::pair<int, int> bruteResult =
        twoSumBruteForce(nums, target);

    std::pair<int, int> hashResult =
        twoSumHash(nums, target);

    printResult("Brute force", nums, target, bruteResult);
    printResult("Hash method", nums, target, hashResult);
}

int main() {
    runTest(
        "Required test",
        {15, 4, 18, 8, 19, 22, 24, 59, 59, 20, 18, 12, 36, 42, 9},
        24
    );

    runTest("Positive values", {2, 7, 11, 15}, 9);
    runTest("Negative values", {-3, 4, 3, 90}, 0);
    runTest("Duplicate values", {3, 3}, 6);
    runTest("Zero values", {0, 4, 3, 0}, 0);

    return 0;
}