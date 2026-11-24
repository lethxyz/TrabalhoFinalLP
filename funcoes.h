#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

/* ---------- constantes ---------- */

#define tam 7
#define TAM_EQUIPE 3

#define ARQUIVO_RANKING "ranking.txt"
#define ARQUIVO_BATALHAS "batalhas.txt"
#define ARQUIVO_SAVE "savegame.bin"

#define MAX_JOGADORES 100
#define TAM_NICKNAME 20
#define PONTOS_VITORIA 5
#define PONTOS_DERROTA 3

/* retorno de batalhar() e continuar_batalha() */
#define PARTIDA_ERRO -1
#define PARTIDA_DERROTA 0
#define PARTIDA_VITORIA 1
#define PARTIDA_SALVA 2

/* indice devolvido por escolherHabilidade() para "salvar e voltar" */
#define ACAO_SALVAR 2

/*Tempo entre os printf para melhor visibilidade*/
#ifdef _WIN32
    #include <windows.h>
    #define PAUSA(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define PAUSA(ms) usleep((ms) * 1000)
#endif

/* ---------- structs ---------- */

typedef struct {
    char nomeAtaque[30];
    char tipo[15];
    int danoBase;
    int nivelNecessario;
} Habilidade;

typedef struct {
    char nome[30];
    char tipo[20];
    int hp;
    int maxHp;
    Habilidade habilidades[2];
} Monstro;

typedef struct {
    char nickname[TAM_NICKNAME];
    int pontos;
} Jogador;

/* estado completo de uma partida: e isso que vai para o savegame.bin */
typedef struct {
    char nickname[TAM_NICKNAME];
    Monstro monstros_jogador[tam];
    Monstro monstros_cpu[tam];
    int equipe_jogador[TAM_EQUIPE];
    int equipe_cpu[TAM_EQUIPE];
    int ativo_jogador;
    int ativo_cpu;
} EstadoPartida;

/* ---------- registro (log) e entrada ---------- */

void registrarPrintf(const char *formato, ...);

/* todo printf do jogo passa por registrarPrintf (tela + batalhas.txt).
   Esta macro precisa ficar DEPOIS dos #include acima. */
#define printf(...) registrarPrintf(__VA_ARGS__)

int lerInteiro(const char *mensagem, int minimo, int maximo);

void iniciarRegistroBatalha(void);
void finalizarRegistroBatalha(void);

/* ---------- menus e telas ---------- */

void exibirMenu(void);
void exibirTutorial(void);
void exibirMonstros(Monstro monstros[]);
void exibirStatus(Monstro *jogador, Monstro *inimigo);
void exibirEquipe(Monstro monstros[], int equipe[], const char *nome);

/* ---------- escolhas ---------- */

int escolherHabilidade(Monstro *monstro);
void escolherEquipe(Monstro monstros[], int equipe[], const char *jogador);
void escolherEquipeAdversario(int equipe[]);
int escolherHabilidadeAdversario(void);
int escolherProximoAdversario(Monstro monstros[], int equipe[]);
int escolherProximo(Monstro monstros[], int equipe[]);
int contarVivos(Monstro monstros[], int equipe[]);

/* ---------- nickname e ranking ---------- */

int nickname_valido(const char *nickname);
int nicknames_iguais(const char *a, const char *b);
void ler_nickname(char nickname[]);

int carregar_ranking(Jogador jogadores[]);
void salvar_ranking(Jogador jogadores[], int total);
void ordenar_ranking(Jogador jogadores[], int total);
void criar_ranking_inicial(void);
void exibir_ranking(void);
void atualizar_pontuacao(const char *nickname, int venceu);
void aplicar_resultado(const char *nickname, int resultado);

/* ---------- salvamento da partida ---------- */

int salvar_partida(const EstadoPartida *estado);
int carregar_partida(EstadoPartida *estado);

/* ---------- batalha ---------- */

int batalhar(Monstro monstros[], const char *nickname);
int continuar_batalha(char nickname[]);

#endif
