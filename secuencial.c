#include <stdio.h>


long fibonacci(int n);

int main() {
    int n;
    long resultado;

    printf("\nNúmero a calcular? ");
    scanf("%d", &n);

    resultado = fibonacci(n);

    printf("\nEl número Fibonacci de %d es %ld\n", n, resultado);

    return 0;
}


long fibonacci(int n) {
    long fn1, fn2, fn;

    if (n == 0 || n == 1)
        return n;

    if (n < 30) {
        fn1 = fibonacci(n - 1);
        fn2 = fibonacci(n - 2);
        fn = fn1 + fn2;
        return fn;
    }

    return 0; 
}
