# Time Series Classification
Classification of time series using Recursion and Dynamic Programming. 

The program gets trained with datasets which belong to classes and the program predicts to which classes other datasets belong. It also prints error rate and CPU time.

### Compilation
For implementation using Recursion:

`$ gcc functions.c time_series_classification.c -o dtwrec`

For implementation using Dynamic Programming:

`$ gcc -DREC functions.c time_series_classification.c -o dtwdp`

### Usage
`$ ./cat trainfile testfile | ./dtwrec limit > output`

`$ ./cat trainfile testfile | ./dtwdp limit > output`

Examples with included train and test files:

`$ ./cat train_FaceFour.txt test_FaceFour.txt | ./dtwdp 7`

`$./cat train_ECG5000.txt test_ECG5000.txt | ./dtwdp 45 > /dev/null`
