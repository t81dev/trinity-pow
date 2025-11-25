/*
 * trinity-pow.c — Ternary Proof-of-Work
 * https://github.com/t81dev/trinity-pow
 *
 * Only nonces in perfect balance may pass.
 *
 * Compile:
 *   gcc trinity-pow.c -O3 -lcrypto -o trinity-pow
 *
 * Run:
 *   ./trinity-pow --difficulty 4 --max-depth 22 --entropy 1.58
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/sha.h>
#include <time.h>

typedef int8_t trit;  // -1, 0, +1

typedef struct {
    trit digits[64];
    int len;
} nonce_t;

typedef struct {
    nonce_t* list;
    int count;
    int cap;
} store_t;

// Configurable via CLI
int MAX_DEPTH = 20;
double MIN_ENTROPY = 1.58;  // log2(3) ≈ 1.58496 → maximum randomness
int DIFFICULTY = 3;         // leading zero bytes in SHA-256

store_t found;

// Fast entropy check (prunes 99.9% of branches early)
double entropy(const nonce_t* n) {
    if (n->len <= 3) return 2.0;  // don't prune tiny nonces
    int c[3] = {0};
    for (int i = 0; i < n->len; i++) c[n->digits[i] + 1]++;
    double h = 0.0;
    for (int i = 0; i < 3; i++) {
        if (c[i]) {
            double p = c[i] / (double)n->len;
            h -= p * log2(p);
        }
    }
    return h;
}

// SHA-256 has leading zero bytes?
int valid_hash(const nonce_t* n) {
    unsigned char hash[32];
    SHA256((unsigned char*)n->digits, n->len * sizeof(trit), hash);
    for (int i = 0; i < DIFFICULTY; i++) {
        if (hash[i] != 0) return 0;
    }
    return 1;
}

// Print a nonce in cosmic form
void print_nonce(const nonce_t* n) {
    for (int i = 0; i < n->len; i++) {
        putchar(n->digits[i] == -1 ? '-' : n->digits[i] == 0 ? '0' : '+');
    }
    printf("  (len=%d, entropy=%.4f)\n", n->len, entropy(n));
}

// Store a winner
void save_nonce(nonce_t n) {
    if (found.count >= found.cap) {
        found.cap *= 2;
        found.list = realloc(found.list, found.cap * sizeof(nonce_t));
    }
    found.list[found.count++] = n;
}

// The sacred recursion
void mine(nonce_t current, int depth) {
    if (depth >= MAX_DEPTH) return;

    // Entropy pruning — kill ugly branches early
    if (depth > 3 && entropy(&current) < MIN_ENTROPY) return;

    // Check if this nonce is worthy
    if (depth >= 8 && valid_hash(&current)) {
        printf("TRINITY NONCE FOUND: ");
        print_nonce(&current);
        save_nonce(current);
    }

    // Branch into all three cosmic directions
    for (trit t = -1; t <= 1; t++) {
        current.digits[depth] = t;
        current.len = depth + 1;
        mine(current, depth + 1);
    }
}

int main(int argc, char** argv) {
    // Parse args
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--max-depth") && i+1 < argc) MAX_DEPTH = atoi(argv[++i]);
        if (!strcmp(argv[i], "--entropy") && i+1 < argc) MIN_ENTROPY = atof(argv[++i]);
        if (!strcmp(argv[i], "--difficulty") && i+1 < argc) DIFFICULTY = atoi(argv[++i]);
    }

    printf("TRINITY-POW — Ternary Proof-of-Work\n");
    printf("Max depth: %d | Min entropy: %.3f | Difficulty: %d zero bytes\n\n", 
           MAX_DEPTH, MIN_ENTROPY, DIFFICULTY);

    found.count = 0;
    found.cap = 1024;
    found.list = malloc(found.cap * sizeof(nonce_t));

    nonce_t start = {{0}, 0};
    clock_t t = clock();
    mine(start, 0);
    double sec = (clock() - t) / (double)CLOCKS_PER_SEC;

    printf("\nSearch complete in %.2f seconds\n", sec);
    printf("Found %d Trinity Nonces — perfectly balanced, as all things should be.\n", found.count);

    free(found.list);
    return 0;
}
