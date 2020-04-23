/*
 * Implementation of a Circular Dynamic Array
 * 
 * This file contains one class:
 * 1. CDA
 * 
 * This class is templated, and takes one typename,
 * referred to as "T" throughout the code.
 * 
 * The only library required for this class to work properly
 * is <cstdlib>, which contains rand(), which is used by QuickSort
 * to pick random pivot elements.
 * 
 * 
 * @author      Stephen Gregory
 * @date        04/21/2020
 */

#ifndef CDA_CPP
#define CDA_CPP

#include <cstdlib> // Only used for rand()

// CDA is a Circular Dynamic Array 
template <typename T>
class CDA {
    public:

        CDA();                                              // Default Constructor for a CDA object.
        CDA(int s);                                         // Constructor for CDA given an initial array size s.
        CDA(const CDA &cda);                                // Copy Constructor.
        CDA& operator=(const CDA &cda);                     // Copy Assignment Operator.
        T& operator[](int index);                           // Overloaded Bracket Operator, so CDA can be indexed like a regular array.

        void AddEnd(T v);                                   // Add an element v to the end of the CDA.
        void AddFront(T v);                                 // Add an element v to the front of the CDA.
        void DelEnd();                                      // Delete the element at the end of the CDA.
        void DelFront();                                    // Delete the element at the front of the CDA.
        void upsize();                                      // Helper function to double the size of the CDA.
        void downsize();                                    // Helper function to half the size of the CDA.

        int Length();                                       // Return the number of elements in the CDA.
        int Capacity();                                     // Return the total allocated capacity of the CDA.
        void Clear();                                       // Clear all of the elements in the CDA.
        bool Ordered();                                     // Returns true if the CDA is ordered, false otherwise.
        int SetOrdered();                                   // Check if the CDA is ordered, and assign is_ordered_ accordingly.

        T Select(int k);                                    // Return the kth smallest element in the CDA
        T QuickSelect(int k);                               // Helper function to find the kth smallest element in the CDA (calls QuickSelectReal).
        T QuickSelectReal(int left, int right, int k);      // Helper function using QuickSelect to find the kth smallest element in the CDA .
        int getMedianOfThreePivot(T low, T mid, T high);    // Find a random pivot element between indexes left and right.
        int getMedianOfThreeIndex(T low, int low_index, T mid, int mid_index, T high, int high_index);
        void QuickSortReal(int low, int high);              // Sort the CDA using Quick Sort (Switches to InsertionSortSubset).
        void QuickSort();                                   // Sort the CDA (Calls QuickSortReal).
        void InsertionSort();                               // Sort the CDA using Insertion Sort.
        void InsertionSortSubset(int low, int high);        // Sort a subset of the CDA using Insertion Sort.
        void CountingSort(int m);                           // Sort the CDA using Counting Sort.
        int Search(T e);                                    // Returns the index of the element e.
        int BinarySearch(T e, int left, int right);         // Helper function to get the index of the element e when CDA is sorted.
        int LinearSearch(T e);                              // Helper function to get the index of the element e when CDA is unsorted.
        ~CDA();

    private:

        int length_;                                        // The length/number of elements in the CDA.
        int capacity_;                                      // The total capacity of the CDA.
        bool is_ordered_;                                   // A flag representing whether the CDA is sorted or unsorted.
        int front_;                                         // The index of the "first" item of the array (as viewed externally).
        T *my_array_;                                       // Pointer to our dynamic array of T objects.
        T throw_away_;                                      // Sentinel value used when the user attempts to access an out of bounds index.
};


using namespace std;

template <typename T>
CDA<T>::CDA() {
    length_ = 0;
    capacity_ = 1;
    is_ordered_ = false;
    front_ = 0;
    my_array_ = new T[capacity_];
}


template <typename T>
CDA<T>::CDA(int s) {
    length_ = s;
    capacity_ = s;
    is_ordered_ = false;
    my_array_ = new T[capacity_];
}


