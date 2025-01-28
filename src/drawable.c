#include <openwm/drawable.h>
#include <openwm/context.h>
#include <string.h>

openwm_drawable* openwm_create_drawable(struct openwm_context* ctx, openwm_point2i pos, openwm_point2i size, openwm_drawable_draw_t draw, void* custom_data)
{
    if (ctx == NULL)
        return NULL;

    openwm_drawable* drawable = ctx->allocate(sizeof(openwm_drawable));
    if (drawable == NULL)
        return NULL;

    *drawable = (openwm_drawable){
        .pos = pos,
        .size = size,
        .draw = draw,
        .enabled = 1,
        .custom_data = custom_data,
        .children_start = NULL,
        .children_end = NULL,
        .on_event_handle = NULL,
        .on_mouse_scroll = NULL,
        .on_mouse_button = NULL,
        .on_mouse_move = NULL,
        .on_key_press = NULL,
        .prev = NULL,
        .next = NULL,
    };

    return drawable;
}

void openwm_drawable_add_child(openwm_drawable* drawable, openwm_drawable* child)
{
    if (drawable == NULL || child == NULL)
        return;

    if (drawable->children_start == NULL)
    {
        drawable->children_start = drawable->children_end = child;
        return;
    }

    drawable->children_end->next = child;
    child->prev = drawable->children_end;
    drawable->children_end = child;
}

void openwm_dispose_drawable(struct openwm_context* ctx, openwm_drawable* drawable)
{
    if (drawable == NULL)
        return;

    if (drawable->children_start != NULL)
    {
        for (openwm_drawable* child = drawable->children_start; child != NULL; child = child->next)
            openwm_dispose_drawable(ctx, child);
    }

    if (drawable->custom_data != NULL)
        ctx->deallocate(drawable->custom_data);

    ctx->deallocate(drawable);
}
