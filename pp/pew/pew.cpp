#include <iostream>
#include <math.h>

#include "pew.h"

ull pew(ull x, ull y) {
    return (ull)pow((double)x,(double)y);
}

int main() {
    std::cout << pew(10,2) << std::endl;
    std::cout << pew(104,204) << std::endl;

}
