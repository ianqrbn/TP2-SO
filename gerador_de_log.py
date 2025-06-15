import random

# --- Parâmetros da Simulação ---
NOME_ARQUIVO_SAIDA = "meu_programa.log"
NUMERO_DE_ACESSOS = 50000


REGIOES_DE_MEMORIA = [
    (0x10000000, 1024 * 8),    # Uma pilha de 8 KB
    (0x7ffde000, 1024 * 16),   # Dados globais de 16 KB
    (0x00400000, 1024 * 128),  # Uma grande área de 'heap' ou dados de 128 KB
]

# Lista de endereços "quentes" (localidade temporal)
# São endereços específicos que serão acessados repetidamente.
ENDERECOS_QUENTES = [
    0x10000148,  # Variável de contador em um laço
    0x7ffde010,  # Ponteiro ou variável global importante
    0x7ffde014,
    0x00401abc,  # Objeto frequentemente utilizado no heap
]

def gerar_log():
    """Gera o arquivo de log com acessos à memória."""
    with open(NOME_ARQUIVO_SAIDA, "w") as f:
        for i in range(NUMERO_DE_ACESSOS):
            # Escolhe o tipo de acesso aleatoriamente
            tipo_acesso = random.choices(
                population=['sequencial', 'aleatorio_local', 'quente'],
                weights=[0.3, 0.5, 0.2], # 30% sequencial, 50% aleatório local, 20% quente
                k=1
            )[0]

            addr = 0

            if tipo_acesso == 'sequencial':
                # Simula varredura de um vetor ou buffer
                regiao_base, tamanho = random.choice(REGIOES_DE_MEMORIA)
                # Pega um ponto de partida aleatório dentro da região
                offset_inicial = random.randint(0, tamanho - 256)
                # Avança um pouco a cada passo (simulando i++, por exemplo)
                addr = regiao_base + offset_inicial + (i % 64) * 4

            elif tipo_acesso == 'aleatorio_local':
                # Simula acessos aleatórios mas dentro de uma região "conhecida"
                regiao_base, tamanho = random.choice(REGIOES_DE_MEMORIA)
                offset = random.randint(0, tamanho - 4)
                # Garante alinhamento de 4 bytes
                addr = regiao_base + (offset & ~3)

            elif tipo_acesso == 'quente':
                # Acessa um dos endereços "quentes"
                addr = random.choice(ENDERECOS_QUENTES)

            # Define se a operação é Leitura (R) ou Escrita (W)
            # Leituras são geralmente mais comuns que escritas
            rw = random.choices(population=['R', 'W'], weights=[0.75, 0.25], k=1)[0]

            # Escreve a linha no arquivo no formato hexadecimal
            f.write(f"{addr:08x} {rw}\n")

    print(f"Arquivo '{NOME_ARQUIVO_SAIDA}' gerado com sucesso com {NUMERO_DE_ACESSOS} linhas.")

if __name__ == "__main__":
    gerar_log()