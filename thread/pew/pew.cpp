#include <iostream>
#include <math.h>

#define ull unsigned long long

double pew(unsigned long long x, unsigned long long y) {
    return (double)pow((double)x,(double)y);
}

int main() {
    std::cout << pew(10,2) << std::endl;
    std::cout << pew(104,204) << std::endl;

}
