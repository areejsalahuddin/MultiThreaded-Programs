#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

// matrix size max->10 
int matrix1_rows,shared,matrix2_columns; // number of rows and coloumns of matrices 1 and 2 
int **matrix1;
int **matrix2;
int **multiplied_matrix_element;
int **multiplied_matrix_row;


struct args {
int rowIndex;
int columnIndex;
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
  fscanf(p_input,"%d" ,&matrix1_rows); 
  fscanf(p_input,"%d" ,&shared); 
  matrix1 = (int **) malloc(matrix1_rows * sizeof(int *));
  for(int row = 0; row<matrix1_rows; row++){
  matrix1[row] = (int *) malloc(shared * sizeof(int *));
     for (int column = 0 ; column <shared; column++){
     fscanf(p_input,"%d " ,&matrix1[row][column]); 
     }
   }
 
  fscanf(p_input,"%d" ,&shared); 
  fscanf(p_input,"%d" ,&matrix2_columns);
   matrix2 = (int **) malloc(shared * sizeof(int *));
  for(int row = 0; row<shared; row++){
   matrix2[row] = (int *) malloc(matrix2_columns * sizeof(int *));
     for (int column = 0 ; column <matrix2_columns; column++){
     fscanf(p_input,"%d " ,&matrix2[row][column]); 
     }
   }
   
 }
 
 multiplied_matrix_element = (int **) malloc(shared * sizeof(int *));
 for(int row = 0; row<shared; row++){
   multiplied_matrix_element[row] = (int *) malloc(matrix2_columns * sizeof(int *));
 }
 
 multiplied_matrix_row = (int **) malloc(shared * sizeof(int *));
 for(int row = 0; row<shared; row++){
   multiplied_matrix_row[row] = (int *) malloc(matrix2_columns * sizeof(int *));
 }
 fclose(p_input);
}



void *multiply_byELement(void *arg){
struct args *p = (struct args*) arg;
int rowI = p->rowIndex;
int columnI = p->columnIndex;

for(int position=0; position <shared; position++){
multiplied_matrix_element[rowI][columnI] += matrix1[rowI][position] * matrix2[position][columnI];

}



}

void byElement(){
pthread_t threads[matrix1_rows*matrix2_columns];
int thread_no = 0;
 
struct args *a = malloc(sizeof(struct args) * matrix1_rows*matrix2_columns);
for  (int x=0; x<matrix1_rows; x++){
   for(int y=0; y<matrix2_columns; y++) {
   
   a[thread_no].rowIndex = x;
   a[thread_no].columnIndex = y;
   
  pthread_create(&threads[thread_no], NULL, &multiply_byELement,(void *) &a[thread_no]); 
  

  thread_no++; 
   
   }
}

for (int p=0 ; p< matrix1_rows * matrix2_columns ; p++){
pthread_join(threads[p],NULL);
}




}



void *multiply_byRow(void *arg){
struct args *p = (struct args*) arg;

int row =  p->rowIndex;

for(int i=0; i <matrix2_columns; i++){
for(int j=0; j<shared ; j++){
multiplied_matrix_row[row][i] += matrix1[row][j] * matrix2[j][i];

     }
  }
}



void byRow(){
pthread_t threads[matrix1_rows];
struct args *a = malloc(sizeof(struct args) * matrix1_rows);
// thread_no = number of rows in output matrix
int thread_no =0;
for (int x=0; x<matrix1_rows; x++){
   a[thread_no].rowIndex = x;
pthread_create(&threads[thread_no], NULL, &multiply_byRow,(void *) &a[thread_no]);  //always put & before the function 
  thread_no++;
    }

for (int p=0 ; p< matrix1_rows ; p++){
pthread_join(threads[p],NULL);
   }

}





int main (){
load();


clock_t begin, end;
begin = clock();
byElement();
end = clock(); 

printf("OUTPUT\n");
for  (int row=0; row<matrix1_rows; row++){
   for(int column=0; column<matrix2_columns; column++) {
   printf("%d\t",multiplied_matrix_element[row][column]);
}
printf("\n");
}

double time_byELement = (double) (end - begin) / CLOCKS_PER_SEC;

printf("END1 [%lf]\n",time_byELement);



begin= clock();
byRow();
end = clock();

printf("OUTPUT\n");
for  (int row=0; row<matrix1_rows; row++){
   for(int column=0; column<matrix2_columns; column++) {
   printf("%d\t",multiplied_matrix_row[row][column]);
}
printf("\n");
}


double time_byRow = (double) (end - begin) / CLOCKS_PER_SEC;

printf("END2 [%lf]\n",time_byRow);

return 0;
}


