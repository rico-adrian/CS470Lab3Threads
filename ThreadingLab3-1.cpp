#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <mutex>
#include <thread>
#include <cstdlib>

#define QUICK 0
#define INSERT 1
#define BUBBLE 2
int **myM;
int M;
int counter;
int N;
int error;
bool valid = false;
void binFile(FILE * myFile);
void calcMet(FILE* myFile);
void *matrix(void *arg);
void *sorting(void *arg);
void bubbleSort(int arr[], int n);
void insertionSort(int arr[], int n);
void quickSort(int arr[], int low, int high);
bool sorted(int arr[]);

using namespace std;

bool AllZerosOrOnes() {
	int allZeros = 0;
	int allOnes = 0;
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < N;j++) {
			if (myM[i][j] == 0) {
				allZeros++;
			}
			else if (myM[i][j] == 1) {
				allOnes++;
			}
		}
	}
	if (allZeros == (N*N) || allOnes == (N*N)) {
		return true;
	}
	return false;
}

int main(int argc, char **argv) {
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	N = atoi(argv[1]);
	cout<<"Please input a number of M "<<endl;
	int M;
	cin>>M;
	srand(time(NULL));
	while (N <= 2 || M <= 0) {
		cout << "Please enter a new value of N and M" << endl;
		cout << "new N= ";
		cin >> N;
		cout << "new M= ";
		cin >> M;
	}
	FILE * fileName;
	if ((fileName = fopen("matrix.bin", "w+")) == NULL) {
		printf("Error in opening file: %s\n", "matrix.bin");
		exit(-1);
	}
	else {
		fileName = fopen("matrix.bin", "w+");
	}
	cout << "What type of program do you want?" << endl;
	cout << "press 1 for problem 1, press 2 for problem 2 and press 3 for exit" << endl;
	int number;
	cin >> number;

	if (number > 3 || number < 1) {
		cout << "invalid number, please enter only 1 through 3" << endl;
		int number;
		cin >> number;
	}

	FILE * myFile;
	if (fileName != NULL) {
		myFile = fileName;
	}
	cout << "Your N value =  " << N <<
		", your M value = " << M << " within your file matrix.bin or sorting.txt" << endl;
	myM = new int*[N];

	for (int j = 0; j < N; j++) {
		myM[j] = new int[N];
	}
	if (number == 1) {
		pthread_t threads[M];
		for (int i = 0; i < M; i++) {
			if (error = pthread_create(&threads[i], 0, matrix, (void *)&i)) {
				cout << "error" << endl;
				exit(1);
			}
		}

		for (int i = 0; i < M; i++) {
			pthread_join(threads[i], NULL);
		}
	}
	else if (number == 2) {
		pthread_t threads[M];
		for (int i = 0; i < M; i++) {
			if (error = pthread_create(&threads[i], 0, sorting, (void *)&i)) {
				cout << "error" << endl;
				exit(1);
			}
		}

		for (int i = 0; i < M; i++) {
			pthread_join(threads[i], NULL);
		}
	}
	else if (number == 3) {
		exit(EXIT_FAILURE);
	}
	cout << "new matrix" << endl;
	fclose(myFile);

	if (ferror(myFile)) {
		perror("Error reading test.bin");
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);

	return 0;
}

