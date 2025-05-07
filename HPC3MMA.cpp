#include <bits/stdc++.h>        // Includes all standard C++ libraries
using namespace std;
using namespace std::chrono;    // For measuring execution time

// Sequential operations to find min, max, sum, and average
void sequential_operations(vector<int> &arr, int &min_val, int &max_val, long long &sum, double &avg)
{
    // Find minimum element using STL
    min_val = *min_element(arr.begin(), arr.end());

    // Find maximum element using STL
    max_val = *max_element(arr.begin(), arr.end());

    // Calculate sum using accumulate; initialize with 0ll for long long
    sum = accumulate(arr.begin(), arr.end(), 0ll);

    // Calculate average
    avg = double(sum) / arr.size();
}

// Parallel operations using OpenMP to compute min, max, sum, and average
void parallel_operations(vector<int> &arr, int &min_val, int &max_val, long long &sum, double &avg)
{
    int n = arr.size();         // Store array size
    min_val = INT_MAX;          // Initialize min to highest possible int
    max_val = INT_MIN;          // Initialize max to lowest possible int
    sum = 0;                    // Initialize sum

    // Parallel region to compute min and max values
    // reduction(min : min_val) combines partial min results from each thread
    // reduction(max : max_val) combines partial max results
#pragma omp parallel for reduction(min : min_val) reduction(max : max_val)
    for (int i = 0; i < n; ++i)
    {
        min_val = min(min_val, arr[i]);
        max_val = max(max_val, arr[i]);
    }

    // Parallel region to compute the sum
    // reduction(+ : sum) ensures safe addition across threads
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }

    // Compute average after the parallel sections
    avg = double(sum) / n;
}

int main()
{
    // Size of the array
    int n = 100000000;

    // Create a large array with decreasing numbers: n, n-1, ..., 1
    vector<int> arr(n);
    for (int i = 0; i < n; ++i)
    {
        arr[i] = n - i;
    }

    int min_val, max_val;       // Variables to store min and max values
    long long sum;              // To store sum of all elements
    double avg;                 // To store average

    // Measure time for sequential operations
    auto start = high_resolution_clock::now();
    sequential_operations(arr, min_val, max_val, sum, avg);
    auto end = high_resolution_clock::now();
    duration<double> sequential_time = end - start;

    // Output results from sequential computation
    cout << "\nSequential Results:" << endl;
    cout << "Min: " << min_val << ", Max: " << max_val << ", Sum: " << sum << ", Average: " << avg << endl;
    cout << "Time taken for sequential: " << sequential_time.count() << " seconds" << endl;

    // Measure time for parallel operations
    start = high_resolution_clock::now();
    parallel_operations(arr, min_val, max_val, sum, avg);
    end = high_resolution_clock::now();
    duration<double> parallel_time = end - start;

    // Output results from parallel computation
    cout << "\nParallel Results:" << endl;
    cout << "Min: " << min_val << ", Max: " << max_val << ", Sum: " << sum << ", Average: " << avg << endl;
    cout << "Time taken for parallel: " << parallel_time.count() << " seconds" << endl;

    return 0;
}
