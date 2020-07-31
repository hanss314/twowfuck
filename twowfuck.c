#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sha1/sha1.h"

#define ARR_INIT_SIZE 128

typedef struct infarray {
    int * left;
    int * right;
    size_t leftlen;
    size_t rightlen;
} InfArray;

InfArray * init_array(){
    InfArray * arr = malloc(sizeof(InfArray));
    arr->leftlen = ARR_INIT_SIZE;
    arr->rightlen = ARR_INIT_SIZE;
    arr->left = calloc(ARR_INIT_SIZE, sizeof(int));
    arr->right = calloc(ARR_INIT_SIZE, sizeof(int));
    return arr;
}

void ensure_size(int ** arr, size_t * size, int ind){
    while(ind >= *size){
        size_t i = *size;
        *size *= 2;
        *arr = realloc(*arr, *size);
        for(; i<*size; i++){
            *arr[i] = 0;
        }
    }
}

int get_access(InfArray * a, long long int ind, int ***arr, size_t **size){
    int ** arr_access;
    size_t * size_access;
    if(ind>=0){
        arr_access = &a->right;
        size_access = &a->rightlen;
    }
    else{
        arr_access = &a->left;
        size_access = &a->leftlen;
        ind = 1-ind;
    }
    *arr = arr_access;
    *size = size_access;
    return ind;
}

int get_value(InfArray * a, long long int index){
    int ** arr_access;
    size_t * size_access;
    index = get_access(a, index, &arr_access, &size_access);
    ensure_size(arr_access, size_access, index);
    return *arr_access[index];
}

void set_value(InfArray * a, long long int index, int value){
    int ** arr_access;
    size_t * size_access;
    index = get_access(a, index, &arr_access, &size_access);
    ensure_size(arr_access, size_access, index);
    (*arr_access)[index] = value;
}

void increment(InfArray * a, long long int index){
    int ** arr_access;
    size_t * size_access;
    index = get_access(a, index, &arr_access, &size_access);
    ensure_size(arr_access, size_access, index);
    (*arr_access)[index]++;
}

void decrement(InfArray * a, long long int index){
    int ** arr_access;
    size_t * size_access;
    index = get_access(a, index, &arr_access, &size_access);
    ensure_size(arr_access, size_access, index);
    (*arr_access)[index]--;
}

void interpret(const char * program, const char * args){
    size_t pptr = 0;
    size_t inptr = 0;
    long long int mptr = 0;
    
    size_t proglen = strlen(program);
    size_t inplen = strlen(args);

    char instr;
    InfArray * mem = init_array();

    SHA1_CTX sha;
    unsigned char digest[20];
    unsigned long long dest;

    

    while(pptr < proglen && (instr=program[pptr]) != '\0'){
        switch(instr%7){
        case 0:
            mptr -= 1; break;
        case 1:
            mptr += 1; break;
        case 2:
            increment(mem, mptr); break;
        case 3:
            decrement(mem, mptr); break;
        case 4:
            set_value(mem, mptr, args[inptr]); 
            inptr++; inptr %= inplen+1;
            break;
        case 5:
            putchar(get_value(mem, mptr)); break;
        case 6:
        default:
            if(get_value(mem, mptr) == 0){
                pptr += 2; break;
            }
            SHA1Init(&sha);
            SHA1Update(&sha, (const unsigned char*)&program[pptr], proglen-pptr);
            SHA1Final(digest, &sha);
            dest = 0;
            for(int i=0; i<20; i++){
                dest |= digest[i]; dest <<= 8;
            }
            printf("%lld\n", dest);
            dest %= proglen;
            pptr = dest-1;
        }
        pptr++;
    }
}

int main(int argc,  char **argv){
    if(argc < 2 || argc > 3){
        printf(
            "Usage: %s <program> [input]\n"
            "Reading produces a null terminator when the end is reached.\n"
            "If input does not exist or is empty, reading will always produce a null terminator\n",
            argv[0]
        );
        return 1;
    }
    if(argc == 3) interpret(argv[1], argv[2]);
    else interpret(argv[1], "");
}
