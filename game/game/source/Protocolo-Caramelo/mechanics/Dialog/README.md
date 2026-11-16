# Sistema de diálogos

Diálogos no estilo Undertale para o **Protocolo-Caramelo**, feitos sobre a `libgraphical`:

- caixa de texto com **margem → borda → preenchimento** (os `func_alter*` da biblioteca);
- **rosto** do personagem de um dos lados (direita por padrão) com efeito de **gelatina** enquanto ele fala;
- cada letra **sobe de baixo para cima** ao aparecer, com intervalo agradável entre elas;
- **todas** as letras (até as que ainda não apareceram) **ondulam** continuamente;
- **opções no fim da cena** (`DialogScene` + `DialogChoice`), que podem levar a outras cenas;
- **histórico no app Bate-boca**: cenas marcadas com `archive_title` ficam guardadas e podem ser relidas no monitor;
- tudo configurável por **estilo** (`DialogStyle`), sem mexer no motor.

## Sumário

1. [Início rápido](#início-rápido)
2. [Arquivos](#arquivos)
3. [Funções principais](#funções-principais)
4. [DialogLine](#dialogline)
5. [Cenas e opções (DialogScene)](#cenas-e-opções-dialogscene)
6. [Bate-boca e DialogLog](#bate-boca-e-dialoglog)
7. [DialogStyle: personalizando](#dialogstyle-personalizando)
8. [Ganchos: func_alter e alter_char](#ganchos-func_alter-e-alter_char)
9. [Como funciona por dentro](#como-funciona-por-dentro)
10. [Integração com o jogo](#integração-com-o-jogo)
11. [Testes](#testes)
12. [Limitações conhecidas](#limitações-conhecidas)

---

## Início rápido

```c
#include "mechanics/dialogs.h"

void MinhaCena(void)
{
    static const DialogLine cena[] = {            /* static: o sistema guarda o ponteiro */
        DIALOG_LINE(C_FELIPE, CS_HAPPY, "Fala Pedroca, como você tá?"),
        DIALOG_LINE(C_PEDRO,  CS_SAD,   "Eu estou bem... eu acho."),
        DIALOG_LINE_STYLED(C_FELIPE, CS_SUPERANIMATED,
            "Quero te apresentar o doce Caramelo!", &dialog_styleShout),
    };
    Dialog_Play(cena, NULL);                      /* NULL = nada a fazer ao terminar */
}
```

Durante o jogo: **Z**, **Enter**, **Espaço** ou **clique esquerdo** completam a fala atual (se ainda aparece) ou passam para a próxima.

Para testar já: compile com `GAME_DEBUG` e aperte **F2** (`DialogScripts_FelipeCall`) ou **F3** (`DialogScripts_AnaCall`, a cena com opções), no cômodo ou com o monitor aberto. Depois abra o app **Bate-boca** para ver as conversas guardadas.

## Arquivos

| Arquivo | Responsabilidade |
| --- | --- |
| `dialog_fx.{h,c}` | Matemática pura dos efeitos (easing, cronograma das letras, onda, gelatina, quebra de linha). **Sem SDL.** |
| `dialog_style.{h,c}` | `DialogStyle`, estilos prontos e decoradores de exemplo. |
| `dialog.{h,c}` | O "motor": roteiro, entrada, atualização e desenho. |
| `dialog_log.{h,c}` | `DialogLog`: o arquivo das conversas guardadas. **Sem SDL** e sem dependência do resto do jogo. |
| `dialog_scripts.{h,c}` | Suas cenas (uma função por cena). Tem o exemplo completo com opções (Ana). |
| `tests/test_dialog_fx.c` | Testes da matemática (inclui a navegação do menu). |
| `tests/test_dialog_log.c` | Testes do registro de conversas. |
| `screens/play/monitor/app/bateboca.{h,c}` | O app **Bate-boca** (substitui a Lixeira). |

Para criar conteúdo você só mexe em `dialog_scripts.c` (falas) e `dialog_style.c` (estilos).

## Funções principais

```c
void Dialog_Start(const DialogLine *lines, size_t count, void (*on_finish)(void));
#define Dialog_Play(lines, on_finish)   /* o mesmo, com `count` calculado */
```
Começa um diálogo. `lines` **não é copiado** (use `static const`). Se já havia um diálogo ativo, ele é substituído sem chamar o `on_finish` dele. `on_finish` roda ao fim da última fala, com o sistema já livre (pode chamar `Dialog_Play` de novo, por exemplo para encadear cenas).

```c
int  Dialog_HandleInput(SDL_Event *e);
```
Repasse cada evento. Retorna `1` se o diálogo o consumiu (o resto do jogo deve ignorá-lo). Consome as teclas de confirmação e **todos os cliques** (inclusive o "soltar" do clique que fechou o diálogo, para não acionar o botão que estava embaixo). `SDL_QUIT` e `ESC` **nunca** são consumidos.

```c
void Dialog_Render(void);
```
Atualiza e desenha por cima do jogo. Não faz nada sem diálogo ativo e congela o tempo com o jogo pausado.

```c
void Dialog_PlayScene(const DialogScene *scene);   /* cena com opções/histórico: veja abaixo */
void Dialog_Next(void);    /* completa a fala; se já completa, vai para a próxima (ou confirma a opção) */
void Dialog_Abort(void);   /* encerra na hora, sem on_finish */
int  Dialog_IsActive(void);
int  Dialog_IsChoosing(void);   /* 1 enquanto o menu de opções espera o jogador */
void Dialog_SetCharCallback(void (*fn)(const DialogLine *line, size_t index));
```
`Dialog_SetCharCallback` é chamado a cada letra (não-espaço) que começa a aparecer: o ponto certo para tocar o "blip" de voz. `index` é a posição da letra no texto original.

```c
const DialogStyle *Dialog_CurrentStyle(void);
```
Estilo da fala atual (ou `NULL`). Serve para decoradores `alter*`, que recebem só o `Text*`.

## DialogLine

```c
typedef struct DialogLine {
    Character          speaker;   /* C_PEDRO, C_FELIPE, C_ANA, C_JOSE, C_RICARDO... */
    Character_State    state;     /* CS_NORMAL, CS_HAPPY, CS_ANGRY... (a imagem do rosto) */
    const char        *text;      /* UTF-8; '\n' força quebra */
    const DialogStyle *style;     /* NULL = dialog_styleDefault */
    void (*on_show)(void);        /* opcional: roda quando a fala aparece */
} DialogLine;
```

O rosto vem de `chars_tex[speaker][state]` (já carregado em `GameMechanics_Init`). Se a imagem não existir, a fala aparece sem rosto (o espaço dele continua reservado).

## Cenas e opções (DialogScene)

`Dialog_Play` serve para falas soltas. Quando a cena precisa de **opções no fim** ou de **ficar guardada no Bate-boca**, use uma `DialogScene`:

```c
typedef struct DialogChoice {
    const char        *label;       /* texto da opção (curto: cabe em uma linha) */
    void             (*on_select)(void);   /* opcional: roda quando o jogador escolhe */
    const DialogScene *next;        /* opcional: cena que continua depois da escolha */
} DialogChoice;

struct DialogScene {
    const char        *archive_title;      /* != NULL: guarda no Bate-boca com este título */
    const DialogLine  *lines;  size_t count;
    const DialogChoice *choices; size_t choice_count;   /* 0 = sem menu */
    void             (*on_finish)(void);   /* ao terminar a cena (depois da escolha) */
};
#define DIALOG_SCENE_LINES(array)     /* .lines/.count a partir de um vetor */
#define DIALOG_SCENE_CHOICES(array)   /* .choices/.choice_count a partir de um vetor */
```

Exemplo (opção que leva a outra cena, e outra que só dispara uma função):

```c
static const DialogLine despedida_lines[] = {
    DIALOG_LINE(C_ANA, CS_SAD, "...Até mais."),
};
static const DialogScene despedida = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(despedida_lines),
};

static const DialogLine pergunta_lines[] = {
    DIALOG_LINE(C_ANA, CS_NORMAL, "Quer alguma coisa?"),
};
static const DialogChoice opcoes[] = {
    { .label = "Sair",   .next = &despedida },
    { .label = "Comprar", .on_select = AbrirLoja },
};
static const DialogScene pergunta = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(pergunta_lines),
    DIALOG_SCENE_CHOICES(opcoes),
};
/* ... */
Dialog_PlayScene(&pergunta);
```

**Escreva as cenas de baixo para cima** (destinos das opções primeiro): assim nenhuma precisa de declaração antecipada. Cenas e vetores têm de ser `static` (o motor guarda ponteiros). Como `next` é uma `DialogScene`, uma opção pode levar a **outro menu**, formando uma árvore de decisões (veja `ana_buy` em `dialog_scripts.c`).

### Como o menu se comporta

- Ele abre sozinho depois que a **última** fala termina de aparecer (`choice.delay_ms`), ou na hora se o jogador apertar "avançar".
- A caixa de opções aparece **logo acima** da caixa de fala, no canto esquerdo ou direito (`choice.align`). Ela usa a mesma margem, borda, preenchimento e `func_alter*` do estilo da última fala.
- Cada opção **sobe de baixo para cima**, uma depois da outra; só a **selecionada** ondula, muda de cor e ganha uma setinha que quica.
- **Teclado:** cima/baixo (ou W/S) mudam a seleção (circular); Z/Enter/Espaço confirmam. **Mouse:** passar por cima seleciona, clicar confirma.
- Confirmações nos primeiros 250 ms depois da abertura são ignoradas, para o jogador não escolher sem querer enquanto apertava "avançar".
- Ordem ao escolher: grava a opção no Bate-boca → `on_select` da opção → `on_finish` da cena → cena `next` da opção.

## Bate-boca e DialogLog

O app **Bate-boca** (que substituiu a Lixeira no monitor) é um histórico de conversas. Uma cena com `archive_title` grava, na conversa com esse título:

- cada **fala**, no instante em que ela começa a aparecer (quem fala, expressão e texto);
- a **opção escolhida**, como fala do jogador (`DIALOG_PLAYER`, hoje `C_PEDRO`).

Várias cenas com o **mesmo título** se acumulam na mesma conversa (como o histórico de um contato). Você também pode gravar à mão, por exemplo avisos do sistema:

```c
void DialogLog_AddSpeech(const char *title, int speaker, int state, const char *text);
void DialogLog_AddChoice(const char *title, int speaker, const char *label);
void DialogLog_AddNote(const char *title, const char *text);   /* "Chamada encerrada" */

size_t DialogLog_Count(void);
const DialogLogConversation *DialogLog_Get(size_t i);
int    DialogLog_Find(const char *title);       /* índice ou -1 */
void   DialogLog_MarkRead(size_t i);
size_t DialogLog_TotalUnread(void);
void   DialogLog_Reset(void);                    /* apaga tudo */
```

Os textos são **copiados**. Limites: 32 conversas e 400 mensagens por conversa (as mais antigas saem primeiro).

No app, a lista da esquerda mostra a conversa mais recente primeiro, com um **ponto laranja** quando há mensagens novas; a da direita mostra as mensagens em balões (falas do Pedro e opções escolhidas à direita, as dos outros à esquerda, avisos no centro). A roda do mouse rola a lista e as mensagens. O layout e as cores ficam nas primeiras linhas de `bateboca.c`; a parte da imagem usada como foto é `AVATAR_CROP_Y_FRAC` (ajuste quando a arte final dos personagens chegar).

O app só **lê** o `DialogLog`. Ele não é salvo em disco: `GameInfos_Save/Load` ainda estão vazios no jogo, então o histórico vive só durante a execução (`Dialog_Quit` o apaga).

## DialogStyle: personalizando

Medidas em **pixels virtuais** (960×600); o sistema escala para a janela.

Para variar um estilo, copie o padrão e altere:

```c
static DialogStyle estilo_bravo;

void Setup(void)
{
    estilo_bravo = DialogStyle_Make();                 /* cópia do padrão */
    estilo_bravo.text.color = (SDL_Color){255, 120, 120, 255};
    estilo_bravo.wave.amplitude_px = 5;
    estilo_bravo.face.side = DIALOG_FACE_LEFT;         /* rosto à esquerda */
}
/* depois: DIALOG_LINE_STYLED(C_ANA, CS_ANGRY, "...", &estilo_bravo) */
```

Estilos prontos (globais, criados em `Dialog_Init`): `dialog_styleDefault`, `dialog_styleWhisper`, `dialog_styleShout`.

### `box`: a caixa
| Campo | Efeito |
| --- | --- |
| `screen_margin`, `bottom_gap` | Distância da caixa às laterais e à parte de baixo. |
| `lines` | Linhas de texto visíveis (define a altura). |
| `margin`, `border`, `padding` | Lados (`top/right/bottom/left`) e `.color` de cada camada, como no `Text` da biblioteca. |
| `text_bg` | Fundo da área de texto. |
| `accent` | Cor extra para os decoradores. |
| `text_slack` | Folga vertical extra (px) se seu `alter_char` mexe muito em `y`. |
| `alterMargin/Border/Padding` | `func_alter*` da libgraphical (veja abaixo). `NULL` = camada lisa. |

### `text`: o texto
`font` (`GFONT_REGULAR`, `GFONT_TITLE`...), `scale` (as fontes do cache têm 100 pt; `1/3.6` ≈ 28 px), `color`, `fmt`, `chr_dist`, `lineskip` (0 = da fonte).

### `reveal`: aparição das letras
| Campo | Efeito |
| --- | --- |
| `char_interval_ms` | Tempo entre o início de duas letras (**velocidade da fala**). |
| `char_duration_ms` | Quanto dura a subida de cada letra. |
| `rise_px` | De quantos px abaixo a letra parte. |
| `pause_comma_ms` / `pause_stop_ms` | Pausas depois de `, ; :` e de `. ! ? …` (só quando terminam a palavra: `3.14` não pausa). |
| `ease` | Curva da subida: `DialogEase_Linear`, `_OutQuad`, `_OutCubic` (padrão), `_OutBack` (quicada), `_InOutSine`. Pode ser qualquer `double f(double)`. |

### `wave`: ondulação contínua
`amplitude_px` (0 desliga), `frequency_hz`, `phase_per_char` (defasagem entre letras vizinhas: valores maiores = onda mais "apertada").

### `face`: rosto e gelatina
`side` (`DIALOG_FACE_RIGHT` | `_LEFT` | `_NONE`), `inset`, `gap` e `jelly`:

| `jelly` | Efeito |
| --- | --- |
| `squash` | Quanto estica/achata (0.05 = 5%), ancorado nos "pés". |
| `sway_px` | Balanço lateral do topo (a base fica parada). |
| `frequency_hz` | Velocidade. |
| `sway_phase` | Defasagem do balanço ao longo da altura (efeito de "chicote"). |
| `ramp_in_ms` / `ramp_out_ms` | Rapidez para ligar ao começar a falar / assentar ao terminar. |
| `strips` | Em quantas faixas a imagem é fatiada (mais = mais suave). |

A gelatina liga enquanto a fala aparece e **assenta suavemente** quando ela termina.

### `prompt`
Setinha que quica quando a fala terminou (`size = 0` desliga). Em uma fala com opções, quem convida é o menu e a setinha não aparece.

### `choice`: o menu de opções
Vale para o estilo da **última fala** da cena.

| Campo | O que faz |
| --- | --- |
| `align` | `DIALOG_CHOICE_ALIGN_LEFT` (padrão) ou `_RIGHT`: canto da caixa de fala onde o menu fica. |
| `gap` / `min_width` / `row_gap` | Distância até a caixa de fala, largura mínima e espaço entre opções (px virtuais). |
| `idle_color` / `selected_color` | Cor das opções e da opção selecionada. |
| `marker_size` / `marker_gap` / `marker_color` / `marker_bounce_px` | A setinha ao lado da selecionada (`marker_size = 0` desliga). |
| `selected_wave` | Onda (`amplitude_px`, `frequency_hz`, `phase_per_char`) só da opção selecionada. |
| `delay_ms` | Espera depois da fala terminar, antes de o menu abrir. |
| `stagger_ms` / `duration_ms` / `rise_px` | Atraso entre uma opção e a próxima, duração e distância da subida. |
| `alter_char` | Gancho por letra do menu (igual ao `alter_char` do estilo). Receba `info->option` e `info->selected`. |

## Ganchos: func_alter e alter_char

Estes campos são a ponte direta com os `func_alter*` da biblioteca. As funções são chamadas **a cada quadro**.

**Camadas da caixa** (`box.alterMargin/Border/Padding`): recebem `(Text*, Texture*)` com a textura da camada já como alvo de renderização. Desenhe com `SDL_*` normalmente. Dois exemplos prontos em `dialog_style.c`:

```c
void DialogDecor_PaddingGradient(Text *t, Texture *tex);  /* degradê padding.color -> accent */
void DialogDecor_BorderPulse(Text *t, Texture *tex);      /* borda pulsando border.color <-> accent */
```
Para ler cores do estilo ativo dentro delas use `Dialog_CurrentStyle()`.

**Cada letra** (`alter_char`): é o `func_alterText` da biblioteca com informação extra. Roda **depois** da aparição e da onda:

```c
void MeuEfeito(TextChar *c, const DialogCharInfo *info)
{
    /* info->index, info->time_ms, info->reveal (0..1), info->ui_scale,
       info->option / info->selected (só no menu de opções; na fala: -1 / 0) */
    c->y += 3 * info->ui_scale * sin(info->time_ms / 150.0 + info->index);
    c->color.b = 80;
}
estilo.alter_char = MeuEfeito;
```
Prontos: `DialogChar_Shake` (grito) e `DialogChar_Rainbow`. Multiplique distâncias por `info->ui_scale`; evite `x` negativo (a área de texto não tem folga à esquerda).

## Como funciona por dentro

Por quadro, `Dialog_Render`:

1. **Atualiza** os relógios (`reveal_ms` pode "pular" ao completar a fala; `age_ms` nunca para, por isso a onda e a gelatina continuam) e a energia da gelatina.
2. **Calcula o layout** em px de tela. A caixa tem tamanho **fixo** (`bx_width`/`bx_height` do `Text`); sem isso a caixa "respiraria" junto com a onda, porque a biblioteca mede o texto *depois* do `func_alterText`.
3. **Quebra o texto por palavras** (`DialogFx_WordWrap`), usando as larguras reais dos glifos. A biblioteca só quebra por letra; aqui os espaços viram `'\n'`. Refeito só quando a janela muda.
4. **Desenha o `Text`** com `func_alterText = Dialog_alterChar`, que soma: folga no topo + onda + subida da aparição, e ajusta a opacidade. Letras ainda não reveladas têm alpha 0, então a quebra de linha já está definitiva desde o primeiro quadro.
5. **Desenha o rosto** em faixas horizontais (`SDL_RenderCopyF`) com escala e balanço por faixa.
6. **Desenha a setinha** se a fala terminou (e não há menu).
7. **Menu de opções** (só na última fala de uma cena com `choices`): um segundo `Text`, com a mesma moldura, ocupa o espaço logo acima da caixa. O `func_alterText` dele (`Menu_alterChar`) sobe cada opção em sequência e só deixa a selecionada ondular; o motor descobre a que opção pertence cada letra por uma tabela feita ao abrir o menu. As linhas do menu têm geometria conhecida (`DialogMenuLayout`), usada também para o mouse.

`func_alterText` não recebe dados do usuário, então o motor guarda a fala atual num `static` (por isso há **um** diálogo por vez).

## Integração com o jogo

Já feita nesta entrega:

- `mechanics.h`: removido `#include "dialogs.h"` (o diálogo usa `Character`, definido em `game_mechanics.h`, que inclui `mechanics.h`: manter geraria ciclo). Inclua `mechanics/dialogs.h` onde usar diálogos.
- `main.h`: inclui `mechanics/dialogs.h`.
- `game_mechanics.c`: `Dialog_Init()` / `Dialog_Quit()`.
- `play.c`: `Dialog_Render()` ao fim de `Play_Render`.
- `room.c` e `monitor.c`: `Dialog_HandleInput(e)` no início do tratamento de eventos; **F2** e **F3** (só em `GAME_DEBUG`) tocam as cenas de exemplo.
- `monitor_apps.{h,c}`: `MONITOR_APP_TRASH` / "Lixeira" viraram `MONITOR_APP_BATEBOCA` / "Bate-boca" (`icon_trash.png` foi trocado por `icon_bateboca.png`).
- `window_input.c`: a **roda do mouse** passa a ser entregue ao app sob o cursor (`SDL_MOUSEWHEEL`, usada na rolagem do Bate-boca). Também foi corrigida a variável `pointer_inside`, que era usada sem inicializar nos eventos de teclado.
- `source/CMakeLists.txt`: novos `.c` (`dialog_log.c`, `bateboca.c`).
- `game_infos.h`: removido um `;` indevido dentro do `enum` (erro de compilação que já existia).

## Testes

A matemática (`dialog_fx.c`, inclusive a navegação do menu) e o registro (`dialog_log.c`) são independentes do SDL:

```bash
cd source/Protocolo-Caramelo/mechanics/Dialog
gcc -std=c99 -Wall -Wextra -o /tmp/test_dialog_fx  tests/test_dialog_fx.c  dialog_fx.c -lm && /tmp/test_dialog_fx
gcc -std=c99 -Wall -Wextra -o /tmp/test_dialog_log tests/test_dialog_log.c dialog_log.c    && /tmp/test_dialog_log
```

O desenho (motor, menu e app) depende do SDL e da `libgraphical` e precisa ser conferido jogando: **F3** abre a cena com opções.

## Limitações conhecidas

- **Um diálogo por vez.** Iniciar outro substitui o atual.
- Os rótulos das opções têm de caber em **uma linha** (o menu não quebra linha; o que passar da largura é cortado).
- Com menu aberto, as opções não são numeradas (sem atalho 1/2/3).
- O Bate-boca mostra o histórico, mas não permite **rever a cena** com a caixa de diálogo; e o histórico não é gravado em disco.
- Com `func_alterText` definido, a biblioteca **recria as texturas do texto a cada quadro** (comportamento dela). Para uma caixa de diálogo é barato; evite dezenas de caixas simultâneas.
- A caixa é **maior** que `lines × lineskip`: soma `2 × (amplitude da onda + text_slack)` e `rise_px`, para a onda nunca ser cortada.
- Qualquer glifo ausente na fonte some (ex.: o `…` talvez não exista na Pixeloid; os textos de exemplo usam `...`).
- O jogo não tem módulo de áudio; o som de voz fica por conta do `Dialog_SetCharCallback`.
