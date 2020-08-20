#include <iostream>

unsigned long long**** data = new unsigned long long***[20];
int numeric[20];

void numberInArr(unsigned long long number) {
    int i = 19;

    while (number != 0) {
        numeric[i--] = number % 10;
        number /= 10;
    }
}

void clear() {
    for(int i = 0; i < 20; ++i)
        for(int j = 0; j < 200; ++j)
            for(int k = 0; k < 100; ++k) {
                data[i][j][k][0] = 0;
                data[i][j][k][1] = 0;
            }
    data[0][0][0][1] = 1;

    for(int i = 0; i < 20; ++i)
        numeric[i] = 0;
}

void solvingProblem(int k) {
    for (int digit = 0; digit < 19; ++digit)
        for (int sum = 0; sum < 9 * digit + 1; ++sum)
            for (int divRemainder = 0; divRemainder < k; ++divRemainder)
                for (int point = 0; point < 2; ++point) {
                    if (point == 1) {
                        for (int j = 0; j <= 9; ++j)
                            data[digit + 1][sum + j][(int) ((10 * divRemainder + j) % k)][0]
                                    += data[digit][sum][divRemainder][0];
                    }
                    else {
                        for (int j = 0; j <= numeric[digit + 1]; ++j) {
                            int buff = 1;
                            if (j != numeric[digit + 1])
                                buff = 0;
                            data[digit + 1][sum + j][(int) ((10 * divRemainder + j) % k)][buff]
                                    += data[digit][sum][divRemainder][1];
                        }
                    }
                }
}

unsigned long long checkRange(int p, int q) {
    unsigned long long num = 0;
    while (p <= q) {
        num += data[19][p][0][0] + data[19][p][0][1];
        ++p;
    }
    return num;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);;

    unsigned int k, p, q;
    unsigned long long a, b, num = 0;

    std::cin >> k >> a >> b >> p >> q;

    if(k != 0 && a <= b && p <= q) {
        for (int i = 0; i < 20; ++i) {
            data[i] = new unsigned long long **[200];
            for (int j = 0; j < 200; ++j) {
                data[i][j] = new unsigned long long *[100];
                for (int k = 0; k < 100; ++k)
                    data[i][j][k] = new unsigned long long[2];
            }
        }

        clear();
        numberInArr(b);
        solvingProblem(k);
        num += checkRange(p, q);

        clear();
        numberInArr(a - 1);
        solvingProblem(k);
        num -= checkRange(p, q);

        std::cout << num;
    } else
        std::cout << 0;

    return 0;
}