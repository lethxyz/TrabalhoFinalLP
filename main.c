#include "funcoes.h"

int main(void) {
    Monstro monstros[tam] = {
        {"Vulkarn", "Fogo", 100, 100, {
            {"Lava Burst", "Fogo", 30, 1},
            {"Inferno Crush", "Fogo", 45, 1}
        }},
        {"Aquoryn", "Agua", 100, 100, {
            {"Aqua Spear", "Agua", 25, 1},
            {"Tsunami Fang", "Agua", 40, 1}
        }},
        {"Rokke", "Eletricidade", 100, 100, {
            {"Thunderstorm", "Eletricidade", 35, 1},
            {"Rolling Thunder", "Eletricidade", 50, 1}
        }},
        {"Noctyra", "Trevas", 100, 100, {
            {"Shadow Claw", "Trevas", 28, 1},
            {"Abyssal Bite", "Trevas", 42, 1}
        }},
        {"Florantis", "Natureza", 100, 100, {
            {"Thorn Shot", "Natureza", 25, 1},
            {"Carnivorous", "Natureza", 45, 1}
        }},
        {"Cryonox", "Gelo", 100, 100, {
            {"Frost Claw", "Gelo", 32, 1},
            {"Absolute Zero", "Gelo", 48, 1}
        }},
        {"Drakthor", "Terra", 100, 100, {
            {"Boulder Smash", "Terra", 30, 1},
            {"Earthquake", "Terra", 45, 1}
        }}
    };

    int opcao;
    int resultado;
    char nickname[TAM_NICKNAME];

    srand((unsigned int) time(NULL));
    criar_ranking_inicial();

    do {
        exibirMenu();
        opcao = lerInteiro("Opcao: ", 1, 5);

        switch (opcao) {
            case 1:
                exibirTutorial();
                break;

            case 2:
                ler_nickname(nickname);
                resultado = batalhar(monstros, nickname);
                aplicar_resultado(nickname, resultado);
                break;

            case 3:
                resultado = continuar_batalha(nickname);
                aplicar_resultado(nickname, resultado);
                break;

            case 4:
                exibir_ranking();
                break;

            case 5:
                printf("\nAte a proxima!\n");
                break;
        }

    } while (opcao != 5);

    return 0;
}
