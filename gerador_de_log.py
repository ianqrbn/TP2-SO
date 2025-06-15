import random
import os

NUMERO_DE_ACESSOS = 50000

# --- Perfis de programas com seus padrões de acesso ---
PERFIS = {
    "compilador.log": {
        "regioes": [
            (0x10000000, 1024 * 32),   # Pilha maior (muita recursão)
            (0x7ffde000, 1024 * 64),   # Dados estáticos variados
            (0x00400000, 1024 * 256),  # Heap grande
        ],
        "enderecos_quentes": [0x10000200, 0x7ffde010, 0x00401abc],
        "pesos_acesso": [0.3, 0.4, 0.3],  # Mais quente que aleatório
    },
    "matriz.log": {
        "regioes": [
            (0x20000000, 1024 * 1024),  # Grande matriz contínua (1MB)
        ],
        "enderecos_quentes": [],
        "pesos_acesso": [0.9, 0.1, 0.0],  # Muito sequencial
    },
    "compressor.log": {
        "regioes": [
            (0x30000000, 1024 * 64),   # Buffer de entrada
            (0x30010000, 1024 * 64),   # Buffer de saída
        ],
        "enderecos_quentes": [0x30000004, 0x30010008],
        "pesos_acesso": [0.2, 0.5, 0.3],  # Padrão repetitivo
    },
    "simulador.log": {
        "regioes": [
            (0x40000000, 1024 * 256),  # Vetores de partículas
            (0x40040000, 1024 * 64),   # Constantes físicas, buffers
        ],
        "enderecos_quentes": [0x40040010, 0x40040014],
        "pesos_acesso": [0.4, 0.5, 0.1],  # Misto, com predominância local
    },
}


def gerar_log(nome_arquivo, regioes, enderecos_quentes, pesos_acesso):
    with open(nome_arquivo, "w") as f:
        for i in range(NUMERO_DE_ACESSOS):
            tipo_acesso = random.choices(
                population=["sequencial", "aleatorio_local", "quente"],
                weights=pesos_acesso,
                k=1
            )[0]

            addr = 0

            if tipo_acesso == 'sequencial':
                regiao_base, tamanho = random.choice(regioes)
                offset_inicial = random.randint(0, tamanho - 256)
                addr = regiao_base + offset_inicial + (i % 64) * 4

            elif tipo_acesso == 'aleatorio_local':
                regiao_base, tamanho = random.choice(regioes)
                offset = random.randint(0, tamanho - 4)
                addr = regiao_base + (offset & ~3)

            elif tipo_acesso == 'quente' and enderecos_quentes:
                addr = random.choice(enderecos_quentes)

            rw = random.choices(population=['R', 'W'], weights=[0.75, 0.25], k=1)[0]
            f.write(f"{addr:08x} {rw}\n")

    print(f"{nome_arquivo} gerado com {NUMERO_DE_ACESSOS} acessos.")


def gerar_todos_logs():
    for nome_arquivo, perfil in PERFIS.items():
        gerar_log(
            nome_arquivo,
            perfil["regioes"],
            perfil["enderecos_quentes"],
            perfil["pesos_acesso"]
        )


if __name__ == "__main__":
    gerar_todos_logs()
