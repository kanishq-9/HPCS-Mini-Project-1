#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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
if(argc<3){
fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
return 1;
}

const char *inputFile = argv[1];
const char *outputFile = argv[2];

int n = countElements(inputFile);
int *arr = malloc(n*sizeof(int));

readElements(inputFile, arr);
clock_t startTime=clock();

quickSort(arr,0, n-1);

clock_t endTime=clock();
double elapsedTime=(double)(endTime-startTime)/CLOCKS_PER_SEC;
printf("Sorted %d elements sequentially in %f seconds\n",n, elapsedTime);
writeElements(outputFile, arr,n);

free(arr);
return 0;
}
