//
// Created by jcfei on 18-9-9.
//

#include "tensor.h"

template<class T>
struct cp_mats{
    Mat<T> A,B,C;
};

template<class T>
cp_mats<T> cp_als(Cube<T> &a, int r) {
    int n1=a.n_rows; int n2=a.n_cols; int n3=a.n_slices;  //dimension

    Mat<T> A(n1,r); Mat<T> B=randn<Mat<T>>(n2,r); Mat<T> C=randn<Mat<T>>(n3,r); //random A,B,C

    Mat<T> cal(n1,n2); Mat<T> tmp(n1,r);

//    cube b(a);
    reshape(a,n1*n2,n3,1);
    Mat<T> a1 = a.slice(0);
    reshape(a,n1,n2,n3);
    tmp= a1*a1.t();
    a.reshape(n1,n2,n3);

    for (int turn=0; turn<1;turn++) {
        tmp.zeros();
        tmp.set_size(n1, r);
        Mat<T> kr_tmp(n2*n3,r);
        for (int i = 0; i < r; i++) {
            kr_tmp.col(i) = kron(C.col(i), B.col(i));
        }
        tmp = a1 * kr_tmp;
        A = tmp * pinv((C.t() * C) % (B.t() * B));
//        Mat<T> tmpp = (C.t() * C) % (B.t() * B);
//        solve( A, tmp, tmpp);
        A = normalise(A);

        tmp.set_size(n2, r);
        tmp.zeros();
        kr_tmp.set_size(n1, r);
        for (int j = 0; j < n3; j++) {
            for (int i = 0; i < r; i++) {
                kr_tmp.col(i) = C(j, i) * A.col(i);
            }
            cal = a1.cols(j*n2, j*n2+n2-1);
            tmp = tmp + cal.t() * kr_tmp; //slice computing
        }
        B = tmp * pinv((C.t() * C) % (A.t() * A));
//        tmpp = (C.t() * C) % (A.t() * A);
//        solve( B, tmp, tmpp);

        B = normalise(B);


        tmp.set_size(n3, r);
        tmp.zeros();
        cal.set_size(1,n1*n2);
        kr_tmp.set_size(n1*n2,r);
//        for (int j = 0; j < n2; j++) {
//            for (int i = 0; i < r; i++) {
//                kr_tmp.col(i) = B(j, i) * A.col(i);
//            }
//            cal = a1.cols(j*n2, j*n2+n2-1);
//            cal.reshape(n1,n2);
//            tmp = tmp + cal.t() * kr_tmp; //slice computing
//        }
//        a1.set_size(n1*n2,n3);
        Mat<T> a3(n1*n2,n3);
        for (int i = 0; i < r; i++) {
            kr_tmp.col(i) = kron(B.col(i), A.col(i));
        }

        for (int j = 0; j < n3; j++) {
            Mat<T> tmp1 = a.slice(j);
            cal = vectorise(tmp1);
            a3.col(j) = cal;
        }
        tmp = a3.t() * kr_tmp;
//        mat tmpp = (B.t() * B) % (A.t() * A);
//        C = solve(tmpp, tmp);
        C = tmp * pinv((B.t() * B) % (A.t() * A));

        if (turn < 0){
            C = normalise(C);
        }
    }

//    Cube<T> result(n1,n2,n3);
//    for(int i=0;i<n1;i++){
//        for(int j=0;j<n2;j++){
//            for(int k=0;k<n3;k++){
//                T tmp=0; //类型错误...
//                for(int r0=0; r0<r;r0++){
//                    tmp = tmp + A(i,r0) * B(j,r0) * C(k,r0);
//
//                }
//                result(i,j,k) = tmp;
//            }
//        }
//    }
////    cout << result << endl;
//
//    result = (result-a) % (result-a);
//    cout << sqrt(accu(result)) << endl;
//    cout << sqrt(accu(a % a)) << endl;

    cp_mats<T> result0;
    result0.A = A;
    result0.B = B;
    result0.C = C;

//    cout << A << endl;
//    cout << B << endl;
//    cout << C << endl;

    return result0;
}


