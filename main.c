#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> 
#include <pthread.h> 
#include<string.h>

int rowa,cola,rowb,colb;
int a[20][20];
int b[20][20];
int c[20][20];
int control=0;
char fileout[500];

typedef struct
{
  int i;
  int j;
}point;

void read_file(char  filename1[],char  filename2[]);
void writeToFile(char * filename,struct timeval start,struct timeval stop,int type);
void mult1();
void mult2();
void mult3();
void *threadfunc1(void* l);
void *threadfunc2(void* l);
void *threadfunc3(void* l);


int main(int argc, char *argv[])
{
    
 if(argc!=1)
 {
    
   char file1[500];
   char file2[500];
   int i=0;
   while(argv[1][i]!='\0')
   { 
    printf("%c\n",argv[1][i]);
     file1[i]=argv[1][i];
     printf("%d",i);
     i++;
   }file1[i]='\0';
   i=0;
   while(argv[2][i]!='\0')
   {
    //printf("dcxd");
     printf("%c\n",argv[2][i]);
     file2[i]=argv[2][i];
     i++;
   }
   file2[i]='\0';
    strcat(file1,".txt");
    strcat(file2,".txt");
    read_file(file1,file2);
    control=1;
     i=0;
    while(argv[3][i]!='\0')
    {
         printf("%c\n",argv[3][i]);
        //printf("dfsdf");
        fileout[i]=argv[3][i];
        i++;
    }
    fileout[i]='\0';

    
 }else
 {
    
    read_file("a.txt","b.txt");
    control=0;
 }
 
 mult1();
 mult2();
 mult3();
 


   return 0;    
}

void read_file(char  filename1[],char  filename2[])
{
    FILE *fptr;
    printf("%s",filename1);
fptr = fopen(filename1, "r");
fscanf(fptr, "row=%d col=%d", &rowa, &cola);
 for (int i = 0; i < rowa; i++) {
        for (int j = 0; j < cola; j++) {
            fscanf(fptr, "%d", &a[i][j]);
            
        }
    }

// Close the file
fclose(fptr);


fptr = fopen(filename2, "r");


fscanf(fptr, "row=%d col=%d", &rowb, &colb);
 for (int i = 0; i < rowb; i++) {
        for (int j = 0; j < colb; j++) {
            fscanf(fptr, "%d", &b[i][j]);
        }
    }

// Close the file
fclose(fptr);
 

}

void mult1()
{

       struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
   

     pthread_t th;
    pthread_create(&th, NULL, threadfunc1, NULL); 
     
    gettimeofday(&stop, NULL); //end checking time

   if(control){
        char filename[500];
        int i=0;
        while(fileout[i]!='\0')
        {
            filename[i]=fileout[i];
            i++;
        }
        filename[i]='\0';
        strcat(filename,"_per_matrix.txt");
     writeToFile(filename,start,stop,3);
     }else{
           writeToFile("c_per_matrix.txt",start,stop,3);
     }

   

}
void *threadfunc1(void *  l)
{
    for (int i = 0; i < rowa; i++) {
        for (int j = 0; j < colb; j++) {
            c[i][j] = 0;
            for (int k = 0; k < cola; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}


void mult2()
{

       struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
   
    pthread_t th[rowa];   
    for (int i = 0; i < rowa; i++) {
        int *v = (int *)malloc(sizeof(int));
         *v=i;
         pthread_create(&th[*v], NULL, threadfunc2, (void *)v);     
    }
    pthread_join(th[rowa-1], NULL); 
    
    gettimeofday(&stop, NULL); //end checking time
    if(control){
        char filename[500];
        int i=0;
        while(fileout[i]!='\0')
        {
            filename[i]=fileout[i];
            i++;
        }
        filename[i]='\0';
        strcat(filename,"_per_row.txt");
     writeToFile(filename,start,stop,3);
     }else{
           writeToFile("c_per_row.txt",start,stop,3);
     }

   
}


void *threadfunc2(void* l)
{
   //
    int i= *( int *)l;
   
   
    for (int j = 0; j < colb; j++) {
            c[i][j] = 0;
            for (int k = 0; k < cola; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
            printf("(%d,%d)==>%d\n",i,j,c[i][j]);
            
           
        }
}


void mult3()
{

       struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
   
    pthread_t th[rowa][colb];
    point x[rowa][colb];  
    for (int i = 0; i < rowa; i++) {
          
        for (int j = 0; j < colb; j++) {
            x[i][j].i=i;
            x[i][j].j=j;
            c[i][j] = 0;
          pthread_create(&th[i][j], NULL, threadfunc3, (void *)&x[i][j]); 
        }
          
    }
    pthread_join(th[rowa-1][colb-1], NULL); 
    
    gettimeofday(&stop, NULL); //end checking time
     if(control){
        char filename[500];
        int i=0;
        while(fileout[i]!='\0')
        {
            filename[i]=fileout[i];
            i++;
        }
        filename[i]='\0';
        strcat(filename,"_per_element.txt");
     writeToFile(filename,start,stop,3);
     }else{
           writeToFile("c_per_element.txt",start,stop,3);
     }


   
}




void *threadfunc3(void* l)
{
    point *x=(point *)l;
    int i=x->i;
    int j=x->j;

            for (int k = 0; k < cola; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }        
}

void writeToFile(char * filename,struct timeval start,struct timeval stop,int type){
     
 FILE *file = fopen(filename, "w"); // Open file for writing
    if (file == NULL) {
        printf("Error creating file!\n");
        return;
    }
    switch (type)
    {
    case 1:
        fprintf(file," Method: A thread per matrix\n");
        break;
    case 2:
     fprintf(file," Method:  A thread per row\n");
        break;
    case 3:
     fprintf(file," Method: A thread per element\n");
        break;
    }
    
    fprintf(file,"Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    fprintf(file,"Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    // Write matrix to file
    for (int i = 0; i < rowa; i++) {
        for (int j = 0; j < colb; j++) {
            fprintf(file, "%d\t", c[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file); // Close the file
}
