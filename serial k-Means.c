#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//**********************************************************************
#define N  100000           // number of vectors 
#define Nv 1000             // number of elements of each vector 
#define Nc 100              // number of classes 
#define THRESHOLD 0.000001  //threshold for convergence 

//**********************************************************************
#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline","unsafe-math-optimizations")
#pragma GCC option("arch-native","tune-native","n0-zero-upper")

//**********************************************************************
float Vec[N][Nv]; 
float Center[Nc][Nv]; 
int Class[N]; 

//**********************************************************************
void setVec(void){
    int i, j; 
    for(i=0;i<N;i++){
		for(j=0;j<Nv;j++){
			Vec[i][j] = (1.0*rand())/RAND_MAX; 
		}
	}
}

//**********************************************************************
void firstCenters(void){   

    int i,j; 
	
    for(i=0;i<Nc;i++){
        for(j=0;j<Nv;j++){
            Center[i][j] = Vec[i][j]; 
        }
    }
	
}

//**********************************************************************
float findClass(void){
	
    int i,j,k;
    float dist,temp,sum = 0;  
	
    for(i=0;i<N;i++){
        temp = 10000000; 
        for(k=0;k<Nc;k++){
            dist = 0; 
            for(j=0;j<Nv;j++){
                dist += (Vec[i][j] - Center[k][j])*(Vec[i][j] - Center[k][j]) ; 
            } 
            if (dist < temp){
                Class[i] = k ; 
                temp = dist ;  
            }
        }
        sum += temp; 
    }
    return sum; 
	
}

//**********************************************************************
void findMean(int c1[Nc],float c2[Nc][Nv]){
	
    int i,j; 
	
    for(i=0;i<Nc;i++){
		for(j=0;j<Nv;j++){
			Center[i][j] = c2[i][j] / c1[i]; 
		}
	}
	
}

//**********************************************************************
void findCenter(void){
	
	int i,j,k, counter1[Nc] = {0}; 
    float counter2[Nc][Nv] = {0}; 
	
    for(k=0;k<Nc;k++){ 
        for(i=0;i<N;i++){
            if (Class[i] == k){
                for(j=0;j<Nv;j++){
                    counter2[k][j] += Vec[i][j];  
                }
                counter1[k] += 1;
            }
        }
    }
    findMean(counter1,counter2); 
}

//**********************************************************************
int main(){   
    float totalDistance = 1.0e30;
	float previousDistance; 
    int i = 0; 

    srand(time(0)); 
    setVec();       
    initCenter();   

    do{
        i += 1; 
        previousDistance = totalDistance ; 
        totalDistance = findClass(); 
        findCenter(); 
        printf("Iteration number = %d  Error = %.12f\n",i,(previousDistance - totalDistance)/totalDistance);
    }while((previousDistance - totalDistance)/totalDistance >THRESHOLD) ; 
    
    return 0; 
}