void *matrix(void *arg) {
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int counter0 = 0;
	int counter1 = 0;
	int counterChanges = 0;
	bool b = AllZerosOrOnes();
	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
			myM[j][k] = rand() % 2;
			fwrite(&myM, sizeof(int), N*N, fopen("matrix.bin", "wb"));
		}
	}
	fclose(fopen("matrix.bin", "wb"));
	while (b == false) {
		pthread_mutex_lock(&mutex);
		int k = rand() % N;
		int z = rand() % N;
		for (int i = -1;i <= 1;i++) {
			for (int j = -1;j <= 1;j++) {
				if (((z - j) >= 0) && ((z - j) <= (N - 1))) {
					if (((k - i) >= 0) && ((k - i) <= (N - 1))) {
						if (!(i == 0 && j == 0)) {
							if ((myM[k - i][z - j]) == 0) {
								counter0++;
							}
							else if ((myM[k - i][z - j]) == 1) {
								counter1++;
							}
						}
					}
				}
			}
		}
		if (counter0 > counter1) {
			myM[k][z] = 0;
		}
		else if (counter1 > counter0) {
			myM[k][z] = 1;
		}
		else {
			if (myM[k][z] == 1) {
				myM[k][z] = 0;
			}
			else {
				myM[k][z] = 1;
			}
		}
		counterChanges++;
		cout << endl;
		counter0 = 0;
		counter1 = 0;

		for (int n = 0; n < N; n++) {
			for (int k = 0;k < N;k++) {
				printf("%d ", myM[n][k]);
			}
			cout << "\n";
		}
		b = AllZerosOrOnes();
		
		//error for half 0s and half 1s		
		if (counterChanges == 3500) {
			cout << "changes is finished, nothing else can change" << endl;
			b = true;
		}
		if (b == true) {
			pthread_exit(NULL);
			exit(0);
		}
		fread(&myM, sizeof(int), N*N, fopen("matrix.bin", "rb"));
		pthread_mutex_unlock(&mutex);
	}
	fclose(fopen("matrix.bin", "rb"));
}
void *sorting(void *arg) {
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int arr[N*N];
	FILE* file;
	srand(time(NULL));
	file = fopen("sorting.txt", "w");
	for (int j = 0; j < (N*N); j++) {	
		arr[j] = rand() % 100;
		fprintf(file, "%d ", arr[j]);
	}
	cout << "asfsagsbsb";
	fclose(file);
	FILE *readMe = fopen("sorting.txt", "r");
	int arr1[N*N];
	for (int i = 0;i < N*N;i++) {
		fscanf(readMe, "%d", &arr1[i]);
	}
	fclose(file);
	cout << endl;
	for (int i = 0; i < (N*N); i++)
	{
		cout << arr1[i] << " ";
	}

	while(sorted(arr1)==false) {
		pthread_mutex_lock(&mutex);
		int k = rand() % (N*N);
		int z = rand() % (N*N);
		cout << k << endl;
		cout << z << endl;
		int j = 0;
		int size;
		if (k <= z) {
			size = z - k;
		}
		else {
			size = k - z;
		}

		int temp[size];
		if (k <= z) {
			for (int i = k;i <=z;i++) {
				temp[j] = arr1[i];
				j++;
				//cout << temp[j] << " ";
			}
		}
		else if (k > z) {
			for (int i = z;i <= k;i++) {
				temp[j] = arr1[i];
				j++;
				//cout << temp[j] << " ";
			}
		}
		cout << endl;
		int randomSort = (rand() % 3) + 1;

		if (randomSort == QUICK) {
			if (k <= z) {
				quickSort(temp, k, z);
			}
			else if (k > z) {
				quickSort(temp, z, k);
			}
		}
		else if (randomSort == INSERT) {
				insertionSort(temp, size+1);		
		}
		else if (randomSort == BUBBLE) {
			bubbleSort(temp, size+1);
		}

		
		int l = 0;
		cout << "changes number " << l << endl;
		l++;
		FILE *writeOpen = fopen("sorting.txt", "w");
		if (sorted(arr1) == true) {
		
			for (int j = 0; j < (N*N); j++) {
				fprintf(writeOpen, "%d ",arr1[j]);
			}
			fclose(writeOpen);
			pthread_exit(NULL);
			exit(0);
		}
		else {
			int q = 0;
			if (k <= z) {
				for (int i = k;i <= z;i++) {
					arr1[i] = temp[q];
					q++;
					cout << arr1[j] << " ";
				}
			}
			else if (k > z) {
				for (int i = z;i <= k;i++) {
					arr1[i] = temp[q];
					q++;
					cout << &arr1[j] << " ";
				}
			}
			FILE *asddd = fopen("sorting.txt", "w");
			for (int j = 0; j <(N*N); j++) {
				fprintf(asddd, "%d ", arr1[j]);
			}
			fclose(asddd);
		}
		cout << endl;
		pthread_mutex_unlock(&mutex);
	}
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

bool sorted(int arr[]) {
		for (int i = 1;i < (N*N);i++) {
			if (arr[i-1] > arr[i]) {
				return false;
			}
		}
		cout<<"aaaa";
		return true;
}

int partition(int arr[], int low, int high)
{
	int pivot = arr[high];    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

void insertionSort(int arr[], int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		/* Move elements of arr[0..i-1], that are
		greater than key, to one position ahead
		of their current position */
		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}


// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
		// Last i elements are already in place   
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}