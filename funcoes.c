#include "funcoes.h"

/* arquivo de registro da batalha (so este .c usa) */
static FILE *arquivoBatalha = NULL;

/* =====================================================
   REGISTRO (LOG) E ENTRADA
   ===================================================== */

void registrarPrintf(const char *formato, ...) {
    va_list argumentos;
    va_list copia;

    va_start(argumentos, formato);
    va_copy(copia, argumentos);

    vprintf(formato, argumentos);
    fflush(stdout); // O texto vai aparecer na tela mesmo sem \n
    PAUSA(250);     // Pausa por 500ms

    if (arquivoBatalha != NULL) {
        vfprintf(arquivoBatalha, formato, copia);
        fflush(arquivoBatalha);
    }

    va_end(copia);
    va_end(argumentos);
}

int lerInteiro(const char *mensagem, int minimo, int maximo) {
    int valor;
    int resultado;
    int caractere;

    while (1) {
        printf("%s", mensagem);
        resultado = scanf("%d", &valor);

        /* fim da entrada (Ctrl+D / Ctrl+Z): sai em vez de repetir para sempre */
        if (resultado == EOF) {
            printf("\nEntrada encerrada. Saindo do jogo.\n");
            exit(EXIT_SUCCESS);
        }

        if (resultado == 1) {
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }

            if (valor >= minimo && valor <= maximo) {
                return valor;
            }
        } else {
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }
        }

        printf("Entrada invalida! Digite um numero entre %d e %d.\n",
               minimo, maximo);
    }
}

void iniciarRegistroBatalha(void) {
    time_t agora;
    char *data;

    arquivoBatalha = fopen(ARQUIVO_BATALHAS, "a");

    if (arquivoBatalha == NULL) {
        printf("Aviso: nao foi possivel abrir %s.\n", ARQUIVO_BATALHAS);
        return;
    }

    agora = time(NULL);
    data = ctime(&agora);

    fprintf(arquivoBatalha,
        "\n\n========================================\n");
    fprintf(arquivoBatalha, "REGISTRO DE BATALHA\n");

    if (data != NULL) {
        fprintf(arquivoBatalha, "Data: %s", data);
    }

    fprintf(arquivoBatalha,
            "========================================\n");
    fflush(arquivoBatalha);
}

void finalizarRegistroBatalha(void) {
    if (arquivoBatalha != NULL) {
        fprintf(arquivoBatalha,
                "\n========== FIM DO REGISTRO ==========\n");

        fclose(arquivoBatalha);
        arquivoBatalha = NULL;

        printf("\nBatalha salva em %s!\n", ARQUIVO_BATALHAS);
    }
}

/* =====================================================
   MENUS E TELAS
   ===================================================== */

void exibirMenu(void) {
    printf("\n--MENU PRINCIPAL--\n"
           "Selecione uma das opcoes abaixo:\n"
           "1 - Tutorial\n"
           "2 - Nova batalha\n"
           "3 - Continuar partida salva\n"
           "4 - Verificar ranking\n"
           "5 - Sair\n");
}

void exibirTutorial(void) {
    printf("\n|-----------------------------------TUTORIAL-----------------------------------|\n"
             "|    Elemental Clash: escolha tres monstros e enfrente a equipe adversaria!    |\n"
             "|                                                                              |\n"
             "|          Cada monstro possui duas habilidades com danos diferentes.          |\n"
             "|          Vence quem derrotar os tres monstros da equipe adversaria.          |\n"
             "| Durante a batalha, escolha a opcao 3 para salvar a partida e voltar ao menu. |\n"
             "|      Use 'Continuar partida salva' no menu para retomar de onde parou.       |\n"
             "|          Cada vitoria vale +%d pontos e cada derrota -%d no ranking.           |\n",
            PONTOS_VITORIA, PONTOS_DERROTA);
}

void exibirMonstros(Monstro monstros[]) {
    for (int i = 0; i < tam; i++) {
        printf("\n%d - %s\n", i + 1, monstros[i].nome);
        printf("Tipo: %s\n", monstros[i].tipo);
        printf("HP: %d/%d\n", monstros[i].hp, monstros[i].maxHp);
        printf("Habilidades:\n");

        for (int j = 0; j < 2; j++) {
            printf("%d - %s | Dano: %d\n", j + 1,
                   monstros[i].habilidades[j].nomeAtaque,
                   monstros[i].habilidades[j].danoBase);
        }

        printf("----------------------\n");
    }
}

