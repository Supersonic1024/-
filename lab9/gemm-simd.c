#include <stdio.h>
#include "defs.h"
#include <immintrin.h>  // ���� AVX ָ�  

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{  
    for (int i = 0; i < m; i++) {  
        for (int j = 0; j < n; j++) {  
            __m256 c_val = _mm256_setzero_ps(); // ʹ�� AVX ��ʼ�� c_val  
            for (int p = 0; p < k; p += 8) { // ÿ�δ���8��Ԫ��  
                __m256 a_val = _mm256_loadu_ps(A + i * N + p);  
                __m256 b_val = _mm256_loadu_ps(B + p * N + j);  
                c_val = _mm256_fmadd_ps(a_val, b_val, c_val); // ����˷�  
            }  
            C[i * N + j] += _mm256_reduce_add_ps(c_val); // ��Լ������ c_val �ĺ�  
        }  
    }  
}