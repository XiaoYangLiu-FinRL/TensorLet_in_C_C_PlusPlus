#include "tensor.h"
#include "runningtime.h"

#include "ten2mat.cpp"
#include "cpgen.cpp"
#include "cp_als.cpp"
#include "tucker_hosvd.cpp"
#include "tensor_hooi.cpp"
#include "t_svd.cpp"

#include "Tensor3D.h"
#include <stdio.h>
#include <mkl.h>

using namespace std;

int main(){

    MKL_INT n1,n2,n3;
    double t0,t1;
    int I=100;
    int R=10;
    //    bool cc= true;

    n1=I; n2=2*I; n3=I;

    int iter=10;
    double ccc =0.05;
    t0=gettime();
    Tensor3D<double> a(1000,1000,1000);
    double *cc = (double*)mkl_calloc(100000000,sizeof(double),64);  //返回成功为1
    cout << cc << endl;
    cout << "sizeof(double)" << sizeof(double) << endl;
    cout << sizeof(cc) << endl;

    t1=gettime();
    cout << sizeof(a) << endl;
    cout << "time:" <<t1-t0 <<endl;
    cout << a.getsize()[2];

    return 0;
}

//int main(void) {
//    double *a, *b, *c;
//    int n, i;
//    double alpha, beta;
//    MKL_INT64 AllocatedBytes;
//    int N_AllocatedBuffers;
//    alpha = 1.1; beta = -1.2;
//    n = 1000;
//    mkl_peak_mem_usage(MKL_PEAK_MEM_ENABLE);
//    a = (double*)mkl_malloc(n*n*sizeof(double),64);
//    b = (double*)mkl_malloc(n*n*sizeof(double),64);
//    c = (double*)mkl_calloc(n*n,sizeof(double),64);
//    for (i=0;i<(n*n);i++) {
//        a[i] = (double)(i+1);
//        b[i] = (double)(-i-1);
//    }
//    cout << a << endl;
//
//    dgemm("N","N",&n,&n,&n,&alpha,a,&n,b,&n,&beta,c,&n);
//    AllocatedBytes = mkl_mem_stat(&N_AllocatedBuffers);
//    printf("\nDGEMM uses %d bytes in %d buffers",AllocatedBytes,N_AllocatedBuffers);
//    mkl_free_buffers();
//    mkl_free(a);
//    mkl_free(b);
//    mkl_free(c);
//    AllocatedBytes = mkl_mem_stat(&N_AllocatedBuffers);
//    if (AllocatedBytes > 0) {
//        printf("\nMKL memory leak!");
//        printf("\nAfter mkl_free_buffers there are %d bytes in %d buffers",
//               AllocatedBytes,N_AllocatedBuffers);
//    }
//
//    return 0;
//}