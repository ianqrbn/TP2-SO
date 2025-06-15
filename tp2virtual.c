#include "estruturas.h"
#include "tpi.h"
#include "substituicao.h"

// Declaração das variaveis globais
Quadro *tabela_de_quadros = NULL;
NoTPI **tabela_de_paginas_invertida = NULL;
int num_quadros = 0;
long contador_acessos = 0;
int ponteiro_relogio = 0;

int main(int argc, char *argv[])
{

    if (argc < 5)
    {
        fprintf(stderr, "Uso: %s <algoritmo> <arquivo.log> <tam_pagina_kb> <tam_memoria_kb> [debug]\n", argv[0]);
        fprintf(stderr, "Algoritmos disponíveis: lru, nru, segunda_chance\n");
        return 1;
    }

    char *alg_nome = argv[1];
    char *nome_arquivo = argv[2];
    int tam_pagina_kb = atoi(argv[3]);
    int tam_memoria_kb = atoi(argv[4]);
    bool debug_mode = (argc > 5);

    // Calcula o numero de quadros
    unsigned int tam_pagina_bytes = tam_pagina_kb * 1024;
    num_quadros = (tam_memoria_kb * 1024) / tam_pagina_bytes;

    unsigned int s = 0;
    unsigned int tmp = tam_pagina_bytes;
    while (tmp > 1)
    {
        tmp = tmp >> 1;
        s++;
    }

    tabela_de_quadros = (Quadro *)calloc(num_quadros, sizeof(Quadro));
    tabela_de_paginas_invertida = (NoTPI **)calloc(num_quadros, sizeof(NoTPI *));
    if (!tabela_de_quadros || !tabela_de_paginas_invertida)
    {
        perror("Falha ao alocar memória para as tabelas");
        return 1;
    }

    FILE *arquivo_log = fopen(nome_arquivo, "r");
    if (!arquivo_log)
    {
        perror("Erro ao abrir o arquivo de log");
        return 1;
    }

    // Simulação
    printf("Executando o simulador...\n");

    unsigned long page_faults = 0;
    unsigned long paginas_escritas = 0;
    int quadros_ocupados = 0;

    unsigned int addr;
    char rw;

    while (fscanf(arquivo_log, "%x %c", &addr, &rw) == 2)
    {
        contador_acessos++;

        int numero_pagina = addr >> s;

        // Reseta o bit R periodicamente
        if (strcmp(alg_nome, "nru") == 0 && (contador_acessos % NRU_RESET_INTERVAL == 0))
        {
            for (int i = 0; i < num_quadros; i++)
            {
                if (tabela_de_quadros[i].ocupado)
                {
                    tabela_de_quadros[i].referenciado = false;
                }
            }
        }

        int indice_quadro = encontrar_pagina(numero_pagina, tabela_de_paginas_invertida, num_quadros);

        if (indice_quadro != -1)
        { // HIT
            if (debug_mode)
                printf("%lu: HIT | Página %d no quadro %d\n", contador_acessos, numero_pagina, indice_quadro);
            tabela_de_quadros[indice_quadro].referenciado = true;
            tabela_de_quadros[indice_quadro].timestamp_acesso = contador_acessos;
            if (rw == 'W')
            {
                tabela_de_quadros[indice_quadro].sujo = true;
            }
        }
        else
        { // PAGE FAULT
            page_faults++;
            int quadro_alvo = -1;

            if (quadros_ocupados < num_quadros)
            {
                quadro_alvo = quadros_ocupados;
                quadros_ocupados++;
                if (debug_mode)
                    printf("%lu: FAULT | Página %d carregada no quadro livre %d\n", contador_acessos, numero_pagina, quadro_alvo);
            }
            else
            {
                if (strcmp(alg_nome, "lru") == 0)
                    quadro_alvo = substituicao_lru(tabela_de_quadros, num_quadros);
                else if (strcmp(alg_nome, "nru") == 0)
                    quadro_alvo = substituicao_nru(tabela_de_quadros, num_quadros);
                else
                    quadro_alvo = substituicao_segunda_chance(tabela_de_quadros, num_quadros, &ponteiro_relogio);

                if (debug_mode)
                    printf("%lu: FAULT | Página %d substitui pág %d no quadro %d\n", contador_acessos, numero_pagina, tabela_de_quadros[quadro_alvo].numero_pagina, quadro_alvo);

                if (tabela_de_quadros[quadro_alvo].sujo)
                {
                    paginas_escritas++;
                }

                remover_pagina(tabela_de_quadros[quadro_alvo].numero_pagina, tabela_de_paginas_invertida, num_quadros);
            }

            // Carrega uma nova pagina
            tabela_de_quadros[quadro_alvo].ocupado = true;
            tabela_de_quadros[quadro_alvo].numero_pagina = numero_pagina;
            tabela_de_quadros[quadro_alvo].sujo = (rw == 'W');
            tabela_de_quadros[quadro_alvo].referenciado = true;
            tabela_de_quadros[quadro_alvo].timestamp_acesso = contador_acessos;

            inserir_pagina(numero_pagina, quadro_alvo, tabela_de_paginas_invertida, num_quadros);
        }
    }

    // Relatório
    printf("-------------------------------------------\n");
    printf("Arquivo de entrada: %s\n", nome_arquivo);
    printf("Tamanho da memoria: %d KB\n", tam_memoria_kb);
    printf("Tamanho das paginas: %d KB\n", tam_pagina_kb);
    printf("Tecnica de reposicao: %s\n", alg_nome);
    printf("Numero total de acessos: %lu\n", contador_acessos);
    printf("Total de page faults: %lu\n", page_faults);
    printf("Total de paginas escritas: %lu\n", paginas_escritas);
    printf("-------------------------------------------\n");

    fclose(arquivo_log);
    free(tabela_de_quadros);

    for (int i = 0; i < num_quadros; i++)
    {
        NoTPI *no_atual = tabela_de_paginas_invertida[i];
        while (no_atual != NULL)
        {
            NoTPI *temp = no_atual;
            no_atual = no_atual->proximo;
            free(temp);
        }
    }
    free(tabela_de_paginas_invertida);

    return 0;
}