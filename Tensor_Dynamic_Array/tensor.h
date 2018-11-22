//
// Created by jcfei on 4/30/18.
//

#ifndef TENSOR_TENSOR_H
#define TENSOR_TENSOR_H
#include <fftw3.h>

#include "string.h"
#include <iostream>
#include <armadillo>
using namespace std;
using namespace arma;

template <class T>
class Tensor{
public:

    Tensor<T>(int,int,int); //创建三维张量
    Tensor<T>();  
    ~Tensor<T>();  //析构函数

//重载运算符
    Tensor<T> & operator=(const Tensor&);
    Tensor<T>& operator+=(const Tensor&);
    Tensor<T>& operator-=(const Tensor&);
    Tensor<T>& operator*=(const Tensor&);
    template <class T1> Tensor<T>& operator*=(T1);
    template <class T1> Tensor<T>& operator/=(T1);

    inline T& operator()(int x, int y, int z) {
        if (x<n1 && y<n2 && z<n3) {
            return p[x][y][z];
        }
        else {
            cout<<"Warning: Index beyond range."<<endl;
            exit(0);
        }
    }

    static Tensor<T> rand(int, int, int); //零张量
    static Tensor<T> Identity(int, int, int); //零张量

    //    private:
    int n1,n2,n3;
    T ***p;
    T*** allocSpace();
};

//get the size of tensor
template <class T>
int * getsize(const Tensor<T> &a);

//fiber
template <class T>
Col<T> fiber(const Tensor<T> & t, int m, int n ,int order);

//slice
template <class T>
Mat<T> slice(const Tensor<T> & t, int m, int order);

//tensor 转 matrix
template<class T>
Mat<T> ten2mat(Tensor<T> & a , int b);

//tensor 转 vector
template<class T>
Col<T> ten2vec(Tensor<T> & a);

//norm
template<class T>
double norm(Tensor<T> &a);      

//张量转置运算
template<class T>
Tensor<T> Transpose(Tensor<T> &a);

//Inner product
template<class T1, class T2>
double dotProduct(Tensor <T1>a, Tensor<T2> b);

//t-prod of tensor
template<class T, class T1, class T2>
Tensor<T> tprod(Tensor<T1> &a, Tensor<T2> &b);


//n-mode product
template<class T>
mat ttm(Tensor<T> & a, mat & b, int c);

/**************************************
*******Realization of functions********
**************************************/
//定义三维张量
template <class T>
Tensor<T>::Tensor(int n1, int n2, int n3) : n1(n1), n2(n2),n3(n3)
{
    allocSpace(); 
}

//析构函数
template <class T>
Tensor<T>::~Tensor() {
    for (int i=0; i<n1;i++) {
        for (int j=0;j<n2;j++){
            delete[] p[i][j];
        }
        delete[] p[i];
    }
    delete[] p;
    p=NULL;
}

template<class T>
Tensor<T>::Tensor() {
    allocSpace();
    p[0][0][0]=1;
}

//分配内存空间（三维动态数组）
template <class T>
T*** Tensor<T>::allocSpace()
{
    p =new T**[n1];
    for (int i=0; i<n1;i++) {
        p[i] = new T *[n2];
        for (int j=0;j<n2;j++){
            p[i][j]=new T [n3];
        }
    }
    return p;
}

template<class T>
Tensor<T> &Tensor<T>::operator=(const Tensor & a) {
    if(this == &a){
        return *this;
    }
    if(n1 != a.n1 || n2 != a.n2 || n3 != a.n3){
        for (int i=0; i<n1;i++) {
            for (int j=0;j<n2;j++){
                delete[] p[i][j];
            }
            delete[] p[i];
        }
        delete[] p;
        n1=a.n1;n2=a.n2;n3=a.n3;
        allocSpace();
    }
    for (int i=0; i<n1; ++i){
        for (int j=0; j<n2;++j){
            for(int k=0;k<n3;++k){
                p[i][j][k]=a.p[i][j][k];
            }
        }
    }
    return *this;
}

template<class T>
Tensor<T> &Tensor<T>::operator+=(const Tensor &a) {
    if (n1 == a.n1 && n2 == a.n2 && n3 == a.n3){
        for (int i=0; i<a.n1; ++i){
            for (int j=0; j<a.n2;++j){
                for(int k=0;k<a.n3;++k){
                    p[i][j][k]+=a.p[i][j][k];
                }
            }
        }
        return *this;}
    else{
        cout<<"check size match"<<endl;
        exit(0);
    }
}

template<class T>
Tensor<T> &Tensor<T>::operator-=(const Tensor &a) {
    if (n1 == a.n1 && n2 == a.n2 && n3 == a.n3){
        for (int i=0; i<a.n1; ++i){
            for (int j=0; j<a.n2;++j){
                for(int k=0;k<a.n3;++k){
                    p[i][j][k]-=a.p[i][j][k];
                }
            }
        }
        return *this;}
    else{
        cout<<"check size match"<<endl;
        exit(0);
    }
}

