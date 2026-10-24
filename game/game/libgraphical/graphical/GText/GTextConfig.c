#include"GTextConfig.h"

//
// Methods
//
int TextConfig_Equals(TextConfig*a, TextConfig*b)
{
    return 
        TextSpacing_Equals(&a->padding, &b->padding)
        && TextSpacing_Equals(&a->border, &b->border)
        && TextSpacing_Equals(&a->margin, &b->margin)

        && a->color.a == b->color.a
        && a->color.r == b->color.r
        && a->color.g == b->color.g
        && a->color.b == b->color.b
        //
        && a->color_bg.a == b->color_bg.a
        && a->color_bg.r == b->color_bg.r
        && a->color_bg.g == b->color_bg.g
        && a->color_bg.b == b->color_bg.b
        //
        && a->lineskip == b->lineskip
        && a->wrap == b->wrap
        // && a->rotation == b->rotation
        //
        && a->scale_width == b->scale_width
        && a->scale_height == b->scale_height
        //
        && a->bx_width == b->bx_width
        && a->bx_height == b->bx_height
        //
        && a->chr_dist == b->chr_dist
        && a->chr_fmt == b->chr_fmt
        && a->chr_rotation == b->chr_rotation
        && a->chr_flip == b->chr_flip
        //
        // && a->x == b->x
        // && a->y == b->y
        //
        && a->func_alterMargin == b->func_alterMargin
        && a->func_alterBorder == b->func_alterBorder
        && a->func_alterPadding == b->func_alterPadding
        && a->func_alterText == b->func_alterText
        ;
}

int TextConfig_TextuallyEquals(TextConfig*a, TextConfig*b){
    return 
        a->color.a == b->color.a
        && a->color.r == b->color.r
        && a->color.g == b->color.g
        && a->color.b == b->color.b
        //
        && a->color_bg.a == b->color_bg.a
        && a->color_bg.r == b->color_bg.r
        && a->color_bg.g == b->color_bg.g
        && a->color_bg.b == b->color_bg.b
        //
        && a->lineskip == b->lineskip
        && a->wrap == b->wrap
        // && a->rotation == b->rotation
        //
        && a->scale_width == b->scale_width
        && a->scale_height == b->scale_height
        //
        && a->bx_width == b->bx_width
        && a->bx_height == b->bx_height
        //
        && a->chr_dist == b->chr_dist
        && a->chr_fmt == b->chr_fmt
        && a->chr_rotation == b->chr_rotation
        && a->chr_flip == b->chr_flip
        ;
}


void TextConfig_Populate(TextConfig*a, const TextConfig*b)
{
    TextSpacing_Populate(&a->padding, &b->padding);
    TextSpacing_Populate(&a->border, &b->border);
    TextSpacing_Populate(&a->margin, &b->margin);

    if(a->color.r == 0 && a->color.g == 0 && a->color.b == 0 && a->color.a == 0){
        a->color = b->color;
    }
    if(a->color_bg.r == 0 && a->color_bg.g == 0 && a->color_bg.b == 0 && a->color_bg.a == 0){
        a->color_bg = b->color_bg;
    }
    DEFAULT_VALUE(a->lineskip, b->lineskip, INVALID_VAL);
    DEFAULT_VALUE(a->wrap, b->wrap, INVALID_VAL);
    DEFAULT_VALUE(a->rotation, b->rotation, INVALID_VAL);
    DEFAULT_VALUE(a->flip, b->flip, INVALID_VAL);
    //
    DEFAULT_VALUE(a->bx_width, b->bx_width, INVALID_VAL);
    DEFAULT_VALUE(a->bx_height, b->bx_height, INVALID_VAL);
    //
    DEFAULT_VALUE(a->scale_width, b->scale_width, INVALID_VAL);
    DEFAULT_VALUE(a->scale_height, b->scale_height, INVALID_VAL);
    //
    DEFAULT_VALUE(a->chr_dist, b->chr_dist, INVALID_VAL);
    DEFAULT_VALUE(a->chr_rotation, b->chr_rotation, INVALID_VAL);
    DEFAULT_VALUE(a->chr_fmt, b->chr_fmt, INVALID_VAL);
    DEFAULT_VALUE(a->chr_flip, b->chr_flip, INVALID_VAL);
    //
    DEFAULT_VALUE(a->x, b->x, INVALID_VAL);
    DEFAULT_VALUE(a->y, b->y, INVALID_VAL);
    //
    DEFAULT_VALUE(a->func_alterMargin, b->func_alterMargin, INVALID_PTR);
    DEFAULT_VALUE(a->func_alterBorder, b->func_alterBorder, INVALID_PTR);
    DEFAULT_VALUE(a->func_alterPadding, b->func_alterPadding, INVALID_PTR);
    DEFAULT_VALUE(a->func_alterText, b->func_alterText, INVALID_PTR);
}