void exibirStatus(Monstro *jogador, Monstro *inimigo) {
    printf("\n================= STATUS =================\n");
    printf("%s: %d/%d HP VS %s: %d/%d HP\n",
           jogador->nome, jogador->hp, jogador->maxHp,
           inimigo->nome, inimigo->hp, inimigo->maxHp);
    printf("==========================================\n");
}

void exibirEquipe(Monstro monstros[], int equipe[], const char *nome) {
    printf("\n===== EQUIPE DO %s =====\n", nome);

    for (int i = 0; i < TAM_EQUIPE; i++) {
        int indice = equipe[i];

        printf("\n%d - %s\n", i + 1, monstros[indice].nome);
        printf("Tipo: %s\n", monstros[indice].tipo);
        printf("HP: %d/%d\n",
               monstros[indice].hp, monstros[indice].maxHp);
        printf("----------------------\n");
    }
}

/* =====================================================
   ESCOLHAS
   ===================================================== */

/* devolve 0 ou 1 (habilidade) ou ACAO_SALVAR */
int escolherHabilidade(Monstro *monstro) {
    int escolha;

    printf("\nEscolha uma habilidade de %s:\n", monstro->nome);

    for (int i = 0; i < 2; i++) {
        printf("%d - %s | Dano: %d\n", i + 1,
               monstro->habilidades[i].nomeAtaque,
               monstro->habilidades[i].danoBase);
    }

    printf("3 - Salvar partida e voltar ao menu\n");

    escolha = lerInteiro("Digite sua escolha: ", 1, 3);

    if (escolha == 3) {
        return ACAO_SALVAR;
    }

    printf("Habilidade escolhida: %s\n",
           monstro->habilidades[escolha - 1].nomeAtaque);

    return escolha - 1;
}

void escolherEquipe(Monstro monstros[], int equipe[],
                    const char *jogador) {
    int escolha;
    int repetido;

    printf("\n===== %s: ESCOLHA 3 MONSTROS =====\n", jogador);
    exibirMonstros(monstros);

    for (int i = 0; i < TAM_EQUIPE; i++) {
        do {
            repetido = 0;
            escolha = lerInteiro("Escolha um monstro (1 a 7): ", 1, tam);

            for (int j = 0; j < i; j++) {
                if (equipe[j] == escolha - 1) {
                    printf("Voce ja escolheu esse monstro!\n");
                    repetido = 1;
                }
            }
        } while (repetido);

        equipe[i] = escolha - 1;

        printf("Escolha registrada: %s\n", monstros[equipe[i]].nome);
        printf("%s foi adicionado a equipe.\n",
               monstros[equipe[i]].nome);
    }
}

void escolherEquipeAdversario(int equipe[]) {
    int escolha;
    int repetido;

    for (int i = 0; i < TAM_EQUIPE; i++) {
        do {
            repetido = 0;
            escolha = rand() % tam;

            for (int j = 0; j < i; j++) {
                if (equipe[j] == escolha) {
                    repetido = 1;
                }
            }
        } while (repetido);

        equipe[i] = escolha;
    }
}

int escolherHabilidadeAdversario(void) {
    return rand() % 2;
}

int escolherProximoAdversario(Monstro monstros[], int equipe[]) {
    for (int i = 0; i < TAM_EQUIPE; i++) {
        if (monstros[equipe[i]].hp > 0) {
            return equipe[i];
        }
    }

    return -1;
}

int contarVivos(Monstro monstros[], int equipe[]) {
    int vivos = 0;

    for (int i = 0; i < TAM_EQUIPE; i++) {
        if (monstros[equipe[i]].hp > 0) {
            vivos++;
        }
    }

    return vivos;
}

int escolherProximo(Monstro monstros[], int equipe[]) {
    int escolha;
    int indice;

    printf("\n===== ESCOLHA SEU PROXIMO MONSTRO =====\n");

    for (int i = 0; i < TAM_EQUIPE; i++) {
        indice = equipe[i];

        if (monstros[indice].hp > 0) {
            printf("%d - %s | HP: %d/%d\n", i + 1,
                   monstros[indice].nome,
                   monstros[indice].hp,
                   monstros[indice].maxHp);
        }
    }

    while (1) {
        escolha = lerInteiro("Escolha: ", 1, TAM_EQUIPE);
        indice = equipe[escolha - 1];

        if (monstros[indice].hp > 0) {
            printf("Proximo monstro escolhido: %s\n",
                   monstros[indice].nome);
            return indice;
        }

        printf("Esse monstro ja foi derrotado!\n");
    }
}

