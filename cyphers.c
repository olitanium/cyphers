#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define RSAINT uintmax_t
#define RSAPRINT "%" PRIuMAX

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

void caesar(char first, char second, const char* restrict text, char* restrict destination) {
    int difference = second - first;
    int text_length = strlen(text);
    for (
        int i = 0;
        i < text_length;
        i++
    )
    {
        destination[i] = positive_modulo(
            tolower( text[i]) - (int) 'a' // turn to integer offset from 'a'
                + difference // subtract difference
            , 26
        ) // modulo 26
        + 'a'; // recast to letter
    }
    destination[text_length] = '\0';
}

void vigenere(const char* restrict keyword, const char* restrict text, char* restrict destination, int decode) {
    int text_length = strlen(text);
    int keyword_length = strlen(keyword);
    int sign = decode == 0 ? +1 : -1; // if encoding, use +, if decoding, use -
    for (
        int i = 0;
        i < text_length;
        i++
        )
    {
        destination[i] = positive_modulo(
            tolower(text[i])
                - 'a'
                + sign * (
                    tolower(keyword[
                        positive_modulo(i, keyword_length)
                    ])
                    - 'a'
                )
            , 26
        ) + 'a';
    }
    destination[text_length] = '\0';
}

RSAINT modular_pow(RSAINT base, RSAINT exponent, RSAINT modulus) {
    if (modulus == 1)
        return 0;
    RSAINT result = 1;
    base = base % modulus;
    while (exponent > 0)  {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
   
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

unsigned int RSA(RSAINT message, RSAINT n, RSAINT e) {
    return modular_pow(message, e, n);
}

struct RSA_key_trio {
    RSAINT n_public_private;
    RSAINT e_public;
    RSAINT d_private;
};

RSAINT gcd(RSAINT a, RSAINT b) {
    while (a != b) {
        if (a > b) {
            a = a % b;
        }
        else {
            b = b % a;
        }
    }
    return a;
}

RSAINT lcm(RSAINT a, RSAINT b) {
    RSAINT div = a / gcd(a, b);
    return div * b;
}

RSAINT prime_list[] = { 3, 5, 7, 11, 13, 17 };

struct RSA_key_trio
RSA_gen_keys(RSAINT p, RSAINT q) {
    RSAINT n = p * q;
    printf(RSAPRINT"\n", n);
    
    RSAINT totient = lcm(p - 1, q - 1);
    printf(RSAPRINT"\n", totient);
    
    RSAINT e = 3;
    size_t index = 0;
    while( positive_modulo(totient, e) != 0 ) {
        e = prime_list[++index];
    }
    printf(RSAPRINT"\n", e);



    struct RSA_key_trio result = { n, 1, 1 };
    return result;
}



int main() {
    RSA_gen_keys(61, 53);

//    RSAINT message = 65;
//    RSAINT cyphertext = RSA(65, 3233, 17);
//    RSAINT messagetext = RSA(cyphertext, 3233, 413);
  
//    printf(RSAPRINT " -> " RSAPRINT " -> " RSAPRINT, message, cyphertext, messagetext);

    return 0;
}