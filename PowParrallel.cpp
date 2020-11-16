// PowParrallel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>
#include <chrono>
#include <algorithm>

#include <assert.h>
#include <string>
#include <stdio.h>
using namespace std;

#include "BigInteger.h"
#include "Montgomery.hpp"


typedef std::chrono::high_resolution_clock Clock;
inline BigInt binpow(BigInt a, BigInt b) {

    if (b == 0)
        return 1;
    BigInt res = binpow(a, b / 2);
    if (b % 2 ==0)
        return res * res;   
    else
        return res * res * a;
}

using namespace std;

inline BigInt mpow(BigInt a, BigInt b)
{
    BigInt res = 1;
    for (BigInt i = 0; i < b; i=i+1)
    {
        res =res* a;
    }
    return res;
}

int main()
{
    double startTime, endTime, ticks;

    long long a, b, m;
    cin >> a >> b >> m;

    BigInt big_a(a);
    BigInt big_b(b);
    BigInt big_m(m);
    BigInt b1(b / 4);
    BigInt b2(b / 4);
    BigInt b3(b / 4);
    BigInt b4(b / 4+b % 4);

    BigInt res_array[8] = {0};
    BigInt b_array[8] = { b / 4,b / 4,b / 4,b / 4,b / 4,b / 4,b / 4,b / 4 + b % 4 };

    startTime = omp_get_wtime();
   

    omp_set_num_threads(8);
#pragma omp parallel for
    for (int i = 0; i < 8; i++)
    {
        printf("Thead number %d\n", omp_get_thread_num());
        res_array[i] = binpow(big_a, b_array[i]);
    }

   
    endTime = omp_get_wtime();
    ticks = omp_get_wtick();

    BigInt res=1;

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            res = res * res_array[i];
        }
        res = res % big_m;
    }
    cout << "res=" << res;
    cout << "\nDuration: " << endTime - startTime << "\n";
    cout << "Timer accuracy: " << ticks << "\n";

    cout << "Montgomery\n";
    startTime = omp_get_wtime();

    res = MonExp(a, b, m);

    endTime = omp_get_wtime();
    ticks = omp_get_wtick();
    cout << "res=" << res;
    cout << "\nDuration: " << endTime - startTime << "\n";
    cout << "Timer accuracy: " << ticks << "\n";

    cout << "Linear\n";
    startTime = omp_get_wtime();
   
    res = mpow(big_a, big_b) % big_m;
    
    endTime = omp_get_wtime();
    ticks = omp_get_wtick();
    cout << "res=" << res;
    cout << "\nDuration: " << endTime - startTime << "\n";
    cout << "Timer accuracy: " << ticks << "\n";
  
}