// Copy Constructor
template <typename T>
CDA<T>::CDA(const CDA<T> &cda) {
    length_ = cda.length_;
    capacity_ = cda.capacity_;
    is_ordered_ = cda.is_ordered_;
    front_ = cda.front_;
    my_array_ = new T[capacity_];

    for (int i = 0; i < capacity_; i++) {
        my_array_[i] = cda.my_array_[i];
    }
}


// Copy Assignment Operator
template <typename T>
CDA<T>& CDA<T>::operator=(const CDA<T> &cda) {
    length_ = cda.length_;
    capacity_ = cda.capacity_;
    is_ordered_ = cda.is_ordered_;
    front_ = cda.front_;
    delete[] my_array_;
    my_array_ = new T[capacity_];

    for (int i = 0; i < cda.capacity_; i++) {
        my_array_[i] = cda.my_array_[i];
    }

    return *this;
}


template <typename T>
T& CDA<T>::operator[](int index) {
    if (index < 0 || index > length_ - 1) {
        std::cout << "Error. Index is out of bounds. " << endl;
        return throw_away_;
    }

    T* my_pointer = &my_array_[((front_ + index) % capacity_)];
    return *my_pointer;
}


template <typename T>
void CDA<T>::AddEnd(T v) {
    if (length_ == capacity_) {
        upsize();
    }
    
    if (is_ordered_) {
        if (my_array_[(front_ + length_ - 1) % capacity_] > v) {
            is_ordered_ = false;
        }
    }

    my_array_[((front_ + length_) % capacity_)] = v;
    length_++;
}


template <typename T>
void CDA<T>::AddFront(T v) {

    if (is_ordered_) {
        if (my_array_[front_] < v) {
            is_ordered_ = false;
        }
    }

    if (length_ == capacity_) {
        upsize();
    }
    if (front_ == 0) {
        front_ = capacity_ - 1;
    }
    else {
        front_--;
    }
    my_array_[front_] = v;
    length_++;

}


template <typename T>
void CDA<T>::DelEnd() {
    length_--;

    if (length_ <= (capacity_/4)) {
        downsize();
    }
}


template <typename T>
void CDA<T>::DelFront() {
    front_ = (front_ + 1) % capacity_;
    length_--;

    if (length_ <= (capacity_/4)) {
        downsize();
    }
}


template <typename T>
int CDA<T>::Length() {
    return length_;
}


template <typename T>
int CDA<T>::Capacity() {
    return capacity_;
}


template <typename T>
void CDA<T>::Clear() {
    delete[] my_array_;
    length_ = 0;
    capacity_ = 1;
    front_ = 0;
    is_ordered_ = false;
    my_array_ = new T[capacity_];
}


template <typename T>
void CDA<T>::upsize() {
    T *my_new_array = new T[capacity_ * 2];
        
    for (int i = 0; i < length_; i++) {
        my_new_array[i] = my_array_[(front_ + i) % capacity_];
    }
    capacity_ *= 2;

    delete[] my_array_;
    my_array_ = my_new_array;

    front_ = 0;
}


template <typename T>
void CDA<T>::downsize() {
    T *my_new_array = new T[capacity_ / 2];

    for (int i = 0; i < length_; i++) {
        my_new_array[i] = my_array_[(front_ + i) % capacity_];
    }
    capacity_ = capacity_ / 2;
    delete[] my_array_;
    my_array_ = my_new_array;

}


template <typename T>
bool CDA<T>::Ordered() {
    return is_ordered_;
}


template <typename T>
int CDA<T>::SetOrdered() {
   for (int i = 0; i < length_ - 1; i++) {
       if (my_array_[((front_ + i) % capacity_)] > my_array_[(((front_ + i) + 1) % capacity_)]) {
           is_ordered_ = false;
           return  -1;
       }
    }
    is_ordered_ = true;
    return 1;
}


template <typename T>
void CDA<T>::QuickSort() {
    QuickSortReal(0, length_ - 1);
    is_ordered_ = true;
} 


