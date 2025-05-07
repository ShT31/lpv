#include <bits/stdc++.h>  // Includes all standard C++ libraries
#include <omp.h>          // OpenMP library for parallelism
using namespace std;

// Function to print the elements of the array
void display(int n, vector<int> &arr)
{
    cout << "Array: " << endl;
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Sequential Bubble Sort implementation
void bubbleSort(int n, vector<int> &arr)
{
    // Loop through the array and compare adjacent elements
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            // Swap if the current element is greater than the next
            if (arr[i] > arr[j])
                swap(arr[i], arr[j]);
        }
    }
}

// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(int n, vector<int> &arr)
{
    // Start parallel region
#pragma omp parallel
    for (int i = 0; i < n; i++)
    {
        // Odd indexed pass: compares and swaps (1,2), (3,4), ...
#pragma omp for
        for (int j = 1; j < n; j += 2)
        {
            if (arr[j - 1] > arr[j])
            {
                swap(arr[j], arr[j - 1]);
            }
        }

        // Barrier ensures all threads complete odd pass before starting even pass
#pragma omp barrier

        // Even indexed pass: compares and swaps (0,1), (2,3), ...
#pragma omp for
        for (int j = 2; j < n; j += 2)
        {
            if (arr[j - 1] > arr[j])
            {
                swap(arr[j], arr[j - 1]);
            }
        }

        // Barrier ensures all threads finish the even phase before the next iteration
#pragma omp barrier
    }
}

// Function to merge two sorted halves of the array
void merge(vector<int> &arr, int low, int mid, int high)
{
    // Sizes of two subarrays
    int n1 = mid - low + 1;
    int n2 = high - mid;

    // Create temporary subarrays
    vector<int> left(n1), right(n2);

    // Copy data to subarrays
    for (int i = 0; i < n1; i++)
        left[i] = arr[low + i];

    for (int i = 0; i < n2; i++)
        right[i] = arr[mid + 1 + i];

    // Merge the two subarrays back into the main array
    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    // Copy remaining elements of left array (if any)
    while (i < n1)
        arr[k++] = left[i++];

    // Copy remaining elements of right array (if any)
    while (j < n2)
        arr[k++] = right[j++];
}

// Recursive Sequential Merge Sort
void mergeSort(vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

        // Recursively sort first and second halves
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);

        // Merge sorted halves
        merge(arr, low, mid, high);
    }
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

        // Split work into parallel sections
#pragma omp parallel sections
        {
            // Parallel recursive call for left half
#pragma omp section
            {
                parallelMergeSort(arr, low, mid);
            }

            // Parallel recursive call for right half
#pragma omp section
            {
                parallelMergeSort(arr, mid + 1, high);
            }
        }

        // Merge the two sorted halves
        merge(arr, low, mid, high);
    }
}

// Main function
int main()
{
    cout << "Enter the array size: ";
    int n;
    cin >> n;

    cout << "Enter array elements: " << endl;
    vector<int> arr(n);

    // Take array input from user
    for (int i = 0; i < n; i++)
    {
        cout << "Enter element " << i + 1 << " :";
        cin >> arr[i];
    }

    // Copy of the array to use in each sorting method
    vector<int> temp = arr;

    double start_time, end_time;

    // Sequential Bubble Sort timing
    start_time = omp_get_wtime();
    bubbleSort(n, temp);
    end_time = omp_get_wtime();
    cout << "Total time taken by Sequential Bubble Sort is: " << end_time - start_time << endl;
    cout << "The array after sort is: " << endl;
    display(n, temp);

    temp = arr; // Reset array for next sort

    // Parallel Bubble Sort timing
    start_time = omp_get_wtime();
    parallelBubbleSort(n, temp);
    end_time = omp_get_wtime();
    cout << "Total time taken by Parallel Bubble Sort is: " << end_time - start_time << endl;
    cout << "The array after sort is: " << endl;
    display(n, temp);

    temp = arr; // Reset array for next sort

    // Sequential Merge Sort timing
    start_time = omp_get_wtime();
    mergeSort(temp, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Total time taken by Sequential Merge Sort is: " << end_time - start_time << endl;
    cout << "The array after sort is: " << endl;
    display(n, temp);

    temp = arr; // Reset array for next sort

    // Parallel Merge Sort timing
    start_time = omp_get_wtime();
    parallelMergeSort(temp, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Total time taken by Parallel Merge Sort is: " << end_time - start_time << endl;
    cout << "The array after sort is: " << endl;
    display(n, temp);

    return 0;
}
