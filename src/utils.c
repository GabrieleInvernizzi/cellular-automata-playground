#include "utils.h"

#include "raylib.h"

int imin(int a, int b) {
    return a < b ? a : b;
}

int imax(int a, int b) {
    return a > b ? a : b;
}

float get_random_float() {
    return (float)GetRandomValue(0, 100000)/(float)(100001);
}

unsigned int mod(int a, int b) {
    return (a % b + b) % b;
}

