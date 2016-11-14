/* 
 * File          : find.cpp
 *
 *
 * Author        : Juan Carlos Maureira
 * Created       : Thu 30 Jun 2016 11:48:42 AM CLT
 * ast Modified : Thu 30 Jun 2016 04:29:52 PM CLT
 *
 * (c) 2016 Juan Carlos Maureira
 */

#include <iostream>
#include <math.h>
#include <algorithm>

#include <vector>
#include <numeric>
#include <cstdint>
#include <cassert>
#include <stdlib.h>
#include <time.h>

#include <gmp.h>

#include <omp.h>

#include "find.h"

// src: http://howardhinnant.github.io/combinations.html
#include "combinations.h"

// display combinations
template <class It> unsigned display(It begin, It end) {
    unsigned r = 0;
    if (begin != end)
    {
        std::cout << *begin;
        ++r;
        for (++begin; begin != end; ++begin)
        {
            std::cout << ", " << *begin;
            ++r;
        }
    }
    return r;
}

// keep the max powerCouple
struct PowerCouple {
    std::vector<int>& numbers;
    int x = 0;
    int y = 0;
    long double pow = 0;

    PowerCouple(std::vector<int>& n) : numbers(n) { }

    template <class It> bool eval(It begin, It end) {
        // find the array positions of this permutation
        unsigned r = 0;
        int a = 0;
        int b = 0;
        if (begin != end)
        {
            a = *begin;
            ++r;
            for (++begin; begin != end; ++begin)
            {
                b = *begin;
                ++r;
            }
        }

        // evaluate powerCouple
        long double p1 = std::pow((double)numbers[a],(double)numbers[b]);
        long double p2 = std::pow((double)numbers[b],(double)numbers[a]);

        //std::cout << "(a,b)=" << a << " " << b << " = " << numbers[a] << "," << numbers[b] << " " p1 << " " << p2 << std::endl;

        if (p1 > this->pow) {
            this->x = numbers[a];
            this->y = numbers[b];
            this->pow = p1;
            return true;
        } else if (p2 > this->pow ) {
            this->x = numbers[b];
            this->y = numbers[a];
            this->pow = p2;
            return true;
        }

        return false;
    }
};

class EvalPowerCouple {
    unsigned len;
    std::uint64_t count;
    PowerCouple* pc;

    bool show = false;

public:
    explicit EvalPowerCouple(unsigned l, PowerCouple* t) : len(l), count(0), pc(t) { }

    template <class It>
        bool operator()(It first, It last)  // called for each permutation
        {
            ++count;
            pc->eval(first,last);

            unsigned r = 0;
            if (this->show) {
                std::cout << "[ ";
                r = display(first, last);
                if (r < len)
                {
                    std::cout << " | ";
                    display(last, std::next(last, len - r));
                }
                std::cout << " ] ";
                std::cout << "x="<< pc->x << " y=" << pc->y << " pow=" << pc->pow << std::endl; 
            }

            return false;
        }

    operator std::uint64_t() const {return count;}
};

class BuildPermutationsVector {
    unsigned len;
    std::uint64_t count;
    std::vector<std::pair<int,int>>& perms;

    bool show = false;

public:
    explicit BuildPermutationsVector(unsigned l, std::vector< std::pair<int,int> >& t) : len(l), count(0), perms(t) { }

    template <class It> bool operator()(It begin, It end) {
            ++count;

            unsigned r = 0;
            int a = 0;
            int b = 0;
            if (begin != end)
            {
                a = *begin;
                ++r;
                for (++begin; begin != end; ++begin)
                {
                    b = *begin;
                    ++r;
                }
            }

            this->perms.push_back(std::make_pair(a,b));

            return false;
        }

    operator std::uint64_t() const {return count;}
};

std::vector<std::pair<int,int>> getPermutations(std::vector<int>& numbers) {

    const int r = 2;
    const int n = numbers.size();

    std::vector< std::pair<int,int> > perms;

    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    std::uint64_t count = for_each_reversible_permutation(v.begin(),
                                               v.begin() + r,
                                               v.end(),
                                               BuildPermutationsVector(v.size(), perms)); 

    return perms;
}

