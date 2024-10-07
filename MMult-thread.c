#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include "defs.h"  

typedef struct {  
    int m;          // 矩阵 A 的行数  
    int n;          // 矩阵 B 的列数  
    int k;          // 矩阵 A 和 B 的共享维度  
    double *A;     // 矩阵 A  
    double *B;     // 矩阵 B  
    double *C;     // 结果矩阵 C  
    int lda;       // 矩阵 A 的领先维度  
    int ldb;       // 矩阵 B 的领先维度  
    int ldc;       // 矩阵 C 的领先维度  
    int row_start; // 当前线程处理的起始行  
    int row_end;   // 当前线程处理的结束行  
} ThreadData;  

void *threaded_mmult(void *arg) {  
    ThreadData *data = (ThreadData *)arg;  

    int m = data->m;  
    int n = data->n;  
    int k = data->k;  
    double *A = data->A;  
    double *B = data->B;  
    double *C = data->C;  
    int lda = data->lda;  
    int ldb = data->ldb;  
    int ldc = data->ldc;  

    // 处理分配给线程的行  
    for (int i = data->row_start; i < data->row_end; i++) {  
        for (int j = 0; j < n; j++) {  
            double sum = 0.0;  
            for (int p = 0; p < k; p++) {  
                sum += A[i * lda + p] * B[p * ldb + j];  
            }  
            C[i * ldc + j] = sum;  
        }  
    }  
    return NULL;  
}  

void MY_MMult(int m, int n, int k, double *A, int lda, double *B, int ldb, double *C, int ldc) {  
    int num_threads = 4; // 假设使用4个线程  
    pthread_t threads[num_threads];  
    ThreadData thread_data[num_threads];  

    // 创建多个线程并分配任务  
    int rows_per_thread = m / num_threads;  
    
    for (int i = 0; i < num_threads; i++) {  
        thread_data[i].m = m;  
        thread_data[i].n = n;  
        thread_data[i].k = k;  
        thread_data[i].A = A;  
        thread_data[i].B = B;  
        thread_data[i].C = C;  
        thread_data[i].lda = lda;  
        thread_data[i].ldb = ldb;  
        thread_data[i].ldc = ldc;  
        thread_data[i].row_start = i * rows_per_thread;  
        thread_data[i].row_end = (i == num_threads - 1) ? m : (i + 1) * rows_per_thread;  

        pthread_create(&threads[i], NULL, threaded_mmult, (void *)&thread_data[i]);  
    }  

    // 等待所有线程完成  
    for (int i = 0; i < num_threads; i++) {  
        pthread_join(threads[i], NULL);  
    }  
}