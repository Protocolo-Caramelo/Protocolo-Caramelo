# graphical

Biblioteca mínima em C sobre o **SDL2** que oferece estruturas prontas para uso geral: **Window**, **Texture** e **Text** (texto com margem, borda, preenchimento, rotação, formatação por caractere e cache de glifos).

> Escrita em C (padrão GNU C11), no estilo "structs + funções": cada tipo `X` tem funções `X_Algo(...)`.

---

## Sumário

1. [Requisitos](#requisitos)
2. [Estrutura do projeto](#estrutura-do-projeto)
3. [Compilação](#compilação)
4. [Início rápido](#início-rápido)
5. [Convenções](#convenções)
6. [Módulos](#módulos)
   - [Inicialização (`GInit`)](#inicialização-ginit)
   - [Window](#window)
   - [Texture](#texture)
   - [TextCache](#textcache)
   - [Text](#text)
   - [TextConfig](#textconfig)
   - [Modelo de caixa](#modelo-de-caixa-margin--border--padding)
   - [Funções de alteração](#funções-de-alteração-callbacks)
   - [Arestas (`edges`)](#arestas-edges)
   - [Formatação de caracteres](#formatação-de-caracteres)
7. [Testes interativos (`GTest`)](#testes-interativos-gtest)
8. [Limitações conhecidas](#limitações-conhecidas)
9. [Dependências embutidas](#dependências-embutidas)

---

## Requisitos

| Item | Versão |
| --- | --- |
| Compilador C | GCC ou Clang (a biblioteca usa extensões GNU: `{}` vazio e `##__VA_ARGS__`) |
| CMake | 3.23 ou superior (usa `FILE_SET`) |
| SDL2 | 2.x |
| SDL2_image | com suporte a **PNG** e **JPG** |
| SDL2_ttf | 2.x |

Em distribuições Debian/Ubuntu:

```bash
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

## Estrutura do projeto

```
.
├── CMakeLists.txt
├── graphical/
│   ├── graphical.h        # cabeçalho "guarda-chuva": inclua só este
│   ├── GCommons.h         # macros utilitárias (WRAP_FIELD, DEFAULT_VALUE, logs...)
│   ├── GInit.{h,c}        # graphical_init / graphical_quit
│   ├── GWindow.{h,c}      # struct Window (com vtable de callbacks)
│   ├── GTexture.{h,c}     # struct Texture
│   ├── GText/             # tudo sobre texto
│   │   ├── GText.{h,c}            # struct Text (desenho, medidas, texturas)
│   │   ├── GTextCache.{h,c}       # atlas de glifos de uma fonte
│   │   ├── GTextCacheGlyph.{h,c}  # tabela hash glifo -> retângulo no atlas
│   │   ├── GTextConfig.{h,c}      # configuração do texto
│   │   ├── GTextSpacing.{h,c}     # margem / borda / preenchimento
│   │   └── GTextChar*.h           # tipos de caractere e formatação
│   └── GTest/             # testes interativos (opcionais)
├── utf8/                  # utf8.h (sheredom) — somente cabeçalho
└── uthash/                # uthash — somente cabeçalho
```

## Compilação

Como biblioteca estática, a partir da raiz do projeto:

```bash
cmake -S . -B build
cmake --build build
```

Para compilar também as rotinas de teste interativas (`GTest/*`):

```bash
cmake -S . -B build -DGRAPHICAL_BUILD_TESTS=ON
cmake --build build
```

Para usar em outro projeto CMake:

```cmake
add_subdirectory(libgraphical)
target_link_libraries(meu_programa PRIVATE graphical)
```

`graphical` já propaga `utf8`, `uthash` e as bibliotecas do SDL como dependências públicas. Se o seu projeto já gerencia o SDL por conta própria, defina `graphical_link_libraries` **antes** do `add_subdirectory` e a busca automática é ignorada:

```cmake
set(graphical_link_libraries SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
```

> Os códigos usam `#include <SDL2/SDL.h>`, então o diretório **pai** de `SDL2/` precisa estar no caminho de inclusão (já é o caso em instalações padrão do Linux).

## Início rápido

```c
#include <graphical.h>

int main(void)
{
    if(graphical_init() != 0){
        return 1;
    }

    SDL_Window *w = SDL_CreateWindow("Exemplo", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

    // 1) Um TextCache por fonte/tamanho; vários Text podem compartilhá-lo
    TextCache *cache = TextCache_Create(r, "fonte.ttf", 24);
    if(!cache){
        return 1;
    }

    // 2) Um Text com a string inicial
    Text *texto = Text_Create(cache, NULL, "Olá, mundo! ção ✓");
    *Text_color(texto) = (SDL_Color){20, 20, 200, 255};

    int rodando = 1;
    SDL_Event e;
    while(rodando){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                rodando = 0;
            }
        }

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        // 3) Desenha; só o que mudou é recalculado
        Text_DrawUTF8(texto, NULL, TextConfig_Create(.x = 50, .y = 40), NULL);

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }

    // 4) Libera na ordem inversa da criação
    Text_Destroy(&texto);
    TextCache_Destroy(&cache);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    graphical_quit();
    return 0;
}
```

## Convenções

### Acessores `WRAP_FIELD`

Os campos das structs são acessados por funções geradas pela macro `WRAP_FIELD`, que devolvem um **ponteiro** para o campo. Por isso a leitura e a escrita usam `*`:

```c
*Text_x(t) = 100;                 // escreve
double x = *Text_x(t);            // lê
*Text_paddingLeft(t) = 8;
```

O nome é `Tipo_campo`. Alguns campos têm nome "amigável" diferente do campo interno (ex.: `Text_colorBackground` acessa `config.color_bg`).

### Valores "não definidos" (herança de configuração)

Em um `TextConfig`, qualquer campo que **não** foi preenchido significa "use o valor atual do `Text`":

- números: `INVALID_VAL` (`INT_MAX`);
- ponteiros de função: `INVALID_PTR`;
- lados de margem/borda/preenchimento: `TEXTSPACING_UNINITIALIZED_VAL` (`INT_MAX`);
- cores: `{0, 0, 0, 0}` (**atenção**: não é possível pedir explicitamente "preto totalmente transparente" por esse caminho).

`TextConfig_Create(...)` já inicializa tudo como "não definido" e deixa você preencher só o que quiser com *designated initializers*.

### Erros e logs

- Funções que podem falhar retornam `NULL` ou um código (`graphical_init` devolve `0`/`-1`) e registram o motivo via `SDL_LogMessage` (macros `GRAPHICAL_LOG_*`).
- A biblioteca **não chama `exit()`**; cabe a quem a usa decidir o que fazer.
- Funções `*_Destroy` aceitam `NULL` e zeram o ponteiro recebido (`*x = NULL`).

## Módulos

### Inicialização (`GInit`)

```c
int  graphical_init(void);   // SDL (vídeo) + SDL_image (PNG/JPG) + SDL_ttf. 0 = ok, -1 = falha
void graphical_quit(void);   // encerra tudo; chame depois de destruir os objetos
```

### Window

Estrutura leve que agrupa a janela, o renderer e um conjunto opcional de callbacks:

```c
struct Window {
    WindowVTable vtable;   // init, exit, render, present, event
    SDL_Window *raw;
    SDL_Renderer *renderer;
};
```

Os *wrappers* `Window_init`, `Window_exit`, `Window_render`, `Window_present` e `Window_event` chamam o callback correspondente e **ignoram** os que forem `NULL`.

```c
static void meu_render(Window *w){ /* desenha */ }

Window win = {0};
win.vtable.render = meu_render;
// ... preencher win.raw e win.renderer com SDL_CreateWindow / SDL_CreateRenderer
Window_render(&win);
```

### Texture

```c
struct Texture { SDL_Texture *raw; SDL_Renderer *renderer; };

void Texture_Clear(Texture *t);   // limpa com branco transparente (255,255,255,0)
```

`Texture_Clear` restaura o alvo de renderização e a cor de desenho que estavam ativos.

### TextCache

Atlas de glifos de **uma fonte em um tamanho**. Os glifos são renderizados sob demanda (com `TTF_*UNICODE`) e guardados numa textura (`RGBA8888`, alvo de renderização) que começa com 256×256 e dobra de tamanho quando falta espaço.

| Função | Descrição |
| --- | --- |
| `TextCache_Create(renderer, caminho, pt)` | Cria o cache. Retorna `NULL` em caso de erro (fonte inválida, sem memória, textura) |
| `TextCache_GetGlyph(c, codepoint, fmt)` | Retângulo do glifo no atlas (cria se preciso). `NULL` se não puder ser renderizado |
| `TextCache_GetGlyphSize(c, codepoint, fmt, &w, &h)` | Tamanho do glifo (`0`,`0` se indisponível) |
| `TextCache_GetAtlasSize` / `SetAtlasSize` | Consulta / redimensiona o atlas, reposicionando os glifos |
| `TextCache_GetGlyphAmt(c)` | Quantidade de glifos em cache |
| `TextCache_Destroy(&c)` | Libera tudo |

Um mesmo `TextCache` pode ser compartilhado por vários `Text`, e deve viver mais que eles.

### Text

Texto em UTF-8 com caixa completa e cache de texturas.

**Criação** — a macro `Text_Create` aceita de 1 a 3 argumentos:

```c
Text *a = Text_Create(cache);                                  // vazio
Text *b = Text_Create(cache, TextConfig_Create(.wrap = 300));  // com configuração
Text *c = Text_Create(cache, NULL, "Olá");                     // com texto
Text *d = Text_Create(cache, TextConfig_Create(.x = 10), "Olá");
```

**Desenho:**

```c
void Text_DrawUTF8(Text *t, char *str, TextConfig *tc, int (*edges)[DRAW_EDGE_AMT]);
```

- `str`: novo texto; `NULL` mantém o atual.
- `tc`: configuração desta chamada; `NULL` mantém a atual (campos "não definidos" herdam do `Text`).
- `edges`: se não for `NULL`, recebe as arestas (veja [Arestas](#arestas-edges)).

Chamar `Text_DrawUTF8` todo quadro é o uso esperado: as texturas só são regeradas quando o texto ou a configuração mudam.

**Texto:**

| Função | Descrição |
| --- | --- |
| `Text_SetUTF8(t, str)` | Copia a string (`NULL` limpa) |
| `Text_GetUTF8(t)` | String atual (não modifique) |
| `Text_GetSizeUTF8(t)` | Número de **caracteres** (codepoints) |
| `Text_GetLenUTF8(t)` | Tamanho em **bytes**, incluindo o `'\0'` |
| `Text_getUTF8(t, i)` | Codepoint no índice `i` |
| `Text_Destroy(&t)` | Libera o `Text` (não libera o `TextCache`) |

**Propriedades** (use `*Text_nome(t)`):

| Propriedade | Tipo | Padrão | Significado |
| --- | --- | --- | --- |
| `color` | `SDL_Color` | `{0,0,0,255}` | Cor dos caracteres |
| `colorBackground` | `SDL_Color` | `{0,0,0,0}` | Fundo da caixa de texto |
| `lineskip` | `unsigned` | da fonte | Distância entre linhas |
| `wrap` | `unsigned` | `0` | Largura máxima da linha antes de quebrar (`0` = sem quebra) |
| `rotation` | `double` | `0` | Rotação do texto inteiro (graus) |
| `flip` | `SDL_RendererFlip` | `NONE` | Espelhamento do texto inteiro |
| `boxWidthText` / `boxHeightText` | `int` | `0` | Tamanho fixo da área do texto (negativo estende para o lado oposto) |
| `scaleWidth` / `scaleHeight` | `double` | `1` | Escala dos glifos |
| `chrDist` | `int` | `0` | Espaço extra entre caracteres |
| `chrRotation` | `double` | `0` | Rotação de cada caractere (graus) |
| `chrFmt` | `TextCharFormat` | `CHAR_FMT_REGULAR` | Formatação dos caracteres |
| `chrFlip` | `SDL_RendererFlip` | `NONE` | Espelhamento de cada caractere |
| `x`, `y` | `double` | `0` | Posição |
| `padding*`, `border*`, `margin*` | `unsigned` / `SDL_Color` | `0` | Veja [Modelo de caixa](#modelo-de-caixa-margin--border--padding) |

**Medidas e posições** (úteis para layout e *hit-test*): `Text_MeasureUTF8/Padding/Border/Margin(t, &w, &h)` e `Text_PositionUTF8/Padding/Border/Margin(t, &x, &y)`.

**Texturas avulsas:** `Text_GenTextureUTF8/Padding/Border/Margin(t)` devolvem uma `SDL_Texture*` nova (quem chama é dono dela). Podem devolver `NULL` se não houver área a desenhar.

### TextConfig

```c
Text_DrawUTF8(t, NULL, TextConfig_Create(
    .x = 100, .y = 50,
    .rotation = 15,
    .color = (SDL_Color){255, 0, 0, 255},
    .padding = { .left = 6, .right = 6, .top = 3, .bottom = 3 }
), NULL);
```

`TextConfig_Create(...)` expande para um *compound literal* (`&(TextConfig){...}`) que vale **apenas no bloco onde é escrito**. Use-o direto na chamada, como acima; não guarde o ponteiro para depois.

> Ao desenhar, a configuração recebida é completada (`TextConfig_Populate`) com os valores do `Text`; ou seja, o `TextConfig` passado é modificado.

### Modelo de caixa (margin → border → padding)

```
┌─────────────── margin ───────────────┐
│ ┌──────────── border ──────────────┐ │
│ │ ┌───────── padding ────────────┐ │ │
│ │ │          texto               │ │ │
│ │ └──────────────────────────────┘ │ │
│ └──────────────────────────────────┘ │
└──────────────────────────────────────┘
```

Cada camada tem `top`, `right`, `bottom`, `left` (em pixels) e `color`:

```c
*Text_paddingLeft(t) = *Text_paddingRight(t) = 8;
*Text_borderTop(t) = *Text_borderBottom(t) = *Text_borderLeft(t) = *Text_borderRight(t) = 2;
*Text_borderColor(t) = (SDL_Color){0, 0, 0, 255};
*Text_marginColor(t) = (SDL_Color){255, 255, 255, 0};
```

Camadas com todos os lados `0` não geram textura.

### Funções de alteração (callbacks)

Permitem personalizar o desenho. Podem ser passadas em `TextConfig_Create` (`.func_alterText = ...`) ou definidas com `*Text_fAlterText(t) = ...`.

| Callback | Assinatura | Quando é chamado |
| --- | --- | --- |
| `func_alterText` | `void f(Text*, TextChar*)` | Para **cada caractere** durante a medição; pode mudar `x`, `y`, `rotation`, `color`, `width`, `height`... |
| `func_alterMargin` | `void f(Text*, Texture*)` | Ao gerar a textura da margem (já como alvo de renderização) |
| `func_alterBorder` | `void f(Text*, Texture*)` | Ao gerar a textura da borda |
| `func_alterPadding` | `void f(Text*, Texture*)` | Ao gerar a textura do preenchimento |

Exemplo — onda vertical nos caracteres:

```c
static void onda(Text *t, TextChar *c)
{
    (void)t;
    c->y += 6 * sin(SDL_GetTicks() / 200.0 + c->chr_textIdx * 0.5);
}

Text_DrawUTF8(t, NULL, TextConfig_Create(.func_alterText = onda), NULL);
```

Enquanto algum callback estiver definido, as texturas correspondentes são regeradas **a cada quadro**.

### Arestas (`edges`)

`int edges[DRAW_EDGE_AMT]` (30 posições) é indexado pelo enum `DRAW_EDGES`:

- posições das arestas: `DRAW_EDGE_{XMIN,XMAX,YMIN,YMAX}` (caixa completa) e as variantes `DRAW_EDGE_{MARGIN,BORDER,PADDING,TEXT}_*`;
- dimensões: `DRAW_WIDTH`, `DRAW_HEIGHT`, `DRAW_{MARGIN,BORDER,PADDING,TEXT}_{WIDTH,HEIGHT}`.

Duas formas de obter:

- `Text_DrawUTF8(..., &edges)` copia as arestas já giradas e posicionadas (coordenadas de tela);
- `*Text_edges(t)` devolve as mesmas arestas calculadas na hora.

`*Text_chrsDest(t)` expõe, por caractere, a posição final (`TextCharRect`), útil para cursor/seleção. Caracteres sem desenho (como `'\n'`) têm `src == NULL`.

### Formatação de caracteres

`TextCharFormat` combina com `|` os estilos do SDL_ttf:

```c
CHAR_FMT_REGULAR, CHAR_FMT_BOLD, CHAR_FMT_ITALIC,
CHAR_FMT_UNDERLINE, CHAR_FMT_STRIKETHROUGH

*Text_chrFmt(t) = CHAR_FMT_BOLD | CHAR_FMT_UNDERLINE;
```

Valores fora dessas combinações são rejeitados por `Text_DrawUTF8` (que registra o erro e não desenha). O teste pode ser feito com `CHAR_FMT_AVAILABLE(fmt)`.

## Testes interativos (`GTest`)

Com `-DGRAPHICAL_BUILD_TESTS=ON` ficam disponíveis (declaradas em `GTest/GTest.h`):

```c
void SimpleWindow();
void TextCG_Test();
void TextCache_Test(char *fonte, unsigned pt);
void Text_Test1(char *fonte, unsigned pt, char *texto);   // idem Test2, Test3, Test4
```

Cada uma abre uma janela própria e só termina ao fechá-la. `Text_Test4` é a mais completa (campo de texto, botões, bordas e rotações). Há um `GText_Test4_Asset1.jpeg` em `GTest/` usado por ela.

## Limitações conhecidas

- **Apenas o plano básico do Unicode (U+0000–U+FFFF).** As funções `TTF_*UNICODE` usam UCS-2; codepoints acima disso (por exemplo, emojis) são desenhados como `U+FFFD` e um aviso é registrado.
- **Cor `{0,0,0,0}` em `TextConfig` significa "não definida"** (herda do `Text`).
- **A rotação do texto** (`rotation`) é feita em torno do centro da caixa; as arestas retornadas por `Text_edges` são caixas alinhadas aos eixos, não polígonos girados.
- **`Text_getUTF8(t, i)` é O(i)**; para percorrer o texto inteiro use `Text_GetUTF8` com `utf8codepoint` (a biblioteca já faz assim internamente).
- **Uma thread:** a biblioteca (como o próprio renderer do SDL) não é thread-safe.
- Nomes legados mantidos por compatibilidade: `GRAPHICAL_LOG_WARM` (use `GRAPHICAL_LOG_WARN`), `CHAR_FMT_AVAIBLE` (use `CHAR_FMT_AVAILABLE`), `TextSpacing_Zered` (use `TextSpacing_IsUninitialized`).

## Dependências embutidas

- [`utf8.h`](https://github.com/sheredom/utf8.h) — domínio público (Unlicense).
- [`uthash`](https://troydhanson.github.io/uthash/) — BSD revisado.
