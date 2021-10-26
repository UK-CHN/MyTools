#include <iostream>
 
using namespace std;
 
void Merge(int *a, int l, int h, int m)
{
	int i, j, k, tmp[h-l+1];
	i = l;
	k = 0;
	j = m + 1;
 
	while (i <= m && j <= h)
	{
		if (a[i] < a[j])
		{
			tmp[k] = a[i];
			k++;
			i++;
		}
		else
		{
			tmp[k] = a[j];
			k++;
			j++;
		}
	}
 
	while (i <= m)
	{
		tmp[k] = a[i];
		k++;
		i++;
	}
 
	while (j <= h)
	{
		tmp[k] = a[j];
		k++;
		j++;
	}
 
 
	for (i = l; i <= h; i++)
	{
		a[i] = tmp[i-l];
	}
}
 
void MergeSort(int *a, int l, int h)
{
	int mid;
	if (l < h)
	{
		mid=(l+h)/2;
		MergeSort(a, l, mid);
		MergeSort(a, mid+1, h);
 
		Merge(a, l, h, mid);
	}
}
 
int main()
{
	int n, i;
	cout<<"\nEnter the number of elements : ";
	cin>>n;
 
	int * arr = new int[n];
	for(i = 0; i < n; i++)
	{
		cout<<"Enter element "<<i+1<<": ";
		cin>>arr[i];
	}
 
	MergeSort(arr, 0, n-1);
 
	cout<<endl << "Sorted Data " << endl ;

	for (i = 0; i < n; i++)
        	cout<<i+1 << ")" << arr[i] << endl ;
 
	delete arr;
	return 0;
}
