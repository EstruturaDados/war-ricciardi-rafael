// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct que representa um território do jogo War
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ─── Missões pré-definidas ────────────────────────────────────────────────────

#define TOTAL_MISSOES 5
char* missoes[TOTAL_MISSOES] = {
    "Conquistar 3 territorios com mais de 5 tropas cada",
    "Eliminar todas as tropas da cor vermelha",
    "Dominar ao menos 4 territorios consecutivos",
    "Ter o maior exercito do mapa (mais tropas no total)",
    "Conquistar 2 territorios inimigos em um unico turno"
};

// ─── Funções de missão ────────────────────────────────────────────────────────

// Sorteia uma missão e copia para o destino alocado dinamicamente (por referência)
void atribuirMissao(char* destino, char* lista[], int total) {
    int idx = rand() % total;
    strcpy(destino, lista[idx]);
}

// Exibe a missão do jogador (por valor — apenas leitura)
void exibirMissao(char* missao, char* cor) {
    printf("\n🎯 Missão do jogador [%s]: %s\n", cor, missao);
}

// Verifica se a missão foi cumprida (lógica simplificada)
// Retorna 1 se venceu, 0 caso contrário
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* cor) {
    // Missão 1: 3 territórios com mais de 5 tropas
    if (strstr(missao, "3 territorios com mais de 5")) {
        int count = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, cor) == 0 && mapa[i].tropas > 5)
                count++;
        return count >= 3;
    }
    // Missão 2: eliminar cor vermelha
    if (strstr(missao, "cor vermelha")) {
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, "vermelho") == 0)
                return 0;
        return 1;
    }
    // Missão 3: dominar 4 territórios
    if (strstr(missao, "4 territorios")) {
        int count = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, cor) == 0)
                count++;
        return count >= 4;
    }
    // Missão 4: maior exército total
    if (strstr(missao, "maior exercito")) {
        int meu_total = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, cor) == 0)
                meu_total += mapa[i].tropas;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, cor) != 0) {
                int outro = 0;
                char cor_cmp[10];
                strcpy(cor_cmp, mapa[i].cor);
                for (int j = 0; j < tamanho; j++)
                    if (strcmp(mapa[j].cor, cor_cmp) == 0)
                        outro += mapa[j].tropas;
                if (outro >= meu_total) return 0;
            }
        }
        return 1;
    }
    // Missão 5: verificada externamente no turno (flag passada via parâmetro)
    return 0;
}


// ─── Funções do mapa ──────────────────────────────────────────────────────────

void cadastrar(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: ");   scanf("%29s", mapa[i].nome);
        printf("Cor: ");    scanf("%9s",  mapa[i].cor);
        printf("Tropas: "); scanf("%d",  &mapa[i].tropas);
    }
}

void exibirMapa(Territorio* mapa, int n) {
    printf("\n========== MAPA ==========\n");
    for (int i = 0; i < n; i++)
        printf("[%d] %-20s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
}

// ─── Função de ataque ─────────────────────────────────────────────────────────

// conquistasNaTurno: ponteiro por referência para contar conquistas no turno
void atacar(Territorio* atacante, Territorio* defensor, int* conquistasNaTurno) {
    int dado_atk = (rand() % 6) + 1;
    int dado_def = (rand() % 6) + 1;

    printf("\n⚔  %s ataca %s! (dado atk: %d | dado def: %d)\n",
           atacante->nome, defensor->nome, dado_atk, dado_def);

    if (dado_atk > dado_def) {
        printf("   ✅ %s CONQUISTOU %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        (*conquistasNaTurno)++;
    } else {
        printf("   🛡  %s resistiu. %s perde 1 tropa.\n",
               defensor->nome, atacante->nome);
        if (atacante->tropas > 0) atacante->tropas--;
    }
}

// ─── Liberação de memória ─────────────────────────────────────────────────────

void liberarMemoria(Territorio* mapa, char* missao1, char* missao2) {
    free(mapa);
    free(missao1);
    free(missao2);
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    srand(time(NULL));

    // Cadastro de jogadores
    char cor1[10], cor2[10];
    printf("Cor do Jogador 1: "); scanf("%9s", cor1);
    printf("Cor do Jogador 2: "); scanf("%9s", cor2);

    // Missões alocadas dinamicamente
    char* missao1 = malloc(200 * sizeof(char));
    char* missao2 = malloc(200 * sizeof(char));
    if (!missao1 || !missao2) { printf("Erro de alocação.\n"); return 1; }

    atribuirMissao(missao1, missoes, TOTAL_MISSOES);
    atribuirMissao(missao2, missoes, TOTAL_MISSOES);

    exibirMissao(missao1, cor1);
    exibirMissao(missao2, cor2);

    // Cadastro de territórios
    int n;
    printf("\nQuantos territórios? "); scanf("%d", &n);
    Territorio* mapa = calloc(n, sizeof(Territorio));
    if (!mapa) { printf("Erro de alocação.\n"); return 1; }

    cadastrar(mapa, n);
    exibirMapa(mapa, n);

    // Loop de turnos
    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        int atk, def;
        int conquistasNaTurno = 0; // zerado a cada turno

        printf("\nÍndice ATACANTE: "); scanf("%d", &atk);
        printf("Índice DEFENSOR: "); scanf("%d", &def);

        if (atk < 0 || atk >= n || def < 0 || def >= n || atk == def) {
            printf("Índices inválidos.\n"); continue;
        }
        if (strcmp(mapa[atk].cor, mapa[def].cor) == 0) {
            printf("Não pode atacar território aliado!\n"); continue;
        }
        if (mapa[atk].tropas <= 0) {
            printf("Sem tropas para atacar.\n"); continue;
        }

        atacar(&mapa[atk], &mapa[def], &conquistasNaTurno);
        exibirMapa(mapa, n);

        // Missão 5 (2 conquistas no turno) verificada aqui
        char* jogador_cor = mapa[atk].cor;
        int venceu = 0;

        if (strstr(missao1, "2 territorios inimigos em um unico turno")
            && strcmp(cor1, jogador_cor) == 0 && conquistasNaTurno >= 2) {
            venceu = 1; printf("\n🏆 Jogador [%s] cumpriu a missão e VENCEU!\n", cor1);
        } else if (verificarMissao(missao1, mapa, n, cor1)) {
            venceu = 1; printf("\n🏆 Jogador [%s] cumpriu a missão e VENCEU!\n", cor1);
        }

        if (!venceu) {
            if (strstr(missao2, "2 territorios inimigos em um unico turno")
                && strcmp(cor2, jogador_cor) == 0 && conquistasNaTurno >= 2) {
                venceu = 1; printf("\n🏆 Jogador [%s] cumpriu a missão e VENCEU!\n", cor2);
            } else if (verificarMissao(missao2, mapa, n, cor2)) {
                venceu = 1; printf("\n🏆 Jogador [%s] cumpriu a missão e VENCEU!\n", cor2);
            }
        }

        if (venceu) break;

        printf("\nContinuar? (s/n): "); scanf(" %c", &continuar);
    }

    liberarMemoria(mapa, missao1, missao2);
    printf("\nMemória liberada. Fim de jogo!\n");
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