/* =====================================================
   NICKNAME E RANKING
   ===================================================== */

int nickname_valido(const char *nickname) {
    int tamanho = strlen(nickname);

    if (tamanho == 0 || tamanho >= TAM_NICKNAME) {
        return 0;
    }

    for (int i = 0; i < tamanho; i++) {
        if (!isalpha((unsigned char) nickname[i])) {
            return 0;
        }
    }

    return 1;
}

/* compara ignorando maiusculas/minusculas (Ana == ana) */
int nicknames_iguais(const char *a, const char *b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char) a[i]) != tolower((unsigned char) b[i])) {
            return 0;
        }
        i++;
    }

    return a[i] == b[i];
}

void ler_nickname(char nickname[]) {
    char entrada[100];
    int tamanho;
    int caractere;

    while (1) {
        printf("\nDigite seu nickname (somente letras, ate %d): ",
               TAM_NICKNAME - 1);

        /* fim da entrada (Ctrl+D / Ctrl+Z): sai em vez de repetir para sempre */
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do jogo.\n");
            exit(EXIT_SUCCESS);
        }

        tamanho = strlen(entrada);

        if (tamanho > 0 && entrada[tamanho - 1] == '\n') {
            entrada[tamanho - 1] = '\0';
        } else {
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }
        }

        if (nickname_valido(entrada)) {
            strcpy(nickname, entrada);
            return;
        }

        printf("Nickname invalido! Use apenas letras (sem espacos ou numeros).\n");
    }
}

int carregar_ranking(Jogador jogadores[]) {
    FILE *arquivo = fopen(ARQUIVO_RANKING, "r");
    char linha[100];
    int total = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (total < MAX_JOGADORES &&
           fgets(linha, sizeof(linha), arquivo) != NULL) {
        /* %19s = TAM_NICKNAME - 1 */
        if (sscanf(linha, "%19s %d",
                   jogadores[total].nickname,
                   &jogadores[total].pontos) == 2) {
            total++;
        }
    }

    fclose(arquivo);
    return total;
}

void salvar_ranking(Jogador jogadores[], int total) {
    FILE *arquivo = fopen(ARQUIVO_RANKING, "w");

    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel salvar o ranking.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%s %d\n",
                jogadores[i].nickname, jogadores[i].pontos);
    }

    fclose(arquivo);
}

/* ordem decrescente de pontos (bubble sort) */
void ordenar_ranking(Jogador jogadores[], int total) {
    Jogador auxiliar;

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (jogadores[j].pontos < jogadores[j + 1].pontos) {
                auxiliar = jogadores[j];
                jogadores[j] = jogadores[j + 1];
                jogadores[j + 1] = auxiliar;
            }
        }
    }
}

/* cria o ranking.txt com jogadores pre-inseridos se ele ainda nao existir */
void criar_ranking_inicial(void) {
    FILE *arquivo = fopen(ARQUIVO_RANKING, "r");

    if (arquivo != NULL) {
        fclose(arquivo);
        return;
    }

    arquivo = fopen(ARQUIVO_RANKING, "w");

    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "Ana 25\n");
    fprintf(arquivo, "Carlos 20\n");
    fprintf(arquivo, "Beatriz 15\n");
    fprintf(arquivo, "Diego 10\n");
    fprintf(arquivo, "Elisa 5\n");

    fclose(arquivo);
}

void exibir_ranking(void) {
    Jogador jogadores[MAX_JOGADORES];
    int total = carregar_ranking(jogadores);

    ordenar_ranking(jogadores, total);

    printf("\n========== RANKING ==========\n");

    if (total == 0) {
        printf("Nenhum jogador no ranking ainda.\n");
    }

    for (int i = 0; i < total; i++) {
        printf("%2d. %-19s %4d pts\n",
               i + 1, jogadores[i].nickname, jogadores[i].pontos);
    }

    printf("=============================\n");
}

