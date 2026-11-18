#ifndef SCRN_GAMEOVER_H
#define SCRN_GAMEOVER_H

typedef enum GameOver_Reason GameOver_Reason;
enum GameOver_Reason {
    GAMEOVER_HEALTH,   /* a saude chegou a zero */
    GAMEOVER_BILL      /* uma conta semanal venceu sem ser paga */
};

/* Muda o estado do jogo para a tela de game over (idempotente). */
void GameOver_Trigger(GameOver_Reason reason);

/* Desenha a tela e trata os eventos (Enter, Espaco, Esc ou clique: volta ao menu). */
void GameOver_Render();

#endif
