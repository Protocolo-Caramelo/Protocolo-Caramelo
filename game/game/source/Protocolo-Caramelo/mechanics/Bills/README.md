# Contas semanais e equilíbrio (30 dias)

**Contas** (`bills.h`): 4 contas por semana (aluguel, luz e água, internet, alimentação), vencimentos nos dias 7, 14, 21 e 28.
Valores = fração da renda semanal de referência (`BILLS_REFERENCE_WEEKLY_INCOME`, 50 000 caramelos) × `1.12^(semana-1)` × fator de dificuldade
(`GDIFF_BILLS_COST_FACTOR`: 0.8 / 1.0 / 1.25 / 1.5). Semana 1 ≈ 20 000, semana 4 ≈ 28 000 (dificuldade normal).
Pagamento: porta do quarto 2 → dono do prédio (`DialogScripts_DoorOwner`). Conta vencida sem pagar = game over.

**Status** (`game_infos.h`, 1 s real = 1 min de jogo, 60 FPS):

| Status | Regra | Efeito |
|---|---|---|
| Energia | linear: `0.0012`/quadro (~4,3 por hora de jogo) + `0.25` por acerto, `0.60` por erro | 100 → 0 em ~11 h acordado |
| Sono | exponencial na falta de energia (`1.0471^(100-e)`) | e=100: ~0,7/h; e=30: ~18/h |
| Saúde | exponencial no sono (`1.00925^(5t)`) | t=0: ~0,2/h; t=100: ~22/h |

**Cama** (`GameInfos_SleepApply`): fração recuperada `f(x) = (B^x - 1)/(B - 1)`, `x = horas/8`.
Energia `B=100` (4 h ≈ 9%, 8 h = 100%), sono `B=10` (8 h zera), saúde `B=3` (8 h = +35).
O cronômetro cai em `GINFO_SLEEP_ANIM_SEC_PER_HOUR` s reais por hora (8 h ≈ 6 s) e avança relógio, contas e status juntos.