template<class T>
Tensor<T> &Tensor<T>::operator*=(const Tensor &a) {
    if (n1 == a.n1 && n2 == a.n2 && n3 == a.n3){
        for (int i=0; i<a.n1; ++i){
            for (int j=0; j<a.n2;++j){
                for(int k=0;k<a.n3;++k){
                    p[i][j][k]*=a.p[i][j][k];
                }
            }
        }
        return *this;}
    else{
        cout<<"check size match"<<endl;
        exit(0);
    }
}

template<class T>
template<class T1>
Tensor<T> &Tensor<T>::operator/=(T1 num) {
    if (num != 0){
        for (int i=0; i<n1; ++i){
            for (int j=0; j<n2;++j){
                for(int k=0;k<n3;++k){
                    p[i][j][k]/=num;
                }
            }
        }
    }
    else {
        cout << "divide number is zero."<< endl;
        exit(0);
    }
}

template<class T>
template<class T1>
Tensor<T> &Tensor<T>::operator*=(T1 num) {
    for (int i=0; i<n1; ++i){
        for (int j=0; j<n2;++j){
            for(int k=0;k<n3;++k){
                p[i][j][k]/=num;
            }
        }
    }
}


template<class T>
Tensor<T> Tensor<T>::rand(int n1, int n2, int n3) {
    Tensor<T> tem(n1,n2,n3);
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {
            for(int k=0; k<n3; ++k) {
                tem.p[i][j][k] = randn();
            }
        }
    }
    return tem;
}

template<class T>
Tensor<T> Tensor<T>::Identity(int n1, int n2, int n3) {
    if(n1==n2){
        Tensor tem(n1,n2,n3);
        for (int i = 0; i < n1; ++i) {
            for (int j = 0; j < n2; ++j) {
                for(int k=0; k<n3; ++k) {
                    if(k==0 && i==j) {
                        tem.p[i][j][k] = 1;
                    }
                    else {
                        tem.p[i][j][k]=0;
                    }
                }
            }
        }
        return tem;
    }
    else{
        cout<<"Warning: no identity tensor";
        exit(0);
    }
}

template<class T>
int *getsize(const Tensor<T> &a) {
    static int p[3]; 
    p[0]=a.n1;
    p[1]=a.n2;
    p[2]=a.n3;
    return p;
}

template<class T>
Col<T> fiber(const Tensor<T> &t, int m, int n, int order) {
    if(order==1){
        Col<T> c(t.n1);
        for (int i=0; i<t.n1;++i){
            c(i)=t.p[i][m][n];
        }
        return c;
    }
    if(order==2){
        Col<T> c(t.n2);
        for (int i=0; i<t.n1;++i){
            c(i)=t.p[i][m][n];
        }
        return c;
    }
    if(order==3){
        Col<T> c(t.n3);
        for (int i=0; i<t.n1;++i){
            c(i)=t.p[i][m][n];
        }
        return c;
    }
}

template<class T>
Col<T> ten2vec(Tensor<T> &a) {
    int len = a.n1*a.n2*a.n3;
    Col<T> result(len);
    for (int i = 0;  i < a.n1;i++){
        for (int j = 0; j < a.n2; j++){
            for (int k = 0; k < a.n3; k++ ){
                int I = a.n1;
                int J = a.n1*a.n2;
                result(i+j*I+k*J) = a.p[i][j][k];
            }
        }
    }
    return result;
}

template<class T>
Mat<T> ten2mat(Tensor<T> &a, int b) {
    int m,len;
    if (b==1){ m=a.n1;len = a.n2*a.n3;
        Mat<T> result = zeros<Mat<T>>(m,len);
        for (int k = 0;  k < a.n3;k++){
            for (int i = 0; i < a.n1; i++){
                for (int j = 0; j < a.n2; j++ ){
                    int J = a.n2;
                    result(i,j+k*J) = a.p[i][j][k];
                }
            }
        }
        return result;
    }
    else if (b == 2){ m=a.n2;len = a.n1*a.n3;
        Mat<T> result = zeros<Mat<T>>(m,len);
        for (int i = 0; i < a.n1; i++){
            for (int k = 0;  k < a.n3;k++){
                for (int j = 0; j < a.n2; j++ ){
                    int J = a.n1;
                    result(j,i+k*J) = a.p[i][j][k];
                }
            }
        }
        return result;
    }
    else { m=a.n3;len = a.n1*a.n2;
        Mat<T> result = zeros<Mat<T>>(m,len);
        for (int j = 0; j < a.n2; j++ ){
            for (int k = 0;  k < a.n3;k++){
                for (int i = 0; i < a.n1; i++){
                    int J = a.n1;
                    result(k,i+j*J) = a.p[i][j][k];
                }
            }
        }
        return result;
    }
}

#endif //TENSOR_TENSOR_H
