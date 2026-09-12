#include <iostream>
#include <vector>
#include <string>

int binarySearchIterative(const std::vector<int>& nums, int target, int& comparisons) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        comparisons++;
        if (nums[mid] == target) {
            return mid;
        }

        comparisons++;
        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}


int binarySearchRecursiveHelper(const std::vector<int>& nums,
      int target,
      int left,
      int right,
      int& comparisons) {

    if (left > right) {
        return -1;
    }

    int mid = left + (right - left) / 2;

    comparisons++;
    if (nums[mid] == target) {
        return mid;
    }

    comparisons++;

    if (nums[mid] < target) {
        return binarySearchRecursiveHelper(
            nums,
            target,
            mid + 1,
            right,
            comparisons
        );
    }

    return binarySearchRecursiveHelper(
        nums,
        target,
        left,
        mid - 1,
        comparisons
    );
}


int binarySearchRecursive(const std::vector<int>& nums,
     int target,
     int& comparisons) {

    return binarySearchRecursiveHelper(
        nums,
        target,
        0,
        nums.size() - 1,
        comparisons
    );
}


int linearSearch(const std::vector<int>& nums,
    int target,
    int& comparisons) {

    for (int i = 0; i < nums.size(); i++) {

        comparisons++;

        if (nums[i] == target) {
            return i;
        }
    }

    return -1;
}


void runTest(const std::string& testName, const std::vector<int>& nums,int target) {

    int iterativeComparisons = 0;
    int recursiveComparisons = 0;
    int linearComparisons = 0;

    int iterativeResult =
        binarySearchIterative(nums, target, iterativeComparisons);

    int recursiveResult =
        binarySearchRecursive(nums, target, recursiveComparisons);

    int linearResult =
        linearSearch(nums, target, linearComparisons);


    std::cout << "\n" << testName << std::endl;
    std::cout << "Target: " << target << std::endl;

    std::cout << "Iterative Binary Search:" << std::endl;
    std::cout << "Index: " << iterativeResult << std::endl;
    std::cout << "Comparisons: " << iterativeComparisons << std::endl;

    std::cout << "Recursive Binary Search:" << std::endl;
    std::cout << "Index: " << recursiveResult << std::endl;
    std::cout << "Comparisons: " << recursiveComparisons << std::endl;

    std::cout << "Linear Search:" << std::endl;
    std::cout << "Index: " << linearResult << std::endl;
    std::cout << "Comparisons: " << linearComparisons << std::endl;
}


int main() {

    std::vector<int> nums = {
        2, 5, 8, 12, 16,
        23, 31, 38, 45, 52,
        61, 70, 84, 93, 100
    };


    runTest("Test 1: First element", nums, 2);

    runTest("Test 2: Last element", nums, 100);

    runTest("Test 3: Middle element", nums, 38);

    runTest("Test 4: Missing below range", nums, 1);

    runTest("Test 5: Missing inside range", nums, 50);


    return 0;
}
