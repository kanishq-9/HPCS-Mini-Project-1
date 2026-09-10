#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define HIGH 1000000

void writeToFile(const char *filename, int *arr, int n){
FILE *f=fopen(filename,"w");
if(!f){
perror("fopen");
exit(1);
}

for(int i=0;i<n;i++){
fprintf(f,"%d",arr[i]);
if((i+1)%10==0||i==n-1)fprintf(f,"\n");
else fprintf(f,",");
}
fclose(f);
}

void generateUniform(const char *filename, int n){
int *arr=malloc(n*sizeof(int));
for(int i=0;i<n;i++)
arr[i]=1+rand()%HIGH;
writeToFile(filename,arr,n);
free(arr);
}


int zipfSample(double alpha, int high){
double u=(double)rand()/RAND_MAX;
double val=pow(u,-1.0/alpha);
int result =(int)val;
if(result<1)result=1;
if(result>high)result=high;
return result;
}


void generateSkewed(const char *filename, int n){
int *arr=malloc(n*sizeof(int));
double alpha=2.0;
for(int i=0;i<n;i++)
arr[i]=zipfSample(alpha,HIGH);

writeToFile(filename, arr, n);
free(arr);
}

void generateNearlySorted(const char *filename, int n){
int *arr=malloc(n*sizeof(int));
for(int i=0;i<n;i++)
arr[i]=i+1;

int numSwaps = n/10;
for(int k=0;k<numSwaps;k++){
int i=rand()%n;
int j=rand()%n;
int tmp=arr[i];
arr[i]=arr[j];
arr[j]=tmp;
}
writeToFile(filename,arr,n);
free(arr);
}

int main(int argc, char *argv[]) {
if(argc<2){
fprintf(stderr,"Usage: %s <numElements>\n",argv[0]);
return 1;
}
int n=atoi(argv[1]);
srand(time(NULL));

generateUniform("uniform.txt",n);
printf("Generated uniform.txt with %d elements\n",n);

generateSkewed("skewed.txt",n);
printf("Generated skewed.txt with %d elements\n",n);

generateNearlySorted("nearly_sorted.txt",n);
printf("Generated nearly_sorted.txt with %d elements\n",n);

return 0;
}