std::vector<int> find_omp(std::vector<int> numbers) {
    std::vector<int> resp(2);
    resp[0] = 0;
    resp[1] = 0;

    mpz_t pow;

    mpz_init(pow);
    std::vector<std::pair<int,int>> perms = getPermutations(numbers); 

    int i =0;

    //std::cout << "perms size: " << perms.size() << std::endl;

    #pragma omp parallel for private(i) shared(pow)
    for(i=0;i < perms.size() ; i++) {
        int x = perms[i].first;
        int y = perms[i].second;

        mpz_t p1;
        mpz_t p2;

        mpz_init(p1);
        mpz_init(p2);

        mpz_ui_pow_ui(p1,numbers[x],numbers[y]);
        mpz_ui_pow_ui(p2,numbers[y],numbers[x]);

        int cmp_1 = mpz_cmp(p1,p2);

        if (cmp_1 > 0) {
            int cmp_2 = mpz_cmp(p1,pow);
            if (cmp_2>0) {
                #pragma omp critical
                {
                    resp[0] = numbers[x];
                    resp[1] = numbers[y];
                    mpz_set(pow,p1);

                    //std::cout << omp_get_thread_num() << " " << i << " new max " << resp[0] << "^" << resp[1] << std::endl;
                }
            }
        } else {
            int cmp_2 = mpz_cmp(p2,pow);
            if (cmp_2>0) {
                #pragma omp critical
                {
                    resp[1] = numbers[x];
                    resp[0] = numbers[y];
                    mpz_set(pow, p2);

                    //std::cout << omp_get_thread_num() << " " << i << " new max " << resp[0] << "^" << resp[1] << std::endl;
                }
            }
        }
        mpz_clear(p1);
        mpz_clear(p2);
    }

    // WARNING copy the vector!!
    return resp;
}

std::vector<int> find_gmp(std::vector<int> numbers) {
    std::vector<int> resp(2);
    resp[0] = 0;
    resp[1] = 0;

    mpz_t pow;
    mpz_t p1;
    mpz_t p2;

    mpz_init(pow);
    mpz_init(p1);
    mpz_init(p2);

    int x = 0;
    int y = 0;

    std::vector<std::pair<int,int>> perms = getPermutations(numbers); 

    for(auto it=perms.begin();it!=perms.end();it++) {
        x = (*it).first;
        y = (*it).second;

        mpz_ui_pow_ui(p1,numbers[x],numbers[y]);
        mpz_ui_pow_ui(p2,numbers[y],numbers[x]);

        int cmp_1 = mpz_cmp(p1,p2);

        if (cmp_1 > 0) {
            int cmp_2 = mpz_cmp(p1,pow);
            if (cmp_2>0) {
                resp[0] = numbers[x];
                resp[1] = numbers[y];
                mpz_set(pow,p1);

                //std::cout << "new max " << resp[0] << "^" << resp[1] << std::endl;
            }
        } else {
            int cmp_2 = mpz_cmp(p2,pow);
            if (cmp_2>0) {
                resp[1] = numbers[x];
                resp[0] = numbers[y];
                mpz_set(pow, p2);

                //std::cout << "new max " << resp[0] << "^" << resp[1] << std::endl;
            }
        }
    }

    // WARNING copy the vector!!
    return resp;
}

std::vector<int> find(std::vector<int> numbers) {
    std::vector<int> resp(2);
    resp[0] = 0;
    resp[1] = 0;

    // compute permutations

    const int r = 2;
    const int n = numbers.size();
 
    PowerCouple pc(numbers);
      
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    std::uint64_t count = for_each_reversible_permutation(v.begin(),
                                               v.begin() + r,
                                               v.end(),
                                               EvalPowerCouple(v.size(), &pc)); 
    
    // copy the power couple to the response vector
    resp[0] = pc.x;
    resp[1] = pc.y;

    // WARNING copy the vector!!
    return resp;
}