void atualizar_pontuacao(const char *nickname, int venceu) {
    Jogador jogadores[MAX_JOGADORES];
    int total = carregar_ranking(jogadores);
    int indice = -1;
    int posicao = 0;
    int variacao = venceu ? PONTOS_VITORIA : -PONTOS_DERROTA;

    for (int i = 0; i < total; i++) {
        if (nicknames_iguais(jogadores[i].nickname, nickname)) {
            indice = i;
            break;
        }
    }

    /* jogador novo: entra no ranking com 0 pontos */
    if (indice == -1) {
        if (total >= MAX_JOGADORES) {
            printf("\nRanking cheio! Pontuacao nao registrada.\n");
            return;
        }

        indice = total;
        strcpy(jogadores[indice].nickname, nickname);
        jogadores[indice].pontos = 0;
        total++;
    }

    jogadores[indice].pontos += variacao;

    ordenar_ranking(jogadores, total);
    salvar_ranking(jogadores, total);

    /* a posicao muda depois da ordenacao, entao busca de novo */
    for (int i = 0; i < total; i++) {
        if (nicknames_iguais(jogadores[i].nickname, nickname)) {
            posicao = i + 1;
            break;
        }
    }

    printf("\n%s: %+d pontos! Total: %d pts | Posicao no ranking: %d\n",
           nickname, variacao, jogadores[posicao - 1].pontos, posicao);
}

/* so pontua quando a batalha terminou (vitoria ou derrota) */
void aplicar_resultado(const char *nickname, int resultado) {
    if (resultado == PARTIDA_VITORIA || resultado == PARTIDA_DERROTA) {
        atualizar_pontuacao(nickname, resultado == PARTIDA_VITORIA);
    }
}

/* =====================================================
   SALVAMENTO DA PARTIDA (savegame.bin)
   ===================================================== */

/* true se o texto tem um '\0' dentro do limite (evita ler fora do vetor) */
static int texto_terminado(const char *texto, size_t limite) {
    return memchr(texto, '\0', limite) != NULL;
}

static int monstro_valido(const Monstro *monstro) {
    if (!texto_terminado(monstro->nome, sizeof(monstro->nome)) ||
        !texto_terminado(monstro->tipo, sizeof(monstro->tipo))) {
        return 0;
    }

    if (monstro->maxHp <= 0 || monstro->hp < 0 ||
        monstro->hp > monstro->maxHp) {
        return 0;
    }

    for (int i = 0; i < 2; i++) {
        if (!texto_terminado(monstro->habilidades[i].nomeAtaque,
                             sizeof(monstro->habilidades[i].nomeAtaque)) ||
            monstro->habilidades[i].danoBase < 0) {
            return 0;
        }
    }

    return 1;
}

static int equipe_valida(const int equipe[]) {
    for (int i = 0; i < TAM_EQUIPE; i++) {
        if (equipe[i] < 0 || equipe[i] >= tam) {
            return 0;
        }
    }

    return 1;
}

/* um arquivo editado ou corrompido nao pode causar acesso fora dos vetores */
static int estado_valido(const EstadoPartida *estado) {
    if (!texto_terminado(estado->nickname, sizeof(estado->nickname)) ||
        !nickname_valido(estado->nickname)) {
        return 0;
    }

    for (int i = 0; i < tam; i++) {
        if (!monstro_valido(&estado->monstros_jogador[i]) ||
            !monstro_valido(&estado->monstros_cpu[i])) {
            return 0;
        }
    }

    if (!equipe_valida(estado->equipe_jogador) ||
        !equipe_valida(estado->equipe_cpu)) {
        return 0;
    }

    if (estado->ativo_jogador < 0 || estado->ativo_jogador >= tam ||
        estado->ativo_cpu < 0 || estado->ativo_cpu >= tam) {
        return 0;
    }

    if (estado->monstros_jogador[estado->ativo_jogador].hp <= 0 ||
        estado->monstros_cpu[estado->ativo_cpu].hp <= 0) {
        return 0;
    }

    return 1;
}

/* grava a struct inteira de uma vez; devolve 1 se deu certo */
int salvar_partida(const EstadoPartida *estado) {
    FILE *arquivo = fopen(ARQUIVO_SAVE, "wb");
    size_t gravados;

    if (arquivo == NULL) {
        return 0;
    }

    gravados = fwrite(estado, sizeof(EstadoPartida), 1, arquivo);

    if (fclose(arquivo) != 0) {
        return 0;
    }

    return gravados == 1;
}