//template<class T>
//cp_mats<T> cp_als(Cube<T> a, int r,int max_iter) {
//    int n1=a.n_rows; int n2=a.n_cols; int n3=a.n_slices;  //dimension
//
//    Mat<T> A(n1,r); Mat<T> B=randu<Mat<T>>(n2,r); Mat<T> C=randu<Mat<T>>(n3,r); //random A,B,C
//
//    Mat<T> cal(n1,n2); Mat<T> tmp(n1,r);
//
////    cube b(a);
//    Mat<T> a1(n1,n2*n3);
////    Mat<T> *a1_tmp;
//    a.reshape(n1,n2*n3,1);
//    a1 = a.slice(0);
////    a1 = *a1_tmp;
//    a.reshape(n1,n2,n3);
//
//    for (int turn=0; turn< max_iter;turn++) {
//        tmp.zeros();
//        tmp.set_size(n1, r);
//        Mat<T> kr_tmp(n2*n3,r);
//        Mat<T> tmpp = (C.t() * C) % (B.t() * B);
//
////#pragma omp parallel for num_threads(2)
//        for (int i = 0; i < r; i++) {
//            kr_tmp.col(i) = vectorise(C.col(i) *trans(B.col(i)));
////            kr_tmp.col(i) = kron(C.col(i), B.col(i));
////            tmp.col(i) = a1 * kr_tmp.col(i);
//        }
//
//        tmp = a1 * kr_tmp;
//        A = tmp * pinv(tmpp);
////        A = tmp * pinv(tmpp,0.01,"std");
//
//        A = normalise(A);
//
//        tmp.set_size(n2, r);
//        tmp.zeros();
////        kr_tmp.set_size(n1, r);
//        kr_tmp.set_size(n1, r);
//
////#pragma omp parallel for num_threads(4)
//        for (int j = 0; j < n3; j++) {
//            for (int i = 0; i < r; i++) {
//                T ddd = C(j,i);
//                kr_tmp.col(i) = ddd * A.col(i);
//            }
//            cal = a1.cols(j*n2, j*n2+n2-1);
//            tmp = tmp + cal.t() * kr_tmp; //slice computing
//        }
////        for (int i = 0; i < r; i++) {
////            kr_tmp.col(i) = vectorise(C.col(i) *trans(A.col(i)));
//////            kr_tmp.col(i) = kron(C.col(i), B.col(i));
//////            tmp.col(i) = a1 * kr_tmp.col(i);
////        }
////        tmp = a1 * kr_tmp;
//
//        tmpp = (C.t() * C) % (A.t() * A);
//        B = tmp * pinv(tmpp);
//        B = normalise(B);
//
//
//        tmp.set_size(n3, r);
//        tmp.zeros();
//        cal.set_size(1,n1*n2);
//        kr_tmp.set_size(n1*n2,r);
//
//        Mat<T> a3(n1*n2,n3);
//        for (int i = 0; i < r; i++) {
////            kr_tmp.col(i) = kron(B.col(i), A.col(i));
//            kr_tmp.col(i) = vectorise(B.col(i) *trans(A.col(i)));
//        }
//
//        for (int j = 0; j < n3; j++) {
//            Mat<T> tmp1 = a.slice(j);
//            cal = vectorise(tmp1);
//            a3.col(j) = cal;
//        }
//        tmp = a3.t() * kr_tmp;
//
////        C = solve(tmpp, tmp);
//        tmpp = (B.t() * B) % (A.t() * A);
//
//        C = tmp * pinv(tmpp);
//
//        if (turn < max_iter-1){
//            C = normalise(C);
//        }
//    }
//
////    cout << A << endl;
////    cout << B << endl;
////    cout << C << endl;
//
//    Cube<T> result(n1,n2,n3);
////    for(int k=0;k<n3;k++){
////        Mat<T> tmp1=zeros(n1,n2);
////        for(int i=0;i<r;i++){
////            Mat<T> tmp ;
////            tmp = kron(B.col(i), A.col(i));
////            tmp.reshape(n1,n2);
////            tmp =  C(k,i) * tmp;
////            tmp1 += tmp;
////        }
////        result.slice(k) = tmp1;
////    }
////
////
////    result = (result-a) % (result-a);
////    cout << sqrt(accu(result)) << endl;
////    cout << sqrt(accu(a % a)) << endl;
//
////    cout << A << endl << B << endl << C << endl;
////    cout << "reconstruction: " << result << endl;
//
//    cp_mats<T> result0;
//    result0.A = A;
//    result0.B = B;
//    result0.C = C;
//
////    cout << A << endl;
////    cout << B << endl;
////    cout << C << endl;
//
//    return result0;
//}

