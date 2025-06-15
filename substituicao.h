#ifndef SUBSTITUICAO_H
#define SUBSTITUICAO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "estruturas.h"

int substituicao_lru(Quadro *tabela_quadros, int num_quadros)
{
    int id_substituicao = -1;
    unsigned long menor_timestamp = ULONG_MAX;

    // Encontra o quadro com o menor timestamp de acesso
    for (int i = 0; i < num_quadros; ++i)
    {
        if (tabela_quadros[i].timestamp_acesso < menor_timestamp)
        {
            menor_timestamp = tabela_quadros[i].timestamp_acesso;
            id_substituicao = i;
        }
    }
    return id_substituicao;
}

int substituicao_nru(Quadro *tabela_quadros, int num_quadros)
{

    // Busca o quadro seguindo a prioridade das classes
    for (int R = 0; R < 2; ++R)
    {
        for (int S = 0; S < 2; ++S)
        {
            for (int i = 0; i < num_quadros; ++i)
            {
                if (tabela_quadros[i].referenciado == R && tabela_quadros[i].sujo == S)
                {
                    return i;
                }
            }
        }
    }
    return 0;
}

int substituicao_segunda_chance(Quadro *tabela_quadros, int num_quadros, int *ponteiro_relogio_ptr)
{
    while (true)
    {
        if (!tabela_quadros[*ponteiro_relogio_ptr].referenciado)
        {
            int id_substituicao = *ponteiro_relogio_ptr;
            *ponteiro_relogio_ptr = (*ponteiro_relogio_ptr + 1) % num_quadros;
            return id_substituicao;
        }

        // Dá uma segunda chance
        tabela_quadros[*ponteiro_relogio_ptr].referenciado = false;
        *ponteiro_relogio_ptr = (*ponteiro_relogio_ptr + 1) % num_quadros;
    }
}
#endif