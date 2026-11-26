# Elemental Clash

Jogo de batalha por turnos em C para o terminal, feito para o Trabalho Final de
Laboratório de Programação em C (UFC Russas, 2026.2), proposta P3 (batalha de criaturas).

O jogador escolhe 3 entre 7 monstros elementais, enfrenta uma equipe sorteada pelo
computador e pontua em um ranking (+5 por vitória, -3 por derrota). A partida pode
ser salva no meio da batalha e retomada depois.

## Requisitos

- `gcc` (Linux, WSL no Windows ou MinGW)
- `make` (opcional)

Não há dependências externas: só a biblioteca padrão de C.
A única exceção é a pausa entre as mensagens (macro `PAUSA`, em `funcoes.h`), que usa
`<windows.h>` no Windows e `<unistd.h>` nos demais sistemas. O compilador escolhe a
biblioteca certa sozinho, sem instalação extra (veja "Pausa entre as mensagens").

## Compilar

Com `make`:

```
make
```

Ou direto com o `gcc`:

```
gcc main.c funcoes.c -o elemental_clash -Wall -Wextra
```

No Windows com MinGW, use `-o elemental_clash.exe` e, se precisar do make,
`mingw32-make`.

## Executar

```
./elemental_clash        # Linux / WSL
.\elemental_clash.exe    # Windows
```

Ou `make run`. O jogo deve ser executado a partir da pasta do projeto, porque
`ranking.txt`, `batalhas.txt` e `savegame.bin` são lidos e gravados na pasta atual.

## Como jogar

Menu principal:

| Opção | O que faz |
|---|---|
| 1 | Tutorial com as regras |
| 2 | Nova batalha: pede o nickname (somente letras) e a escolha de 3 monstros |
| 3 | Continuar partida salva (`savegame.bin`) |
| 4 | Verificar ranking (somente leitura) |
| 5 | Sair |

Durante a batalha, a cada turno escolha a habilidade 1 ou 2, ou a opção 3 para
salvar a partida e voltar ao menu. A batalha termina quando os 3 monstros de um
dos lados forem derrotados.

## Pausa entre as mensagens

Cada mensagem do jogo espera 500 ms antes da próxima, para facilitar a leitura da
batalha. Este trecho foi adicionado depois da produção do relatório e do
`Testes_e_Contribuicoes.pdf`, por Lethycia Renovato de Medeiros.

| Item | Detalhe |
|---|---|
| Macro | `PAUSA(ms)`, definida em `funcoes.h` |
| Windows (`_WIN32`) | `#include <windows.h>` e `Sleep(ms)` |
| Linux / WSL / outros | `#include <unistd.h>` e `usleep((ms) * 1000)` |
| Onde é aplicada | `registrarPrintf` (`funcoes.c`), depois do `vprintf`: `fflush(stdout)` e `PAUSA(500)` |

Como `funcoes.h` redefine `printf` para `registrarPrintf`, todo `printf` do jogo (menu,
tutorial, escolhas, turnos e ranking) passa pela pausa. A escolha entre `windows.h` e
`unistd.h` é feita na compilação e não muda o comando usado para compilar. Para ajustar o
tempo, altere o valor passado a `PAUSA` em `registrarPrintf`.

## Arquivos do projeto

| Arquivo | Conteúdo |
|---|---|
| `main.c` | dados dos monstros, menu e laço principal |
| `funcoes.c` | lógica do jogo: batalha, ranking, salvamento, validação de entradas; pausa de 500 ms entre as mensagens (`registrarPrintf`) |
| `funcoes.h` | constantes, structs (`Habilidade`, `Monstro`, `Jogador`, `EstadoPartida`) e protótipos; macro `PAUSA` (`windows.h` / `unistd.h`) |
| `Makefile` | compilação com `-Wall -Wextra` |
| `Testes_e_Contribuicoes.pdf` | casos de teste (T1 a T8), registro de resultados e histórico de contribuições |
| `Relatorio_Elemental_Clash.pdf` | relatório do trabalho |

## Arquivos gerados ao jogar

| Arquivo | Formato | Uso |
|---|---|---|
| `ranking.txt` | texto, uma linha `nickname pontos` | ranking; criado com jogadores pré-inseridos se não existir |
| `batalhas.txt` | texto | registro (log) de cada batalha, com data |
| `savegame.bin` | binário (struct `EstadoPartida`) | partida salva; apagado quando a partida retomada termina |

Observações:

- O save tem um único espaço: salvar de novo sobrescreve o anterior.
- O `savegame.bin` é binário e depende da máquina; não leve um save de um
  computador para outro.
- Um save corrompido ou editado é rejeitado com a mensagem
  "Nenhuma partida salva valida foi encontrada".

## Testes

Veja `Testes_e_Contribuicoes.pdf`, que reúne os casos de teste e o histórico de
contribuições de cada integrante.

## Equipe

- Ana Clara Rodrigues Leite - 599022
- Kaio Daniel Lima Costa - 581966
- Lethycia Renovato de Medeiros - 601382
- Sofia de Sousa - 599545

Contribuição adicionada depois do material escrito: pausa entre as mensagens
(`PAUSA`, com `windows.h` e `unistd.h`, e `fflush`/`PAUSA(500)` em `registrarPrintf`),
por Lethycia Renovato de Medeiros - 601382.
