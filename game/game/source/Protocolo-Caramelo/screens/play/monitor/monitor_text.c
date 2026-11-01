#include"monitor_text.h"

static int isSet(double value)
{
    return value != (double)INVALID_VAL;
}

// Copy of the base config scaled to the current monitor size
static TextConfig scaledConfig(const MonitorText *mt, double scale)
{
    TextConfig tc = mt->config;

    if(isSet(tc.scale_width)){
        tc.scale_width *= scale;
    }
    if(isSet(tc.scale_height)){
        tc.scale_height *= scale;
    }
    if(tc.wrap != (unsigned)INVALID_VAL && tc.wrap != 0){
        tc.wrap = (unsigned)(tc.wrap * scale);
    }
    return tc;
}

MonitorText* MonitorText_Create(TextCache *cache, const TextConfig *config, const char *str)
{
    MonitorText *mt;

    mt = malloc(sizeof(MonitorText));
    if(!mt){
        return NULL;
    }
    mt->config = *config;
    mt->applied_scale = -1; // nothing applied yet
    mt->text = Text_Create(cache, &mt->config, (char*)str);
    if(!mt->text){
        free(mt);
        return NULL;
    }
    return mt;
}

void MonitorText_Destroy(MonitorText **mt)
{
    if(!mt || !*mt){
        return;
    }
    Text_Destroy(&(*mt)->text);
    free(*mt);
    *mt = NULL;
}

void MonitorText_Measure(MonitorText *mt, double scale, int *w, int *h)
{
    TextConfig tc;
    int (*edges)[DRAW_EDGE_AMT];

    // Same measuring sequence used by the other screens (see room/monitor buttons).
    // The config is only re-applied when the scale changed: applying it twice before
    // a draw would make the Text forget that its textures must be regenerated.
    if(mt->applied_scale != scale){
        tc = scaledConfig(mt, scale);
        Text_UpdateConfig(mt->text, &tc);
        mt->applied_scale = scale;
    }
    Text_MeasureUTF8(mt->text, NULL, NULL);
    Text_MeasureMargin(mt->text, NULL, NULL);
    edges = Text_edgesBack(mt->text);

    if(w){
        *w = (*edges)[DRAW_EDGE_MARGIN_XMAX] - (*edges)[DRAW_EDGE_MARGIN_XMIN];
    }
    if(h){
        *h = (*edges)[DRAW_EDGE_MARGIN_YMAX] - (*edges)[DRAW_EDGE_MARGIN_YMIN];
    }
}

void MonitorText_Draw(MonitorText *mt, int x, int y, double scale)
{
    TextConfig tc;

    tc = scaledConfig(mt, scale);
    tc.x = x;
    tc.y = y;
    Text_DrawUTF8(mt->text, NULL, &tc, NULL);
    mt->applied_scale = scale;
}

void MonitorText_DrawCentered(MonitorText *mt, int cx, int y, double scale)
{
    int w;

    MonitorText_Measure(mt, scale, &w, NULL);
    MonitorText_Draw(mt, cx - w / 2, y, scale);
}
