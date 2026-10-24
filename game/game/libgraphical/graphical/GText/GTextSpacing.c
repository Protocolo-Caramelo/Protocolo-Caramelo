#include"GTextSpacing.h"

static int Color_IsUnset(const SDL_Color*c)
{
    return c->r == 0 && c->g == 0 && c->b == 0 && c->a == 0;
}

//
// Methods
//
void TextSpacing_Populate(TextSpacing*a, const TextSpacing*b)
{
    if(a->top == TEXTSPACING_UNINITIALIZED_VAL){
        a->top = b->top;
    }
    if(a->bottom == TEXTSPACING_UNINITIALIZED_VAL){
        a->bottom = b->bottom;
    }
    if(a->right == TEXTSPACING_UNINITIALIZED_VAL){
        a->right = b->right;
    }
    if(a->left == TEXTSPACING_UNINITIALIZED_VAL){
        a->left = b->left;
    }

    if(Color_IsUnset(&a->color)){
        a->color = b->color;
    }
}

int TextSpacing_Equals(const TextSpacing*a, const TextSpacing*b)
{
    return
        a->left == b->left
        && a->right == b->right
        && a->top == b->top
        && a->bottom == b->bottom

        && a->color.a == b->color.a
        && a->color.r == b->color.r
        && a->color.g == b->color.g
        && a->color.b == b->color.b
        ;
}

int TextSpacing_IsUninitialized(const TextSpacing*a)
{
    return
        a->left == TEXTSPACING_UNINITIALIZED_VAL
        && a->right == TEXTSPACING_UNINITIALIZED_VAL
        && a->top == TEXTSPACING_UNINITIALIZED_VAL
        && a->bottom == TEXTSPACING_UNINITIALIZED_VAL
        ;
}
