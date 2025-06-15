#ifndef TPI_H
#define TPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "estruturas.h"

// Função de Hash
unsigned int hash_function(int page_number, int num_quadros_param)
{
    return page_number % num_quadros_param;
}

// Encontra uma página na TPI. Retorna o índice do quadro ou -1 se não encontrar.
int encontrar_pagina(int page_number, NoTPI **tabela_de_paginas_invertida_param, int num_quadros_param)
{
    unsigned int index = hash_function(page_number, num_quadros_param);
    NoTPI *no_atual = tabela_de_paginas_invertida_param[index];
    while (no_atual != NULL)
    {
        if (no_atual->numero_pagina == page_number)
        {
            return no_atual->indice_quadro;
        }
        no_atual = no_atual->proximo;
    }
    return -1;
}

// Insere uma nova página na TPI
void inserir_pagina(int page_number, int quadro_index, NoTPI **tabela_de_paginas_invertida_param, int num_quadros_param)
{
    unsigned int index = hash_function(page_number, num_quadros_param);
    NoTPI *novo_no = (NoTPI *)malloc(sizeof(NoTPI));
    if (!novo_no)
    {
        perror("Falha ao alocar memória para o nó da TPI");
        exit(EXIT_FAILURE); // Sai do programa em caso de falha crítica
    }
    novo_no->numero_pagina = page_number;
    novo_no->indice_quadro = quadro_index;
    novo_no->proximo = tabela_de_paginas_invertida_param[index];
    tabela_de_paginas_invertida_param[index] = novo_no;
}

// Remove uma página da TPI
void remover_pagina(int page_number, NoTPI **tabela_de_paginas_invertida_param, int num_quadros_param)
{
    unsigned int index = hash_function(page_number, num_quadros_param);
    NoTPI *no_atual = tabela_de_paginas_invertida_param[index];
    NoTPI *anterior = NULL;

    while (no_atual != NULL && no_atual->numero_pagina != page_number)
    {
        anterior = no_atual;
        no_atual = no_atual->proximo;
    }

    if (no_atual == NULL)
        return;

    if (anterior == NULL)
    {
        tabela_de_paginas_invertida_param[index] = no_atual->proximo;
    }
    else
    {
        anterior->proximo = no_atual->proximo;
    }
    free(no_atual);
}
#endif