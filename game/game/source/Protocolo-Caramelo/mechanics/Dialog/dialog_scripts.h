#ifndef DIALOG_SCRIPTS_H
#define DIALOG_SCRIPTS_H

/*
 * Cenas de dialogo do jogo. Cada cena e uma funcao: chame-a no ponto da
 * historia em que ela deve acontecer.
 */

/* Capitulo II: a videochamada em que Felipe apresenta o Caramelo (trecho).
 * Fica guardada no Bate-boca, na conversa "Felipe". */
void DialogScripts_FelipeCall(void);

/* Capitulo II: Pedro liga para a Ana. Exemplo completo de cena com opcoes:
 * Conversar / Comprar um desenho (com outro menu) / Sair.
 * Fica guardada no Bate-boca, na conversa "Ana". */
void DialogScripts_AnaCall(void);

/* A porta: Pedro conversa com o dono do prédio e paga as contas semanais.
 * O menu é montado na hora, com as contas que estiverem pendentes. */
void DialogScripts_DoorOwner(void);

#endif