//,0,"std"
//    Tensor<T> t_con(n1,n2,n3);
//
//    for (int i=0;i<n1;i++){
//        for (int j=0;j<n2;j++){
//            for(int k=0;k<n3;k++){
//                T sum = 0;
//                for (int l=0;l<r;l++){
//                    sum = sum + A(i,l)*B(j,l)*C(k,l);
//                }
//                t_con(i,j,k) = sum;
//            }
//        }
//    }
//    mat m1=ten2mat(t_con,1);
//    mat m2=ten2mat(a,1);
//    cout << m1-m2 << endl;

//    cout << A << endl;
//    cout << B << endl;
//    cout << C << endl;

template<class T>
cp_mats<T> cp_als(Cube<T> &a, int r,int max_iter) {
    int n1=a.n_rows; int n2=a.n_cols; int n3=a.n_slices;  //dimension

    Mat<T> A(n1,r); Mat<T> B=randu<Mat<T>>(n2,r); Mat<T> C=randu<Mat<T>>(n3,r); //random A,B,C

//    Mat<T> a1(n1,n2*n3);
//    a.reshape(n1,n2*n3,1);
//    a1 = a.slice(0);
//    a.reshape(n1,n2,n3);

    Mat<T> cal(n1,n2); Mat<T> tmp(n1,r);
    for (int turn=0; turn< max_iter;turn++) {
        tmp.zeros();
        tmp.set_size(n1, r);
        Mat<T> kr_tmp(n2*n3,r);
        Mat<T> tmpp = (C.t() * C) % (B.t() * B);

        tmp.set_size(n2, r);
        tmp.zeros();
        kr_tmp.set_size(n2, r);
        Mat<T> vvv;
        for (int j = 0; j < n3; j++) {
//            vvv = C.row(j);
            for (int i = 0; i < r; i++) {
                register T ddd = C(j,i);
//                T ddd = vvv(i);
                kr_tmp.col(i) = ddd * B.col(i);
            }
            cal = a.slice(j);
//            cal = a1.cols(j*n2, j*n2+n2-1);
            tmp = tmp + cal * kr_tmp; //slice computing
        }
        tmpp = (C.t() * C) % (B.t() * B);
        A = tmp * pinv(tmpp);
        A = normalise(A);

        kr_tmp.set_size(n1, r);
        for (int j = 0; j < n3; j++) {
            for (int i = 0; i < r; i++) {
                T ddd = C(j,i);
                kr_tmp.col(i) = ddd * A.col(i);
            }
            cal = a.slice(j);
//            cal = a1.cols(j*n2, j*n2+n2-1);
            tmp = tmp + cal.t() * kr_tmp; //slice computing
        }

        tmpp = (C.t() * C) % (A.t() * A);
        B = tmp * pinv(tmpp);
        B = normalise(B);

        tmp.set_size(n3, r);
        tmp.zeros();
        cal.set_size(1,n1*n2);
        kr_tmp.set_size(n1*n2,r);

        Mat<T> a3(n1*n2,n3);
        for (int i = 0; i < r; i++) {
            kr_tmp.col(i) = vectorise(B.col(i) *trans(A.col(i)));
        }

        for (int j = 0; j < n3; j++) {
            Mat<T> tmp1 = a.slice(j);
            cal = vectorise(tmp1);
            a3.col(j) = cal;
        }
        tmp = a3.t() * kr_tmp;

//        C = solve(tmpp, tmp);
        tmpp = (B.t() * B) % (A.t() * A);

        C = tmp * pinv(tmpp);

        if (turn < max_iter-1){
            C = normalise(C);
        }
    }
//    a1.reset();
//    cout << A << endl;
//    cout << B << endl;
//    cout << C << endl;

//    Cube<T> result(n1,n2,n3);
//    for(int k=0;k<n3;k++){
//        Mat<T> tmp1=zeros(n1,n2);
//        for(int i=0;i<r;i++){
//            Mat<T> tmp ;
//            tmp = kron(B.col(i), A.col(i));
//            tmp.reshape(n1,n2);
//            tmp =  C(k,i) * tmp;
//            tmp1 += tmp;
//        }
//        result.slice(k) = tmp1;
//    }
//
//    result = (result-a) % (result-a);
//    cout << sqrt(accu(result)) << endl;
//    cout << sqrt(accu(a % a)) << endl;

//    cout << A << endl << B << endl << C << endl;
//    cout << "reconstruction: " << result << endl;

    cp_mats<T> result0;
    result0.A = A;
    result0.B = B;
    result0.C = C;

//    cout << A << endl;
//    cout << B << endl;
//    cout << C << endl;

    return result0;
}