/* le a struct inteira e valida; devolve 1 se o save existe e e valido */
int carregar_partida(EstadoPartida *estado) {
    FILE *arquivo = fopen(ARQUIVO_SAVE, "rb");
    size_t lidos;

    if (arquivo == NULL) {
        return 0;
    }

    lidos = fread(estado, sizeof(EstadoPartida), 1, arquivo);
    fclose(arquivo);

    if (lidos != 1) {
        return 0;
    }

    return estado_valido(estado);
}

/* =====================================================
   BATALHA
   ===================================================== */

/* monta uma partida nova: copias independentes para jogador e CPU */
static void preparar_partida(EstadoPartida *estado, Monstro catalogo[],
                             const char *nickname) {
    strncpy(estado->nickname, nickname, TAM_NICKNAME - 1);
    estado->nickname[TAM_NICKNAME - 1] = '\0';

    memcpy(estado->monstros_jogador, catalogo, tam * sizeof(Monstro));
    memcpy(estado->monstros_cpu, catalogo, tam * sizeof(Monstro));

    for (int i = 0; i < tam; i++) {
        estado->monstros_jogador[i].hp = estado->monstros_jogador[i].maxHp;
        estado->monstros_cpu[i].hp = estado->monstros_cpu[i].maxHp;
    }

    escolherEquipe(estado->monstros_jogador, estado->equipe_jogador,
                   "JOGADOR");
    escolherEquipeAdversario(estado->equipe_cpu);

    exibirEquipe(estado->monstros_jogador, estado->equipe_jogador,
                 "JOGADOR");
    exibirEquipe(estado->monstros_cpu, estado->equipe_cpu, "CPU");

    estado->ativo_jogador = estado->equipe_jogador[0];
    estado->ativo_cpu = estado->equipe_cpu[0];
}

/* roda o loop da batalha sobre o estado recebido.
   devolve PARTIDA_VITORIA, PARTIDA_DERROTA ou PARTIDA_SALVA */
