#include <pebble.h>

static Window *window;
static TextLayer *text_top;
static TextLayer *text_layer;

// static int value;
// static char s_buffer[8];

/*
static void main_window_unload(Window *window) 
{
  // Destroy TextLayer
  text_layer_destroy(text_top);
  text_layer_destroy(text_layer);
  text_layer_destroy(text_info);
}
*/

static void update_time() 
{
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer2[8];
  static char s_buffer3[20];
  strftime(s_buffer2, sizeof(s_buffer2), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  
  strftime(s_buffer3, sizeof(s_buffer3), "%a %D", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(text_top, s_buffer3);
  text_layer_set_text(text_layer, s_buffer2);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
  update_time();
}


/*
static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  text_layer_set_text(text_info, "menu");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  text_layer_set_text(text_info, "up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  text_layer_set_text(text_info, "dwn");
}


static void click_config_provider(void *context) 
{
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
*/

static void window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_top = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 40 } });
  text_layer_set_text_alignment(text_top, GTextAlignmentCenter);
  text_layer_set_font(text_top, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  
  text_layer = text_layer_create((GRect) { .origin = { 0, 40 }, .size = { bounds.size.w, 50 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));

  
  text_layer_set_text(text_top, "date");
  update_time();
  
  layer_add_child(window_layer, text_layer_get_layer(text_top));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  /*
  static void message_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received message.");
  Tuple *msg_type_tuple = dict_find(iter, PebbleMessageKeyType);
  Tuple *msg_value_tuple = dict_find(iter, PebbleMessageKeyValue);
  write_line_on_screen(msg_value_tuple->value->cstring);
}
...
// Set sniff interval.
app_comm_set_sniff_interval(SNIFF_INTERVAL_NORMAL); 

// Register message handlers
app_message_register_inbox_received(message_handler);
app_message_register_inbox_dropped(message_dropped);

// Init buffers
app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message set up.");
*/
}


static void window_unload(Window *window) 
{
  text_layer_destroy(text_layer);
}


static void init(void) 
{
  window = window_create();
//  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  window_stack_push(window, animated);
}

static void deinit(void) 
{
  window_destroy(window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}