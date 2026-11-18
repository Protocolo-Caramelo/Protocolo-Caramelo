#include "dialog_scripts.h"
#include "dialog.h"
#include "dialog_log.h"

/*
 * Como escrever uma cena
 * ----------------------
 *   1. Um vetor `static const DialogLine` com uma DIALOG_LINE por fala
 *      (quem fala, expressao, texto).
 *   2. Para uma fala diferente das outras, use DIALOG_LINE_STYLED e aponte
 *      para um estilo: &dialog_styleShout, &dialog_styleWhisper ou um seu.
 *   3. Um `static const DialogScene` junta as falas. Campos opcionais:
 *        .archive_title  guarda a cena no Bate-boca, na conversa com esse titulo
 *        .choices        menu de opcoes no fim (cada opcao pode levar a outra cena)
 *        .on_finish      funcao chamada quando a cena termina
 *   4. Dialog_PlayScene(&cena).
 *
 * Tudo precisa ser `static`: o sistema guarda ponteiros, nao copias.
 * Escreva as cenas "de baixo para cima": as que sao destino de uma opcao
 * primeiro, a cena de entrada por ultimo (assim nao precisa declarar antes).
 */

/* ================================================================== */
/* Felipe: a apresentacao do Caramelo                                  */
/* ================================================================== */
static void Felipe_CallEnded(void)
{
    DialogLog_AddNote("Felipe", "Chamada encerrada");
}

static const DialogLine felipe_call_lines[] = {
    DIALOG_LINE(C_FELIPE, CS_HAPPY,
        "Fala Pedroca, há quanto tempo! Como você tá?"),
    DIALOG_LINE(C_PEDRO, CS_SAD,
        "Eae Felipe, eu estou bem... eu acho."),
    DIALOG_LINE(C_FELIPE, CS_NORMAL,
        "Eu te entendo, mano. Já estive em uma situação parecida."),
    DIALOG_LINE(C_PEDRO, CS_NORMAL,
        "E como você superou?"),
    DIALOG_LINE(C_FELIPE, CS_ANIMATED,
        "Ora, é justamente isso que eu quero te mostrar!"),
    DIALOG_LINE_STYLED(C_PEDRO, CS_DISTRUSTING,
        "Hum? Como assim?", &dialog_styleWhisper),
    DIALOG_LINE(C_FELIPE, CS_NORMAL,
        "Bom, eu vou ser direto. Você deve saber sobre a crise e o debate "
        "que ocorreram alguns anos antes da gente nascer, não é?"),
    DIALOG_LINE(C_PEDRO, CS_NORMAL,
        "Mais ou menos, eu lembro dos meus pais comentando sobre quando eu "
        "era mais novo."),
    DIALOG_LINE_STYLED(C_FELIPE, CS_SUPERANIMATED,
        "Quero te apresentar o doce Caramelo!", &dialog_styleShout),
    DIALOG_LINE_STYLED(C_PEDRO, CS_DISTRUSTING,
        "Caramelo...? Nunca ouvi falar.", &dialog_styleWhisper),
};

static const DialogScene felipe_call = {
    .archive_title = "Felipe",
    DIALOG_SCENE_LINES(felipe_call_lines),
    .on_finish = Felipe_CallEnded,
};

void DialogScripts_FelipeCall(void)
{
    DialogLog_AddNote("Felipe", "Chamada iniciada");
    Dialog_PlayScene(&felipe_call);
}

/* ================================================================== */
/* Ana: cena com opcoes                                                */
/* ================================================================== */

/* Estilo proprio da Ana: borda laranja, rosto na direita, menu na esquerda
 * (o inverso do padrao, so para mostrar como trocar).
 * Precisa ser criado depois de Dialog_Init (DialogStyle_Make le o padrao). */
static DialogStyle ana_style;
static int ana_style_ready;

static void Ana_EnsureStyle(void)
{
    if(ana_style_ready){
        return;
    }
    ana_style = DialogStyle_Make();
    ana_style.box.border.color      = (SDL_Color){255, 150, 60, 255};
    ana_style.box.padding.color     = (SDL_Color){38, 20, 12, 240};
    ana_style.box.accent            = (SDL_Color){120, 58, 20, 240};
    ana_style.text.color            = (SDL_Color){255, 236, 220, 255};
    ana_style.face.side             = DIALOG_FACE_RIGHT;
    ana_style.wave.amplitude_px     = 3.2;
    ana_style.reveal.char_interval_ms = 30;
    ana_style.choice.align          = DIALOG_CHOICE_ALIGN_LEFT;
    ana_style.choice.selected_color = (SDL_Color){255, 170, 90, 255};
    ana_style.choice.marker_color   = (SDL_Color){255, 170, 90, 255};
    ana_style_ready = 1;
}

static void Ana_CallEnded(void)
{
    DialogLog_AddNote("Ana", "Chamada encerrada");
}

static void Ana_Friendship(void)
{
    DialogLog_AddNote("Ana", "+10 de amizade com Ana");
    Ana_CallEnded();
}

static void Ana_Blocked(void)
{
    DialogLog_AddNote("Ana", "Conversas com Ana e a loja de arte bloqueadas por 48 horas");
    Ana_CallEnded();
}

/* Na versao final, estas funcoes cobrariam os caramelos e registrariam a compra */
static void Ana_BoughtDog(void)
{
    DialogLog_AddNote("Ana", "Compra: desenho de um cachorro caramelo");
}

static void Ana_BoughtCity(void)
{
    DialogLog_AddNote("Ana", "Compra: desenho da cidade");
}

