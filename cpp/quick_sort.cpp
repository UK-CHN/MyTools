#include<iostream>
#include<cstdlib>
 
using namespace std;
 
void Swap(int *a, int *b)
{
	int tmp; 
	tmp = *a;
	*a = *b;
	*b = tmp;
}
 
int Partition(int a[], int l, int h)
{
	int pivot, index, i;
	index = l;
	pivot = h;
 
	for(i=l; i < h; i++)
	{
		if(a[i] < a[pivot])
		{
			Swap(&a[i], &a[index]);
			index++;
		}
	}
	Swap(&a[pivot], &a[index]);
 
	return index;
}
 
int RandomPivotPartition(int a[], int l, int h)
{
	int pvt, n, tmp;
	n = rand();

	pvt = l + n%(h-l+1);
 
	Swap(&a[h], &a[pvt]);
 
	return Partition(a, l, h);
}
 
int QuickSort(int a[], int l, int h)
{
	int pindex;
	if(l < h)
	{
		pindex = RandomPivotPartition(a, l, h);
		QuickSort(a, l, pindex-1);
		QuickSort(a, pindex+1, h);
	}
	return 0;
}
 
int main()
{
	int n, i;
	cout<<"\nEnter the number of data element to be sorted: ";
	cin>>n;
 
	int *arr = new int[n];
	for(i = 0; i < n; i++)
	{
		cout<<"Enter element "<<i+1<<": ";
		cin>>arr[i];
	}
 
	QuickSort(arr, 0, n-1);
 
	// Printing the sorted data.
	cout<<endl << "Sorted Data " << endl ;

	for (i = 0; i < n; i++)
        	cout<<i+1 << ")" << arr[i] << endl ;
 
	delete arr;
	return 0;
}
