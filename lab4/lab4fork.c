//总要求：
//编写一个调用fork的程序，在调用fork前初始化矩阵A、B、C，然后父子进程都调用naive gemm 完成矩阵乘实现C=A*B+C。记录以下数据：
//a. 执行程序，查看进程对应CPU的利用率
//b. 用pstree查看进程的父子关系
//c. 用time测量程序的运行，记录cpu时间和时钟时间
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "time.h"

#define M 3
#define N 3
#define K 2


void dgemm(int m, int n, int k, int beta,
          double A[][k], double B[][n], double C[][n]){
    for(int i=0; i< m;i ++){    //C[i] 
        for(int j=0; j< n; j++){  //C[i][j]
            C[i][j] = beta*C[i][j];
            for(int p=0; p< k; p++){  
                C[i][j] += A[i][p]*B[p][j]; 
             }
        }
    }
}

void printf_matrix(int row, int col, double matrix[row][col] ){
  for(int i=0; i<row; i++){
    for(int j=0; j<col;j++){
        printf("%lf ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n\n");
}


void main()
{
    double A[M][K] = { {1.0,  -3.0},
                      {2.0,  4.0},
                      {1.0, -1.0} };         
    double B[K][N] = { {1.0,  2.0,  1.0},
                      {-3.0, 4.0, -1.0} };  
    double C[M][N] = { {.5, .5, .5}, 
                      {.5, .5, .5},
                      {.5, .5, .5} }; 

    printf_matrix(M,K,A);
    printf_matrix(K,N,B);
    int rc = fork();
    if (rc < 0)
    {//fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {//child (new process)
        printf("hello, I am a child (pid:%d)\n", (int) getpid());
        // 记录时钟时间  
        clock_t start_clock, end_clock;  
        double cpu_time_used;  
        // 记录墙钟时间  
        time_t start_time, end_time;  
        double elapsed_time;  

        // 开始时钟时间  
        start_clock = clock();  
        // 开始墙钟时间  
        time(&start_time);   

        //C=A*B + beta*C
        dgemm(M,N,K,2, A,B,C);
        printf_matrix(M,N,C);
        // 结束时钟时间  
        end_clock = clock();
        // 结束墙钟时间  
        time(&end_time);  

        // 计算CPU时间  
        cpu_time_used = ((double) (end_clock - start_clock)) / CLOCKS_PER_SEC;  
        printf("CPU时间: %f秒\n", cpu_time_used);
        // 计算墙钟时间  
        elapsed_time = difftime(end_time, start_time);  
        printf("墙钟时间: %f秒\n", elapsed_time); 
    }
    else 
    {// parent goes down this path (orginal process)
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int) getpid()); 
        clock_t start_clock, end_clock;  
        double cpu_time_used;  
        time_t start_time, end_time;  
        double elapsed_time;  

        start_clock = clock();   
        time(&start_time);   

        //C=A*B + beta*C
        dgemm(M,N,K,2, A,B,C);
        printf_matrix(M,N,C); 
        end_clock = clock();
        time(&end_time);  

        cpu_time_used = ((double) (end_clock - start_clock)) / CLOCKS_PER_SEC;  
        printf("CPU时间: %f秒\n", cpu_time_used);  
        elapsed_time = difftime(end_time, start_time);  
        printf("墙钟时间: %f秒\n", elapsed_time); 
    }
    
}
