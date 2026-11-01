# Desktop do monitor

Sistema de janelas, ícones e barra de tarefas que vive **dentro** do monitor do jogo (não cria janelas do SO).

- **Ícones**: clique duplo abre o app; arraste para mover. Ao soltar, o ícone "encaixa" na célula livre mais próxima da grade (uma sombra mostra onde ele vai parar).
- **Janelas**: arraste a barra de título para mover; arraste bordas/cantos para redimensionar; clique em uma janela para trazê-la para a frente.
- **Botões da barra de título**: minimizar, maximizar/restaurar e fechar. Clique duplo na barra de título também maximiza/restaura. Janela maximizada ocupa toda a área útil e fica fixa.
- **Barra de tarefas**: um botão por janela aberta (na ordem de abertura). Clicar: restaura se estiver minimizada, minimiza se for a ativa, ou traz para a frente.
- **Gerenciador de Tarefas**: app que lista os aplicativos em execução com o estado de cada um (Aberta / Minimizada / Maximizada). Clicar na linha foca a janela; **Encerrar** fecha.

## Módulos

| Arquivo | Responsabilidade |
|---|---|
| `monitor_space` | Sistema de coordenadas. Tudo é descrito em *unidades do monitor* (pixels de `Monitor_view_*.png`, 2283x1568) e convertido para pixels da tela. Define a área útil (`MONITOR_AREA_*`). |
| `monitor_apps` | Tabela dos apps: rótulo, título, ícone, célula inicial na grade, tamanhos e funções opcionais (`init`, `quit`, `draw_content`, `handle_event`). |
| `click_counter` | Conta cliques consecutivos no mesmo alvo (clique duplo). Usado pelos ícones e pela barra de título. |
| `icon_grid` | A grade de ícones (células, origem, conversão posição → célula). |
| `desktop_icon` | Um ícone: sprite + rótulo, desenho, teste de clique, movimento limitado à área. |
| `icon_layer` | Todos os ícones + mouse deles: arrastar, encaixar na grade, selecionar, abrir. |
| `desktop_window` | Uma janela: geometria, estado (maximizada/minimizada), *hit-test*, mover/redimensionar, desenho. |
| `window_stack` | Lista das janelas abertas, na ordem de empilhamento (z) e na ordem de abertura. Só guarda e consulta. |
| `window_manager` | O que se pode fazer com janelas: abrir, fechar, focar, minimizar, maximizar/restaurar, desenhar. |
| `window_input` | Mouse sobre as janelas: foco, mover, redimensionar, botões da barra de título, repasse de eventos ao app. |
| `taskbar` | Barra de tarefas: botões das janelas abertas e seus cliques. |
| `app_taskmgr` | O app Gerenciador de Tarefas (usa só a API de `window_stack`/`window_manager`). |
| `monitor_desktop` | Fachada: junta tudo. É a única coisa que `screens/play/monitor.c` usa. |
| `monitor_text` / `monitor_draw` / `monitor_cursor` | Utilitários: `Text` escalado, retângulos/linhas, formas do cursor. |

Dependências (de cima para baixo, nunca o contrário):
`monitor_desktop` → `window_input`, `taskbar`, `icon_layer` → `window_manager` → `window_stack` → `desktop_window` → `monitor_apps`.
`app_taskmgr` é chamado por `monitor_apps` (callbacks), mas só enxerga `window_stack`/`window_manager`.

## Fluxo de um evento do mouse

Quem recebe, em ordem (`MonitorDesktop_HandleEvent`):

1. quem está "segurando" o mouse (ícone sendo arrastado ou botão da barra de tarefas pressionado);
2. janelas (estão acima dos ícones) — o conteúdo é repassado ao `handle_event` do app;
3. barra de tarefas;
4. ícones.

## Adicionar um app

1. Inclua um id em `MonitorAppId` (`monitor_apps.h`), antes de `MONITOR_APP_COUNT`.
2. Preencha a entrada em `monitor_apps.c` (rótulo, título, `icon_path`, `icon_cell` = coluna/linha da grade, tamanhos).
3. Gere o sprite do ícone (PNG com transparência) em `assets/images/monitor/`.
4. Opcional, para dar vida ao app:
   - `draw_content(const SDL_Rect *body)`: desenha o conteúdo (`body` em pixels, já recortado; use `MonitorSpace_TextScale()` para textos).
   - `handle_event(e, body, pointer_inside)`: recebe movimento/pressionar/soltar do mouse. O "pressionar" só chega com `pointer_inside = 1`; o "soltar" sempre chega.
   - `init` / `quit`: criar e destruir recursos (textos, texturas) uma vez.
   Sem `draw_content`, a janela mostra "Em construção...". `app_taskmgr.c` é um exemplo completo.

## Ajustes rápidos

| O quê | Onde |
|---|---|
| Tamanho/posição da grade de ícones | `ICON_GRID_*` em `icon_grid.h` |
| Abrir com 1 clique em vez de 2 | `ICON_OPEN_CLICKS` em `icon_layer.h` |
| Tempo do clique duplo | `CLICK_MULTI_MS` em `click_counter.h` |
| Limiar de arrasto dos ícones | `ICON_DRAG_THRESHOLD_PX` em `icon_layer.h` |
| Barra de título, botões, área de redimensionar | `DWIN_*` em `desktop_window.h` |
| Cores das janelas | `COLOR_*` no topo de `desktop_window.c` |
| Área e tamanho dos botões da barra de tarefas | `TASKBAR_*` em `taskbar.h` |
| Área útil dentro do monitor | `MONITOR_AREA_*` em `monitor_space.h` |
| Tamanho mínimo/inicial de cada janela | `monitor_apps.c` |

## Assets

Os ícones estavam desenhados na própria imagem do monitor. O script
`tools/extract_monitor_icons.py` os separa em sprites transparentes
(`assets/images/monitor/icon_*.png`) e gera os fundos sem ícones
(`Monitor_view_*_clean.png`), usados em `GAMEPLAY_TEX_PATHS`.
O ícone do Gerenciador de Tarefas é gerado por `tools/make_taskmgr_icon.py`
(no mesmo estilo de traços pretos). As imagens originais não foram alteradas.
Se redesenhar a arte, rode os scripts de novo ou substitua os PNGs diretamente.