static int jogar_partida(EstadoPartida *estado) {
    Monstro *monstros_jogador = estado->monstros_jogador;
    Monstro *monstros_cpu = estado->monstros_cpu;
    int *equipe_jogador = estado->equipe_jogador;
    int *equipe_cpu = estado->equipe_cpu;

    int ativo1 = estado->ativo_jogador;
    int ativo2 = estado->ativo_cpu;
    int ataque1;
    int ataque2;
    int dano;
    int salvou = 0;

    while (contarVivos(monstros_jogador, equipe_jogador) > 0 &&
           contarVivos(monstros_cpu, equipe_cpu) > 0) {

        if (monstros_jogador[ativo1].hp <= 0) {
            printf("\n%s foi derrotado!\n", monstros_jogador[ativo1].nome);

            if (contarVivos(monstros_jogador, equipe_jogador) == 0) {
                break;
            }

            ativo1 = escolherProximo(monstros_jogador, equipe_jogador);
            printf("\nVoce enviou %s!\n", monstros_jogador[ativo1].nome);
        }

        if (monstros_cpu[ativo2].hp <= 0) {
            printf("\n%s foi derrotado!\n", monstros_cpu[ativo2].nome);

            if (contarVivos(monstros_cpu, equipe_cpu) == 0) {
                break;
            }

            ativo2 = escolherProximoAdversario(monstros_cpu, equipe_cpu);
            printf("\nO adversario enviou %s!\n",
                   monstros_cpu[ativo2].nome);
        }

        exibirStatus(&monstros_jogador[ativo1], &monstros_cpu[ativo2]);

        printf("\n===== SEU TURNO =====\n");

        ataque1 = escolherHabilidade(&monstros_jogador[ativo1]);

        if (ataque1 == ACAO_SALVAR) {
            estado->ativo_jogador = ativo1;
            estado->ativo_cpu = ativo2;

            if (salvar_partida(estado)) {
                printf("\nPartida salva em %s! Use 'Continuar partida salva' no menu.\n",
                       ARQUIVO_SAVE);
                salvou = 1;
                break;
            }

            printf("\nErro ao salvar a partida. Continue jogando.\n");
            continue;
        }

        dano = monstros_jogador[ativo1].habilidades[ataque1].danoBase;

        printf("\n%s usou %s!\n",
               monstros_jogador[ativo1].nome,
               monstros_jogador[ativo1].habilidades[ataque1].nomeAtaque);

        monstros_cpu[ativo2].hp -= dano;

        if (monstros_cpu[ativo2].hp < 0) {
            monstros_cpu[ativo2].hp = 0;
        }

        printf("%s recebeu %d de dano!\n",
               monstros_cpu[ativo2].nome, dano);
        printf("HP restante de %s: %d/%d\n",
               monstros_cpu[ativo2].nome,
               monstros_cpu[ativo2].hp,
               monstros_cpu[ativo2].maxHp);

        if (contarVivos(monstros_cpu, equipe_cpu) == 0) {
            break;
        }

        if (monstros_cpu[ativo2].hp <= 0) {
            printf("\n%s foi derrotado!\n", monstros_cpu[ativo2].nome);

            ativo2 = escolherProximoAdversario(monstros_cpu, equipe_cpu);

            printf("\nO adversario enviou %s!\n",
                   monstros_cpu[ativo2].nome);
        }

        printf("\n===== TURNO DO ADVERSARIO =====\n");

        exibirStatus(&monstros_jogador[ativo1], &monstros_cpu[ativo2]);

        ataque2 = escolherHabilidadeAdversario();
        dano = monstros_cpu[ativo2].habilidades[ataque2].danoBase;

        printf("\n%s do adversario usou %s!\n",
               monstros_cpu[ativo2].nome,
               monstros_cpu[ativo2].habilidades[ataque2].nomeAtaque);

        monstros_jogador[ativo1].hp -= dano;

        if (monstros_jogador[ativo1].hp < 0) {
            monstros_jogador[ativo1].hp = 0;
        }

        printf("%s recebeu %d de dano!\n",
               monstros_jogador[ativo1].nome, dano);
        printf("HP restante de %s: %d/%d\n",
               monstros_jogador[ativo1].nome,
               monstros_jogador[ativo1].hp,
               monstros_jogador[ativo1].maxHp);
    }

    if (salvou) {
        return PARTIDA_SALVA;
    }

    printf("\n========== FIM DA BATALHA ==========\n");

    if (contarVivos(monstros_jogador, equipe_jogador) > 0) {
        printf("\nPARABENS! VOCE VENCEU!\n");
        return PARTIDA_VITORIA;
    }

    printf("\nVOCE FOI DERROTADO! O ADVERSARIO VENCEU!\n");
    return PARTIDA_DERROTA;
}

/* nova batalha: catalogo = monstros originais (nao e alterado) */
int batalhar(Monstro monstros[], const char *nickname) {
    EstadoPartida *estado = malloc(sizeof(EstadoPartida));
    int resultado;

    if (estado == NULL) {
        printf("Erro ao alocar memoria para a partida!\n");
        return PARTIDA_ERRO;
    }

    iniciarRegistroBatalha();

    printf("\n========== NOVA BATALHA ==========\n");
    printf("Jogador: %s\n", nickname);

    preparar_partida(estado, monstros, nickname);

    printf("\n========== INICIO DA BATALHA ==========\n");

    resultado = jogar_partida(estado);

    finalizarRegistroBatalha();
    free(estado);

    return resultado;
}

/* retoma a partida do savegame.bin; copia o nickname salvo para o chamador */
int continuar_batalha(char nickname[]) {
    EstadoPartida *estado = malloc(sizeof(EstadoPartida));
    int resultado;

    if (estado == NULL) {
        printf("Erro ao alocar memoria para a partida!\n");
        return PARTIDA_ERRO;
    }

    if (!carregar_partida(estado)) {
        printf("\nNenhuma partida salva valida foi encontrada.\n");
        free(estado);
        return PARTIDA_ERRO;
    }

    strcpy(nickname, estado->nickname);

    iniciarRegistroBatalha();

    printf("\n========== PARTIDA RETOMADA ==========\n");
    printf("Jogador: %s\n", nickname);

    resultado = jogar_partida(estado);

    finalizarRegistroBatalha();

    /* partida terminada: o save nao pode ser reutilizado */
    if (resultado == PARTIDA_VITORIA || resultado == PARTIDA_DERROTA) {
        remove(ARQUIVO_SAVE);
    }

    free(estado);

    return resultado;
}
