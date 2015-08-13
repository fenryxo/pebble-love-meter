#include <pebble.h>

static Window* window;
static TextLayer* text_layer = NULL;
static Layer* progress_bar = NULL;
static AppTimer* timer = NULL;
static double progress = 0;
static char progress_text[100];
static char name[100];
static int result = 0;
static int duration = 0;
static int step_size = 100;
static int mode = 0;

static void start_up(void);

static void progress_bar_create(void);
static void progress_bar_update(void);
static void progress_bar_update_cb(Layer* layer, GContext* ctx);
static void progress_bar_destroy(void);

static void measurement_text_create(void);
static void measurement_text_update(void);
static void measurement_text_destroy(void);

static void measurement_start(void);
static void measurement_step(void);
static void measurement_destroy(void);
static void measurement_done(void);

static void results_create(void);
static void results_destroy(void);

static void show_text(const char* text);
static void destroy_text(void);


static void select_click_handler(ClickRecognizerRef recognizer, void* context)
{
    switch (mode)
    {
    case 0:
        destroy_text();
        mode = 1;
        measurement_start();
        break;
    case 1:
        measurement_step();
        break;
    case 2:
        destroy_text();
        results_create();
        break;
    case 3:
        results_destroy();
        break;
    case 4:
        start_up();
        break;
    }
}

static void click_config_provider(void* context)
{
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window* window)
{
    result = 16;
    duration = 10;
    snprintf(name, sizeof(name), "%s", "Alena");
    start_up();
}

static void start_up(void)
{
    mode = 0;
    show_text("Press a button.");
}

static void progress_bar_create(void)
{
    Layer* window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
	progress_bar = layer_create(GRect(10, 90, bounds.size.w - 20, 10));
    layer_set_update_proc(progress_bar, progress_bar_update_cb);
	layer_add_child(window_layer, progress_bar);
}

static void progress_bar_update(void)
{
    layer_mark_dirty(progress_bar);
}

static void progress_bar_update_cb(Layer* layer, GContext* ctx)
{
    GRect bounds = layer_get_bounds(layer);
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
    
    if (progress < 100)
    {
        int black_w = (int) ((bounds.size.w - 2) * progress / 100);
        int white_w = bounds.size.w - 2 - black_w;
        GRect progress_bounds = GRect(
            bounds.origin.x + 1 + black_w,
            bounds.origin.y + 1,
            white_w,
            bounds.size.h - 2);
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_rect(ctx, progress_bounds, 0, GCornerNone);
    
    }
}

static void progress_bar_destroy(void)
{
    if (progress_bar)
    {
        layer_destroy(progress_bar);  
        progress_bar = NULL;
    }
}

static void measurement_text_create(void)
{
    Layer* window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create(GRect(10, 110, bounds.size.w - 20, 40));
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(text_layer, GTextOverflowModeTrailingEllipsis);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
    measurement_text_update();
}

static void measurement_text_update(void)
{
    snprintf(progress_text, sizeof(progress_text), "Measurement in progress (%d%%)", (int)progress); 
    text_layer_set_text(text_layer, progress_text);
}

static void measurement_text_destroy(void)
{
    if (text_layer)
    {
        text_layer_destroy(text_layer);
        text_layer = NULL;
    }
}

static void measurement_timer_cb(void* data)
{
    measurement_step();
}
    
static void measurement_start(void)
{
    progress = 0;
    measurement_text_create();
    progress_bar_create();
    
    timer = app_timer_register(step_size, measurement_timer_cb, NULL);
}

static void measurement_destroy(void)
{
    measurement_text_destroy();
    progress_bar_destroy();
}

static void measurement_step(void)
{
    if (progress >= 100)
    {
        measurement_done();
    }
    else
    {
        progress += 100.0 / (duration * 1000 / step_size);
        measurement_text_update();
        progress_bar_update();
        timer = app_timer_register(step_size, measurement_timer_cb, NULL);
    }
}

static void measurement_done(void)
{
    measurement_destroy();
    mode = 2;
    show_text("Measurement done.\nPress a button.");
}

static void results_create(void)
{
    mode = 3;
    progress = result;
    progress_bar_create();
    snprintf(progress_text, sizeof(progress_text), "%s\n%d%%", name, result);
    show_text(progress_text);
}

static void results_destroy(void)
{
    progress_bar_destroy();
    destroy_text();
    mode = 4;
}


static void show_text(const char* text)
{
    Layer* window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    text_layer = text_layer_create(GRect(10, 110, bounds.size.w - 20, 40));
    text_layer_set_text(text_layer, text);
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(text_layer, GTextOverflowModeTrailingEllipsis);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void destroy_text(void)
{
    if (text_layer)
    {
        text_layer_destroy(text_layer);
        text_layer = NULL;
    }
}

static void window_unload(Window *window)
{
    measurement_destroy();
    results_destroy();
    destroy_text();
}

static void init(void)
{
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void)
{
  window_destroy(window);
}

int main(void)
{
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();
}