template <typename T>
void CDA<T>::QuickSortReal(int left, int right) {

    if (left >= right) {
        return;
    }
    if ((right - left) <= 16) {
        InsertionSortSubset(left, right);
        return;
    }

    int i = left;
    int j = right;
    T tmp;

    // Find pivot with median of 3 technique
    int low_index = (front_ + left) % capacity_;
    int mid_index = (front_ + ((left + right) / 2)) % capacity_;
    int high_index = (front_ + right) % capacity_;

    T low_pivot = my_array_[low_index];
    T mid_pivot = my_array_[mid_index];
    T high_pivot = my_array_[high_index];

    T pivot = getMedianOfThreePivot(low_pivot, mid_pivot, high_pivot);
    int pivot_index = getMedianOfThreeIndex(low_pivot, low_index, mid_pivot, mid_index, high_pivot, high_index);
 
    /* partition */
    while (i <= j) {
        while (my_array_[(front_ + i) % capacity_] < pivot)
            i++;
        while (my_array_[(front_ + j) % capacity_] > pivot)
            j--;
            if (i <= j) {
                tmp = my_array_[(front_ + i) % capacity_];
                my_array_[(front_ + i) % capacity_] = my_array_[(front_ + j) % capacity_];
                my_array_[(front_ + j) % capacity_] = tmp;
                i++;
                j--;
            }
    };
    tmp = my_array_[(front_ + i + 1) % capacity_];
    my_array_[(front_ + i + 1) % capacity_] = my_array_[pivot_index];
    my_array_[pivot_index] = tmp;

    /* recursion */
    if (left < j)
        QuickSortReal(left, j);
    if (i < right)
        QuickSortReal(i, right);
}


template <typename T>
T CDA<T>::Select(int k) {
    if (SetOrdered() == 1) {
        return my_array_[(front_ + (k - 1)) % capacity_];
    }
    else {
        return QuickSelect(k);
    }
}


template <typename T>
T CDA<T>::QuickSelect(int k) {
    return QuickSelectReal(0, length_ - 1, k - 1);
}


template <typename T>
int CDA<T>::getMedianOfThreePivot(T low, T mid, T high) {
    if (low < mid) {
        if (high < low) {
            return low;
        }
        else if (high < mid) {
            return high;
        }
        else {
            return mid;
        }
    }
    else {
        if (high < mid) {
            return mid;
        }
        else if (high < low) {
            return high;
        }
        else {
            return low;
        }
    }
}


template <typename T>
int CDA<T>::getMedianOfThreeIndex(T low, int low_index, T mid, int mid_index, T high, int high_index) {
    if (low < mid) {
        if (high < low) {
            return low_index;
        }
        else if (high < mid) {
            return high_index;
        }
        else {
            return mid_index;
        }
    }
    else {
        if (high < mid) {
            return mid_index;
        }
        else if (high < low) {
            return high_index;
        }
        else {
            return low_index;
        }
    }
}


template <typename T>
T CDA<T>::QuickSelectReal(int left, int right, int k) {
    int pivot_position;
    int i = left - 1;
    int j = right;
    T tmp;
    
    if (i == j) {
        return my_array_[(front_ + i) % capacity_];
    }

    pivot_position = left + (rand() % (right - left + 1));

    T pivot = my_array_[(front_ + pivot_position) % capacity_];
    tmp = my_array_[(front_ + pivot_position) % capacity_];

    /* partition */
    while (i <= j) {
        while (my_array_[(front_ + i) % capacity_] < pivot) {
            i++;
        }
        while (my_array_[(front_ + j) % capacity_] > pivot) {
            j--;
        }
        if (i <= j) {
                tmp = my_array_[(front_ + i) % capacity_];
                my_array_[(front_ + i) % capacity_] = my_array_[(front_ + j) % capacity_];
                my_array_[(front_ + j) % capacity_] = tmp;
                i++;
                j--;
            }
    };
    tmp = my_array_[(front_ + i + 1) % capacity_];
    my_array_[(front_ + i + 1) % capacity_] = my_array_[(front_ + pivot_position) % capacity_];
    my_array_[(front_ + pivot_position) % capacity_] = tmp;

    if (pivot_position == k) {
        return my_array_[(front_ + k) % capacity_];
    }

    // If k is less than index of current pivot, recurse on left
    if (k < pivot_position) {
        return QuickSelectReal(left, j, k);
    }
    // If k is greater than index if current pivot, recurse on right
    return QuickSelectReal(i, right, k);
} 


