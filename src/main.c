#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


	//INFO ABOUT APP
#define MY_UUID { 0xD7, 0x0F, 0x42, 0xAC, 0xD2, 0x96, 0x46, 0x9F, 0xAF, 0x8A, 0xF0, 0x8E, 0x6A, 0xB0, 0x7E, 0x55 }
PBL_APP_INFO(MY_UUID,
             "Day Widget", "Richard Gilbert",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

//WINDOW
Window window;
//LAYER
Layer layer;
//BITMAP
BmpContainer digit1;
BmpContainer digit2;
//TIME
PblTm currTime;
//IMAGES
const int images[10] = {RESOURCE_ID_IMAGE_0, RESOURCE_ID_IMAGE_1, RESOURCE_ID_IMAGE_2, RESOURCE_ID_IMAGE_3,
					    RESOURCE_ID_IMAGE_4, RESOURCE_ID_IMAGE_5, RESOURCE_ID_IMAGE_6, RESOURCE_ID_IMAGE_7,
					    RESOURCE_ID_IMAGE_8, RESOURCE_ID_IMAGE_9};

//WHAT TO DO ON DRAW OF LAYER
void update_layer_callback(Layer *me, GContext* ctx) {
	
	//???
  (void)me;

	//set colour of text
  graphics_context_set_text_color(ctx, GColorWhite);
	
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, GRect(0,0,144,55), 0, GCornerNone);
	
	//get current time
	
	
	
	//get string
	char text[10];
	string_format_time(text, sizeof (char)*10, "%A", &currTime);

	///in the supplied context, draw this text, with this font, in this rectangle, word wrap, alignment, layout
  graphics_text_draw(ctx,
		     text,
		     fonts_get_system_font(FONT_KEY_GOTHIC_18),
		     GRect(5, 5, 144, 100),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
	
	string_format_time(text, sizeof (char)*10, "%B", &currTime);

  graphics_text_draw(ctx,
		     text,
		     fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
		     GRect(5, 20, 144, 60),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
	
}


//WHAT TO DO ON INIT
void handle_init(AppContextRef ctx) {
  (void)ctx;

	//window name, and push it
  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
	
	get_time(&currTime);
	
	//INIT RESOURCES FILE
	resource_init_current_app(&APP_RESOURCES);
	
	//INIT digit2 WITH RESOURCE ID
	bmp_init_container(images[currTime.tm_mday%10], &digit2);
	
	if(currTime.tm_mday/10 > 0){ //only display > 09 (two digits)
		//INIT digit1
		bmp_init_container(images[currTime.tm_mday/10], &digit1);
		//position digit 1
		digit1.layer.layer.frame.origin.x = 22;
		digit1.layer.layer.frame.origin.y = 72;
		//position digit 2
		digit2.layer.layer.frame.origin.x = 72;
		digit2.layer.layer.frame.origin.y = 72;
		
		//add digit1 to layer
		layer_add_child(&window.layer, &digit1.layer.layer);
	}else{ //one digit
		//position digit 2
		digit2.layer.layer.frame.origin.x = 47;
		digit2.layer.layer.frame.origin.y = 72;
	}
	//add digit2 to layer
	layer_add_child(&window.layer, &digit2.layer.layer);
	
	
	
	//initialise layer as global "layer" and set it's update procedure
	layer_init(&layer, window.layer.frame);
	layer.update_proc = update_layer_callback;
	layer_add_child(&window.layer, &layer);

	//who knows
  layer_mark_dirty(&layer);
}

void handle_deinit(AppContextRef ctx) {
	(void)ctx;

	if(currTime.tm_mday/10 > 0){ //only deinit digit1 if it's used
		bmp_deinit_container(&digit1);
	}
	bmp_deinit_container(&digit2);
}


//main duh
void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	.deinit_handler = &handle_deinit
  };
  app_event_loop(params, &handlers);
}
