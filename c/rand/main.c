#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int get_random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main(void) {
    srand(time(NULL));

    for (int i = 0; i < 1000000; i += 1) {
        int n = get_random_range(5, 10);
        assert(n >= 5 && n <= 10);
    }

    return 0;
}
