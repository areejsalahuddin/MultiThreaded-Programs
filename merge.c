#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int size;
int *array;

struct merge_sort_args{
int FirstElement;
int LastElement;
};



void load(){
FILE *p_input;
char filename[50]="";
printf("Please Enter Filename: ");
scanf("%s",filename);
p_input = fopen(filename,"r");
if(p_input==NULL){
printf("Can't read from file\n");
exit(0);
}

else {
fscanf(p_input,"%d" ,&size); 
array = (int*) malloc(sizeof(int) *size);
for(int i = 0; i<size; i++){
fscanf(p_input,"%d " ,&array[i]); 
  }
  }
  fclose(p_input);
}




void merge(int left, int mid, int right) {

    int LSize = mid - left + 1;
    int RSize = right - mid;

    int leftArray[LSize];
    int rightArray[RSize];
    


    for (int i = 0; i < LSize; i++)
        leftArray[i] = array[left + i];
    for (int j = 0; j < RSize; j++)
        rightArray[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < LSize && j < RSize) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }

        k++;
    }

    while (i < LSize) {
        array[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < RSize) {
        array[k] = rightArray[j];
        j++;
        k++;
    }


}




void *mergesort(void *arg){

struct merge_sort_args *args = (struct merge_sort_args*) arg;

int FirstElementI = args->FirstElement;
int LastElementI = args->LastElement;



if(FirstElementI < LastElementI){
int mid = FirstElementI +  (LastElementI - FirstElementI) / 2; 
pthread_t threads[2];
struct merge_sort_args left;  
left.FirstElement = FirstElementI;
left.LastElement = mid; 

struct merge_sort_args right;
right.FirstElement = mid +1 ;
right.LastElement = LastElementI;
pthread_create(&threads[0],NULL,&mergesort,&left);

 
pthread_create(&threads[1],NULL,&mergesort,&right);
 

pthread_join(threads[0],NULL);

pthread_join(threads[1],NULL);

  merge(args->FirstElement , mid , args->LastElement);
  // printA();
   
}


}




void printA(){
for(int i = 0; i<size; i++){
  printf("%d ", array[i]);
   
  }
  printf("\n");
  
}




int main(){
load();
struct merge_sort_args args;

args.FirstElement = 0;
args.LastElement = size -1; 


pthread_t main_thread; 
pthread_create(&main_thread, NULL, &mergesort,(void *) &args);
pthread_join(main_thread,NULL);
printA();
 
 
  

return 0;
}


 