/* --- Sair -------------------------------------------------------------- */
static const DialogLine ana_leave_lines[] = {
    DIALOG_LINE(C_PEDRO, CS_NORMAL,
        "Na verdade eu mudei de ideia e não quero nada não."),
    DIALOG_LINE_STYLED(C_ANA, CS_ANGRY,
        "Ah, sim, entendo. Obrigada por desperdiçar meu precioso tempo, então!",
        &dialog_styleShout),
    DIALOG_LINE(C_PEDRO, CS_DISTRUSTING,
        "...Até mais então."),
    DIALOG_LINE_STYLED(C_ANA, CS_REVOLTED,
        "Não se dê o esforço de aparecer aqui por um bom tempo, ouviu, idiota?",
        &dialog_styleShout),
};

static const DialogScene ana_leave = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(ana_leave_lines),
    .on_finish = Ana_Blocked,
};

/* --- Conversar --------------------------------------------------------- */
static const DialogLine ana_talk_lines[] = {
    DIALOG_LINE(C_PEDRO, CS_NORMAL,
        "Eu só queria conversar um pouco com alguém. O Felipe não deu as caras hoje."),
    DIALOG_LINE(C_ANA, CS_NORMAL,
        "Hmph... até que você não é tão ruim assim."),
    DIALOG_LINE_STYLED(C_PEDRO, CS_DISTRUSTING,
        "Hein?", &dialog_styleWhisper),
    DIALOG_LINE(C_ANA, CS_NORMAL,
        "Geralmente pessoas entediadas preferem ficar dentro do seu próprio "
        "mundinho, olhando para uma tela."),
    DIALOG_LINE(C_PEDRO, CS_HAPPY,
        "Hehe, digamos que eu sou das antigas!"),
    DIALOG_LINE(C_ANA, CS_HAPPY,
        "Ksks... isso não é uma coisa ruim. Às vezes, manter alguns costumes "
        "do passado é bom."),
    DIALOG_LINE(C_PEDRO, CS_ANIMATED,
        "Socializar é um bom exemplo! Ou então fazer um desenho no papel."),
    DIALOG_LINE(C_ANA, CS_HAPPY,
        "Acho que você tem razão."),
    DIALOG_LINE(C_PEDRO, CS_ANIMATED,
        "Vai começar uma sessão agora! Era disso que eu precisava."),
    DIALOG_LINE(C_ANA, CS_NORMAL,
        "Eu percebi, a minha vai começar também."),
    DIALOG_LINE(C_PEDRO, CS_HAPPY,
        "Então boa sorte para você e até mais!"),
    DIALOG_LINE(C_ANA, CS_SAD,
        "...Até mais."),
};

static const DialogScene ana_talk = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(ana_talk_lines),
    .on_finish = Ana_Friendship,
};

/* --- Comprar um desenho: outro menu dentro da cena ---------------------- */
static const DialogLine ana_sold_lines[] = {
    DIALOG_LINE(C_ANA, CS_HAPPY,
        "Obrigada... fazia um tempo desde a última vez que eu consegui vender "
        "algo. Acabei de entregar para o carinha das entregas rápidas, deve "
        "chegar na sua casa amanhã de manhã."),
    DIALOG_LINE(C_PEDRO, CS_HAPPY,
        "Eu que agradeço!"),
    DIALOG_LINE(C_ANA, CS_NORMAL,
        "Boa sorte na sua sessão. Até mais."),
};

static const DialogScene ana_sold = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(ana_sold_lines),
    .on_finish = Ana_CallEnded,
};

static const DialogChoice ana_art_choices[] = {
    { .label = "Desenho de um cachorro caramelo", .on_select = Ana_BoughtDog,  .next = &ana_sold },
    { .label = "Desenho da cidade",               .on_select = Ana_BoughtCity, .next = &ana_sold },
};

static const DialogLine ana_buy_lines[] = {
    DIALOG_LINE(C_PEDRO, CS_NORMAL,
        "Na verdade, eu queria comprar um dos seus desenhos."),
    DIALOG_LINE(C_ANA, CS_HAPPY,
        "Espera, é sério?! Ah... nesse caso tudo bem, tenho alguns prontos "
        "aqui em casa parados. Qual você quer?"),
};

static const DialogScene ana_buy = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(ana_buy_lines),
    DIALOG_SCENE_CHOICES(ana_art_choices),
};

/* --- Entrada: a cena que abre a ligacao -------------------------------- */
static const DialogChoice ana_choices[] = {
    { .label = "Conversar",          .next = &ana_talk  },
    { .label = "Comprar um desenho", .next = &ana_buy   },
    { .label = "Sair",               .next = &ana_leave },
};

/* O ENDERECO de `ana_style` ja e conhecido em tempo de compilacao, entao a
 * cena pode apontar para ele; o CONTEUDO do estilo e preenchido em
 * Ana_EnsureStyle, antes de a cena comecar. */
static const DialogLine ana_call_lines[] = {
    DIALOG_LINE_STYLED(C_ANA, CS_ANGRY,
        "O que você quer, amigo do magricela? Veio me perturbar de novo?", &ana_style),
    DIALOG_LINE(C_PEDRO, CS_NORMAL,
        "Bom dia para você também..."),
    DIALOG_LINE_STYLED(C_ANA, CS_ANGRY,
        "Eu já te disse para parar de encher meu saco, a menos que você queira alguma coisa?",
        &ana_style),
};

static const DialogScene ana_call = {
    .archive_title = "Ana",
    DIALOG_SCENE_LINES(ana_call_lines),
    DIALOG_SCENE_CHOICES(ana_choices),
};

void DialogScripts_AnaCall(void)
{
    Ana_EnsureStyle();
    DialogLog_AddNote("Ana", "Chamada iniciada");
    Dialog_PlayScene(&ana_call);
}
