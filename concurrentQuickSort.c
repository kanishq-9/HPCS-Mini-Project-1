#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include<string.h>

#define CUTOFF 1000

void insertionSort(int *arr, int low, int high){
for(int i=low+1;i<=high;i++){
int key=arr[i];
int j=i-1;
while(j>=low && arr[j]>key){
arr[j+1]=arr[j];
j-=1;
}
arr[j+1]=key;
}
}

int partition(int *arr, int low, int high){
int pivotIndex = low+rand()%(high-low+1);
int pivot = arr[pivotIndex];
int temp = arr[pivotIndex];
arr[pivotIndex]=arr[high];
arr[high]=temp;

int i=low-1;
for(int j=low;j<high;j++){
if(arr[j]<pivot){
i++;
temp = arr[i];
arr[i]=arr[j];
arr[j]=temp;
}
}
temp=arr[i+1];
arr[i+1]=arr[high];
arr[high]=temp;
return i+1;
}


void quickSort(int *arr, int low, int high){
if(high-low<CUTOFF){
insertionSort(arr, low, high);
return;
}
if(low<high){
int p=partition(arr, low, high);
quickSort(arr, low, p-1);
quickSort(arr, p+1, high);
}
}

void merge(int *arr, int low, int mid, int high){
int n1=mid-low+1;
int n2=high-mid;
int *L=malloc(n1*sizeof(int));
int *R=malloc(n1*sizeof(int));
for(int i=0;i<n1;i++) L[i]=arr[low+i];
for(int j=0;j<n2;j++) R[j]=arr[mid+1+j];

int i=0,j=0,k=low;

while(i<n1 && j<n2)arr[k++]=(L[i]<=R[j])?L[i++]:R[j++];

while(i<n1)arr[k++]=L[i++];
while(j<n2)arr[k++]=R[j++];

free(L);
free(R);
}



int countElements(const char *filename){
FILE *f = fopen(filename, "r");
if(!f){
perror("fopen");
exit(1);
}
char line[256];
int count = 0;
while(fgets(line,sizeof(line),f)){
char *tok = strtok(line, ",\n");
while(tok){
count++;
tok=strtok(NULL,",\n");
}
}
fclose(f);
return count;
}

void readElements(const char *filename, int *arr){
FILE *f = fopen(filename, "r");
if(!f){
perror("fopen");
exit(1);
}
char line[256];
int i=0;
while(fgets(line, sizeof(line), f)){
char *tok = strtok(line, ",\n");
while(tok){
arr[i++] = atoi(tok);
tok = strtok(NULL, ",\n");
}
}
fclose(f);
}

void writeElements(const char *filename, int *arr, int n){
FILE *f = fopen(filename, "w");
if(!f){
perror("fopen");
exit(1);
}

for(int i=0; i<n;i++){
fprintf(f,"%d",arr[i]);
if((i+1)%10==0 || i==n-1)
fprintf(f,"\n");
else
fprintf(f,"\n");
}
fclose(f);
}



int main(int argc, char *argv[]){
if(argc<4){
fprintf(stderr, "Usage: %s <input file> <output file> <num of threads>\n", argv[0]);
return 1;
}

const char *inputFile = argv[1];
const char *outputFile = argv[2];
int numThreads = atoi(argv[3]);

int n = countElements(inputFile);
int *arr = malloc(n*sizeof(int));

readElements(inputFile, arr);
omp_set_num_threads(numThreads);

int *chunkStart = malloc(numThreads*sizeof(int));
int *chunkEnd = malloc(numThreads*sizeof(int));

int base=n/numThreads;
int rem=n%numThreads;
int offset = 0;

for(int i=0;i<numThreads;i++){
int sze = base+(i<rem?1:0);
chunkStart[i]=offset;
chunkEnd[i]=offset+sze-1;
offset+=sze;
}

double startTime = omp_get_wtime();

#pragma omp parallel for schedule(static)
for(int i=0;i<numThreads;i++){
if(chunkStart[i]<=chunkEnd[i])
quickSort(arr, chunkStart[i], chunkEnd[i]);
}
int numChunks = numThreads;
int *start = chunkStart;
int *end = chunkEnd;

while(numChunks>1){
int newCount = (numChunks+1)/2;
for(int i=0;i<numChunks/2;i++){
int low=start[2*i];
int mid=end[2*i];
int high=end[2*i+1];
merge(arr, low, mid, high);
start[i]=low;
end[i]=high;
}
if(numChunks%2==1){
start[newCount-1]=start[numChunks-1];
end[newCount-1]=end[numChunks-1];
}
numChunks=newCount;
}
double endTime=omp_get_wtime();
printf("Sorted %d elements using %d threads in %f seconds\n",n,numThreads, endTime-startTime);

writeElements(outputFile, arr,n);

free(arr);
free(chunkStart);
free(chunkEnd);
return 0;
}
