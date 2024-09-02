#include "utils.h"

#include "raylib.h"

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

float get_random_float() {
    return (float)GetRandomValue(0, 100000)/(float)(100001);
}

unsigned int mod(int a, int b) {
    return (a % b + b) % b;
}

