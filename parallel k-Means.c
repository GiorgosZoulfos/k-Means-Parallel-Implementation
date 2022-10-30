#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

//**********************************************************************
#define N  100000           // number of vectors 
#define Nv 1000             // number of elements of each vector 
#define Nc 100              // number of classes 
#define THRESHOLD 0.000001  //threshold used to check for convergence 

//**********************************************************************
float Vec[N][Nv]; 
float Center[Nc][Nv]; 
int Class[N]; 

//**********************************************************************
void setVec(void){
    int i, j ; 
    for(i=0;i<N;i++){
		for(j=0;j<Nv;j++){
			Vec[i][j] = (1.0*rand())/RAND_MAX; 
		}
	}
}

//**********************************************************************
void printVec(void){
    int i,j ; 
    for(i=0;i<N;i++){
        for(j=0;j<Nv;j++){
            printf("%f ",Vec[i][j]); 
        }
        printf("\n"); 
    }
}

//**********************************************************************
void initCenter(void){   
    int i,j; 
    for(i=0;i<Nc;i++){
        for(j=0;j<Nv;j++){
            Center[i][j] = Vec[i][j]; 
        }
    }
}

//**********************************************************************
void printCenter(void){
    int i,j ; 
    for(i=0;i<Nc;i++){
        for(j=0;j<Nv;j++){
            printf("%f ",Center[i][j]); 
        }
        printf("\n"); 
    }
    printf("\n"); 
}

//**********************************************************************
float findClass(void){
    int i,j,k;
    float temp1,s,sum = 0; 
    #pragma omp parallel for reduction(+:sum) num_threads(4) private(temp1, i, k, j, s)
    for(i=0;i<N;i++){
        s = 10000000; 
        for(k=0;k<Nc;k++){
            temp1 = 0; 
	    # pragma omp simd reduction(+:temp1)
            for(j=0;j<Nv;j++){
                temp1 += (Vec[i][j] - Center[k][j])*(Vec[i][j] - Center[k][j]); 
            } 
            if (temp1 < s){
                Class[i] = k; 
                s = temp1;  
            }
        }
        sum += s;
    }
    return sum; 
}

//**********************************************************************
void findMean(int c[Nc],float ct[Nc][Nv]){
    int i,j; 
    for(i=0;i<Nc;i++){
		for(j=0;j<Nv;j++){
			Center[i][j] = ct[i][j] / c[i];
		}
	}		
}

//**********************************************************************
void findCenter(void){
    float Cnt[Nc][Nv] = {0}; 

    int i,j,k, counter[Nc] = {0}; 
    for(k=0;k<Nc;k++){ 
        for(i=0;i<N;i++){
            if (Class[i] == k){
                for(j=0;j<Nv;j++){
                    Cnt[k][j] += Vec[i][j];  
                }
                counter[k] += 1;
            }
        }
    }
    findMean(counter,Cnt); 
}

//**********************************************************************
int main(){   
    float totDist, prevDist; 
    int i = 0; 

    srand(time(0)); 
    setVec();       // initializing the vectors 
    initCenter();   // initializing the centers 

    totDist = 1.0e30 ; 
    do{
        i += 1 ; 
        prevDist = totDist ; 
        totDist = findClass(); 
        findCenter(); 
        printf("Iteration = %d  Error = %.10f\n",i,(prevDist - totDist)/totDist);
    }while((prevDist - totDist)/totDist >THRESHOLD && i<16) ; 
    
    return 0 ; 
}
