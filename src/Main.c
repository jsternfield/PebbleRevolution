#include "main.h"

static void timer_callback(void *data)
{
	update();
	layer_mark_dirty(canvas);
	timer = app_timer_register(mFramerate, (AppTimerCallback) timer_callback, 0);
}

static void start()
{
	timer = app_timer_register(mFramerate, (AppTimerCallback) timer_callback, 0);
}

static void render(Layer *layer, GContext* ctx) 
{
    // Draw hour
    graphics_context_set_text_color(ctx, mForeColor);
    graphics_draw_text(ctx, mTextHour, mFontTime, GRect(0, 0, SCREEN_WIDTH, TIME_HEIGHT), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

    // Draw minute
    graphics_context_set_text_color(ctx, mForeColor);
    graphics_draw_text(ctx, mTextMinute, mFontTime, GRect(0, TIME_FONT_SIZE, SCREEN_WIDTH, TIME_HEIGHT), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

    // Draw day, month and day
    graphics_context_set_text_color(ctx, mForeColor);
    graphics_draw_text(ctx, mTextDate, mFontDate, GRect(DATE_X, DATE_Y, SCREEN_WIDTH, DATE_HEIGHT), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void set_time_display(struct tm *t)
{
    if (clock_is_24h_style())
    {
        strftime(mTextHour, size, "%H", t);
    }
    else
    {
        strftime(mTextHour size, "%I", t);
    }
    strftime(mTextMinute, size, "%M", t);

    strftime(mTextDate, size, "%a %m/%d", t);
}

static void window_load(Window *window)
{
    // Check for saved settings
    mInvertColors = persist_exists(KEY_INVERT) ? persist_read_bool(KEY_INVERT) : false;
    mBackColor = (mInvertColors) ? GColorWhite : GColorBlack;
    mForeColor = (mInvertColors) ? GColorBlack : GColorWhite;

    window_set_background_color(window, mBackColor);
    canvas = layer_create(GRect(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT));
    layer_set_update_proc(canvas, (LayerUpdateProc) render);
    layer_add_child(window_get_root_layer(window), canvas);
    
    mFontTime = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TIMEFONT_48));
    mFontDate = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DATEFONT_24));
	
    // Set initial time so display isn't blank
    mTextHour = NULL
    mTextHour = malloc(mTextSizeHourMin);
    mTextMinute = NULL;
    mTextMinute = malloc(mTextSizeHourMin);
    mTextDate = NULL);
    mTextDate = malloc(mTextSizeDate);

    struct tm *t;
    time_t temp;
    temp = time(NULL);
    t = localtime(&temp);
    set_time_display(t);

    start();
}

static void window_unload(Window *window) 
{
    app_timer_cancel(timer);
    fonts_unload_custom_font(mFontTime);
    fonts_unload_custom_font(mFontTime);
    layer_destroy(canvas);
}

static void tick_handler(struct tm *t, TimeUnits units_changed)
{
	set_time_display(t);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
    Tuple* t = dict_read_first(iterator);
    while (t != NULL)
    {
        switch(t->key)
        {
        case KEY_INVERT:
            if (strcmp(t->value->cstring, "off") == 0)
            {
                mInvertColors = false;
            }
            if (strcmp(t->value->cstring, "on") == 0)
            {
                mInvertColors = true;
            }
            persist_write_bool(KEY_INVERT, mInvertColors);

        
        break;
        }
        
        t = dict_read_next(iterator);
    }
}

static void init(void)
{
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) 
    {
		.load = window_load,
		.unload = window_unload,
    });
    
    // Open AppMessage to enable communication with the phone
    app_message_register_inbox_received((AppMessageInboxReceived) inbox_received_callback);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	//Prepare stars memory
	prepare_stars();
	
	//Tick tock
	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
	
	//Finally
	window_stack_push(window, true);
}

static void deinit(void) 
{
	destroy_stars();
	tick_timer_service_unsubscribe();
	window_destroy(window);
}

int main(void) 
{
	init();
	app_event_loop();
	deinit();
}