int main() {

    std::cout << "testing PowerCouple.find" << std::endl;

    std::vector<int> numbers1 = {2,3,5,8};
    std::vector<int> numbers2 = {4,9,4,7};
    std::vector<int> numbers3 = {5,9,4,8};
    std::vector<int> numbers4 = {87,18,40,72,83,78,9,29,78,47,70,32,47,47,52,94,30,5,47,2,16,94,46,11,76,27,60,31,33,28,69,30,79,90,24,83,3,3,54,8,76,19,17,23,18,61,87,23,33,81,56,53,73,18,75,58,28,43,86,17,68,67,57,24,77,45,48,1,11,23,74,12,31,67,25,79,74,45,95,83,97,7,2,52,48,91,68,75,21,70,13,90,87,13,69,52,78,99,92,21,40,3,85,36,45,68,97,81,20,66,60,50,94,62,67,84,51,73,89,51,76,81,20,61,77,21,4,83,81,27,8,76,2,6,53,34,53,62,3,29,87,77,89,20,64,36,41,32,62,84,3,87,33,63,19,92,20,71,97,73,19,34,67,63,96,40,10,43,80,84,32,70,80,43,25,73,16,62,55,35,31,5,84,53,2,53,20,49,7,90,70,74,72,34,26,4,7,60,97,21,81,88,37,12,8,40,9,23,4,10,53,6,73,40,53,20,2,51,64,7,87,5,42,52,25,39,68,4,32,47,42,32,91,98,8,23,94,8,17,17,83,70,47,73,97,39,80,78,20,18,13,75,20,86,4,69,86,49,16,78,1,27,94,39,74,84,39,75,0,37,6,0,74,98,23,23,71,89,91,25,38,40,2,91,65,50,55,74,66,81,2,56,43,75,67,35,30,3,50,51,33,37,97,62,31,92,13,82,57,76,81,49,33,7,25,69,57,69,54,48,79,71,81,28,71,80,55,2,23,20,0,40,26,34,76,59,41,12,31,9,36,33,69,37,30,26,27,21,47,83,5,33,22,90,2,99,31,77,34,27,86,96,45,45,4,72,19,86,50,90,55,41,74,73,6,2,74,19,65,14,84,37,54,55,90,26,14,59,49,79,1,19,53,89,9,33,18,22,84,71,58,88,99,95,93,8,36,14,34,2,62,81,82,63,55,82,26,26,48,90,91,69,30,29,89,75,42,57,29,7,80,57,63,18,56,99,88,22,12,34,94,80,78,92,41,59,74,26,61,70,94,10,31,59,79,44,63,58,9,79,84,25,94,11,22,46,92,62,92,34,33,10,30,90,42,6,9,86,87,46,18,9,40,41,26,36,29,60,16,2,36,94,80,59,27,46,1,20,47,76};

    // generate a big array for testing
    int max5 = 10000;
    int max_rnd = 5000;
    std::vector<int> numbers5;
    srand(time(NULL)); // initialize the RNG

    for(int i=0;i<max5;i++) {
        int num = rand() % max_rnd;
        numbers5.push_back(num);
    }

    std::vector<int> resp1 = find(numbers1);
    std::cout << resp1[0] << " " << resp1[1] << std::endl;

    std::vector<int> resp2 = find(numbers2);
    std::cout << resp2[0] << " " << resp2[1] << std::endl;

    std::vector<int> resp3 = find(numbers3);
    std::cout << resp3[0] << " " << resp3[1] << std::endl;

    std::vector<int> resp4 = find(numbers4);
    std::cout << resp4[0] << " " << resp4[1] << std::endl;

    std::vector<int> resp5 = find_gmp(numbers5);
    std::cout << resp5[0] << " " << resp5[1] << std::endl;

    std::vector<int> resp6 = find_omp(numbers5);
    std::cout << resp6[0] << " " << resp6[1] << std::endl;


}
