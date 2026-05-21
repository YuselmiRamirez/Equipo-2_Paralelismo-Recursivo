#include <stdio.h>
#include <omp.h>

long fibonacci(int n);

int main () {
    int n;
    long resul;

    printf("\nNúmero a calcular? ");
    scanf("%d", &n);

    #pragma omp parallel
    {
        #pragma omp single
        {
            resul = fibonacci(n);
        }
    }

    printf("\nEl número Fibonacci de %d es %ld\n", n, resul);
    return 0;
}

long fibonacci(int n) {
    long fn1, fn2, fn;

    if (n == 0 || n == 1)
        return n;

    if (n < 30) {
        #pragma omp task shared(fn1)
        {
            fn1 = fibonacci(n-1);
        }
        #pragma omp task shared(fn2)
        {
            fn2 = fibonacci(n-2);
        }
        #pragma omp taskwait
        {
            fn = fn1 + fn2;
        }
        return fn;
    }
    return 0; 
}
