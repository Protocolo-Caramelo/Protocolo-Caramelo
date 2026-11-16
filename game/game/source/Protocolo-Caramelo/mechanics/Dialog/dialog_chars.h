#ifndef DIALOG_CHARS_H
#define DIALOG_CHARS_H

typedef enum Character Character;
typedef enum Character_State Character_State;
enum Character {
    C_PEDRO,
    C_FELIPE,
    C_ANA,
    C_JOSE,
    C_RICARDO,
    C_DONODOPREDIO,
    C_INSPETOR,

    C_COUNT
};
enum Character_State {
    CS_NORMAL,
    CS_HAPPY,
    CS_ANIMATED,
    CS_SAD,
    CS_ANGRY,
    CS_REVOLTED,
    CS_DISTRUSTING,
    CS_DARK,
    CS_SUPERANIMATED,

    CS_COUNT
};

#endif
