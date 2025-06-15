#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdbool.h>

#define NRU_RESET_INTERVAL 1000

typedef struct
{
    bool ocupado;
    bool sujo;
    bool referenciado;
    int numero_pagina;
    unsigned long timestamp_acesso; // Usa-se somente no LRU
} Quadro;

typedef struct NoTPI
{
    int numero_pagina;
    int indice_quadro;
    struct NoTPI *proximo;
} NoTPI;

#endif