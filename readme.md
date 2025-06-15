# Gerenciador de Memória Virtual

Este projeto simula o gerenciamento de memória virtual com três algoritmos de substituição de páginas: **LRU**, **NRU** e **Segunda Chance**. Ele processa um log de acessos à memória e calcula *page faults* e páginas escritas.

---

## Compilação

Use o `Makefile` fornecido. Para compilar o programa, basta executar:

make

## Execução

Execute o programa com o seguinte comando:
./tp2virtual <algoritmo> <arquivo.log> <tam_pagina_kb> <tam_memoria_kb> [debug]

### Exemplo de execução

./tp2virtual segunda_chance compressor.log 4 128