template <typename T>
void CDA<T>::InsertionSort() {
    int i, j;
    T key;

    for (i = 1; i < length_; i++) {
        key = my_array_[(front_ + i) % capacity_];
        j = i - 1;
        while (j >= 0 && my_array_[(front_ + j) % capacity_] > key) {
            my_array_[(front_ + j + 1) % capacity_] = my_array_[(front_ + j) % capacity_];
            j--;
        }
        my_array_[(front_ + j + 1) % capacity_] = key;
    }
    is_ordered_ = true;
}


template <typename T>
void CDA<T>::InsertionSortSubset(int low, int high) {
    int j;
    T key;

    for (int i = low; i < high; i++) {
        key = my_array_[(front_ + i) % capacity_];
        j = i - 1;
        while (j >= 0 && my_array_[(front_ + j) % capacity_] > key) {
            my_array_[(front_ + j + 1) % capacity_] = my_array_[(front_ + j) % capacity_];
            j--;
        }
        my_array_[(front_ + j + 1) % capacity_] = key;
    }
}


template <typename T>
void CDA<T>::CountingSort(int m) {

    int i;
    int count_array[m+1];
    T output_array[length_];
    int temp;

    // Initialize the count array as all 0's
    for (int i = 0; i < m+1; i++) {
        count_array[i] = 0;
    }

    // Store count of each character  
    for(i = 0; i < length_; ++i) {
        temp = int(my_array_[(front_ + i) % capacity_]);
        count_array[temp] = count_array[temp] + 1;
    }

    // Change count_array[i] so that count_array[i] now contains actual  
    // position of this character in output array  
    for (i = 1; i <= m + 1; ++i) {
        count_array[i] = count_array[i] + count_array[i-1];  
    }

    // Build the output character array  
    for (i = 0; i < length_; i++) {

        int index = (front_ + i) % capacity_;

        output_array[count_array[my_array_[index]] - 1] = my_array_[index];  
        --count_array[my_array_[(front_ + i) % capacity_]];
    }  

    // Copy the output_array to my_array_, so my_array_ is sorted
    for (i = 0; i < length_; ++i) {
        int index = (front_ + i) % capacity_;
        my_array_[index] = output_array[i];  
    }

    is_ordered_ = true;
} 


template <typename T>
int CDA<T>::Search(T e) {
    if (is_ordered_) {
        return BinarySearch(e, 0, length_ - 1);
    }

    else {
        return LinearSearch(e);
    }
}


template <typename T>
int CDA<T>::BinarySearch(T e, int left, int right) {

    if (right >= left) { 
        int middle = left + (right - left) / 2; 
  
        // If the element is present at the middle index
        if (my_array_[(front_ + middle) % capacity_] == e) {
            return middle; 
        }
  
        // If element is smaller than mid, then 
        // it must be in the left subarray
        if (my_array_[(front_ + middle) % capacity_] > e) {
            return BinarySearch(e, left, middle - 1); 
        }

        // Otherwise, the element must be somewhere
        // in right subarray 
        return BinarySearch(e, middle + 1, right); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return -1; 
}


template <typename T>
int CDA<T>::LinearSearch(T e) {
    for (int i = 0; i < length_; i++) {
        if (my_array_[(front_ + i) % capacity_] == e) {
            return i;
        }
    }
    return -1;
}


template <typename T>
CDA<T>::~CDA() {
    delete[] my_array_;
}


#endif