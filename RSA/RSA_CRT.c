#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"

unsigned long long gcdExtended(unsigned long long a, unsigned long long b,  long long *x,  long long *y);
unsigned long long modInverse(unsigned long long a, unsigned long long m);
unsigned long long modInverse1(unsigned long long a, unsigned long long m);
unsigned long long modInverse2(unsigned long long a, unsigned long long m);
unsigned long long modInverse3(unsigned long long a, unsigned long long m);
void modInverse4(mpz_t res, mpz_t aTmp, mpz_t mTmp);

// To find GCD of a and b
unsigned long long gcd(unsigned long long a, unsigned long long b);
// To compute x raised to power y under modulo m
unsigned long long power(unsigned long long x, unsigned long long y, unsigned long long m);

int main(int argc, char *argv[])
{
	mpz_t N1, N2, N3;
	mpz_init(N1);
	mpz_init(N2);
	mpz_init(N3);
	mpz_set_str(N1, "5356488760553659", 10);
	mpz_set_str(N2, "8021928613673473", 10);
	mpz_set_str(N3, "56086910298885139", 10);
	// ---- TEST CASE ----
	// Test with N1, N2, N3 = 3, 5, 7
	// Test with C1, C2, C3 = 2, 3, 2
	// x = 2 (mod 3)
	// x = 3 (mod 5)
	// x = 2 (mod 7)
	// result would be 23 -> x

	mpz_t C1, C2, C3;
	mpz_init(C1);
	mpz_init(C2);
	mpz_init(C3);
	mpz_set_str(C1, "4324345136725864", 10);
	mpz_set_str(C2, "2102800715763550", 10);
	mpz_set_str(C3, "46223668621385973", 10);

	mpz_t M1, M2, M3;
	mpz_init(M1);
	mpz_init(M2);
	mpz_init(M3);
	mpz_mul(M1, N2, N3);
	mpz_mul(M2, N1, N3);
	mpz_mul(M3, N1, N2);

	mpz_t R1, R2, R3;
	mpz_init(R1);
	mpz_init(R2);
	mpz_init(R3);
	modInverse4(R1, M1, N1);
	modInverse4(R2, M2, N2);
	modInverse4(R3, M3, N3);

	mpz_mul(R1, R1, C1);
	mpz_mul(R1, R1, M1);

	mpz_mul(R2, R2, C2); mpz_mul(R2, R2, M2);
	mpz_mul(R3, R3, C3); mpz_mul(R3, R3, M3);
	mpz_t x;
	mpz_init(x);
	mpz_add(x, R1, R2);
	mpz_add(x, x, R3);

	mpz_mul(N1, N1, N2); mpz_mul(N1, N1, N3);
	mpz_mod(x, x, N1);
	mpz_root(x, x, 3);

	printf("C = M ** e mod N\n");
	gmp_printf("M = %Zd\n", x);

	mpz_clear(N1); mpz_clear(N2); mpz_clear(N3);
	mpz_clear(C1); mpz_clear(C2); mpz_clear(C3);
	mpz_clear(M1); mpz_clear(M2); mpz_clear(M3);
	mpz_clear(R1); mpz_clear(R2); mpz_clear(R3);
	mpz_clear(x);

	return 0;
}

unsigned long long modInverse(unsigned long long a, unsigned long long m)
{
	 long long x, y;
	unsigned long long g = gcdExtended(a, m, &x, &y);
	unsigned long long res = 0;
	if (g != 1)
	{
		printf("Inverse doesn't exist\n");
	}
	else
	{
		printf("g = %llu. x = %lld, y = %lld, m = %llu\n", g, x, y, m);
		res = (x % m + m) % m;
		printf("Modular multiplicative inverse is %llu\n", res);
	}

	return res;
}

unsigned long long gcdExtended(unsigned long long a, unsigned long long b,  long long *x,  long long *y)
{
	// Bas Case
	if (a == 0)
	{
		*x = 0;
		*y = 1;
		return b;
	}

	 long long x1, y1;
	unsigned long long gcd = gcdExtended(b % a, a, &x1, &y1);

	printf("GCD = %llu, a = %llu, b = %llu, x1 = %lld, y1 = %lld \n", gcd, a, b, x1, y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

unsigned long long modInverse1(unsigned long long a, unsigned long long m)
{
    unsigned long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1)
      return 0;

    while (a > 1)
    {
        // q is quotient
        q = a / m;
        t = m;

        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    // Make x1 positive
    if (x1 < 0)
       x1 += m0;

    return x1;
}

void modInverse4(mpz_t res, mpz_t aTmp, mpz_t mTmp)
{
	// version of big-number (compared with modInverse1)
	mpz_t a;
	mpz_init_set(a, aTmp);

	mpz_t m;
	mpz_init_set(m, mTmp);

    mpz_t m0;
	mpz_init_set(m0, m);

	mpz_t x0;
	mpz_init(x0);

	mpz_t x1;
	mpz_init(x1);
	mpz_init_set_ui(x1, 1);
	if (mpz_cmp_ui(m, 1) == 0)
	{
		mpz_clear(a);
		mpz_clear(m);
		mpz_clear(m0);
		mpz_clear(x0);
		mpz_clear(x1);
		mpz_set_ui(res, 0);
		return;
	}

	mpz_t q;
	mpz_init(q);
	mpz_t t;
	mpz_init(t);
	while (mpz_cmp_ui(a, 1) > 0)
	{
		// q = a / m;
		mpz_tdiv_q(q, a, m);

		// t = m;
		mpz_set(t, m);

		// m = a % m;
		mpz_mod(m, a, m);

		// a = t;
		mpz_set(a, t);

		// t = x0
		mpz_set(t, x0);

		// x0 = x1 - q * x0;
		// x1 = t;
		mpz_submul(x1, q, x0);
		mpz_set(x0, x1);
		mpz_set(x1, t);
	}

	// Make x1 positive
	if (mpz_cmp_ui(x1, 0) < 0)
		mpz_add(x1, x1, m0);

	mpz_clear(a);
	mpz_clear(m);
	mpz_clear(m0);
	mpz_clear(x0);
	mpz_clear(q);
	mpz_clear(t);

	mpz_set(res, x1);
	mpz_clear(x1);

	return;
}

unsigned long long modInverse2(unsigned long long a, unsigned long long m)
{
    a = a % m;
    for (unsigned long long x = 1ULL; x < m; x++)
       if ((a * x) % m == 1)
          return x;

    return 0;
}

unsigned long long modInverse3(unsigned long long a, unsigned long long m)
{
    unsigned long long g = gcd(a, m);
    unsigned long long res = 0;
    if (g != 1)
    {
        printf("Inverse doesn't exist\n");
    }
    else
    {
        // If a and m are relatively prime, then modulo inverse
        // is a^(m-2) mode m
        res = power(a, m - 2, m);
    }

    return res;
}

// To compute x^y under modulo m
unsigned long long power(unsigned long long a, unsigned long long b, unsigned long long MOD)
{
    // if (y == 0)
    //     return 1;

    // unsigned long long p = power(x, y / 2, m) % m;
    // p = (p * p) % m;

    // return (y % 2 == 0) ? p : (x * p) % m;

     unsigned   long long x=1,y=a;
    while(b > 0)
    {
        if(b%2 == 1)
        {
            x=(x*y);
            if(x>MOD) x%=MOD;
        }
        y = (y*y);
        if(y>MOD) y%=MOD;
        b /= 2;
    }
    return x;
}

// Function to return gcd of a and b
unsigned long long gcd(unsigned long long a, unsigned long long b)
{
    if (a == 0)
        return b;

    return gcd(b % a, a);
}
