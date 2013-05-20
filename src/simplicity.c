#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID {0xA4, 0x2B, 0xB8, 0xE2, 0xD2, 0x62, 0x4A, 0xA6, 0xAA, 0x30, 0xED, 0xBE, 0x01, 0xE3, 0x8A, 0x02}
PBL_APP_INFO(MY_UUID, "Chiara", "13 V", 3, 0 /* App version */, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

Window 		window;
TextLayer 	text_date_layer;
Layer 		line_layer;

#define TOTAL_IMAGE_SLOTS 4
#define NUMBER_OF_IMAGES 10

const int IMAGE_RESOURCE_IDS[NUMBER_OF_IMAGES] = {
	RESOURCE_ID_IMAGE_NUM_0, RESOURCE_ID_IMAGE_NUM_1, RESOURCE_ID_IMAGE_NUM_2,
	RESOURCE_ID_IMAGE_NUM_3, RESOURCE_ID_IMAGE_NUM_4, RESOURCE_ID_IMAGE_NUM_5,
	RESOURCE_ID_IMAGE_NUM_6, RESOURCE_ID_IMAGE_NUM_7, RESOURCE_ID_IMAGE_NUM_8,
	RESOURCE_ID_IMAGE_NUM_9
};

BmpContainer image_containers[TOTAL_IMAGE_SLOTS];
#define EMPTY_SLOT -1

int image_slot_state[TOTAL_IMAGE_SLOTS] = {EMPTY_SLOT, EMPTY_SLOT, EMPTY_SLOT, EMPTY_SLOT};


Layer line_layer;

void line_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;

  graphics_context_set_stroke_color(ctx, GColorWhite);

// Line above clock
  graphics_draw_line(ctx, GPoint(15, 45), GPoint(130, 45));
  graphics_draw_line(ctx, GPoint(15, 44), GPoint(130, 44));

// Line below clock
  graphics_draw_line(ctx, GPoint(15, 121), GPoint(130, 121));
  graphics_draw_line(ctx, GPoint(15, 120), GPoint(130, 120));

}


void load_digit_image_into_slot(int slot_number, int digit_value) {
	// TODO: Signal these error(s)?

	if ((slot_number < 0) || (slot_number >= TOTAL_IMAGE_SLOTS)) {
		return;
	}

	if ((digit_value < 0) || (digit_value > 9)) {
		return;
	}

	if (image_slot_state[slot_number] != EMPTY_SLOT) {
		return;
	}

	image_slot_state[slot_number] = digit_value;
	bmp_init_container(IMAGE_RESOURCE_IDS[digit_value], &image_containers[slot_number]);
	image_containers[slot_number].layer.layer.frame.origin.x = slot_number * 30 + 12;
	image_containers[slot_number].layer.layer.frame.origin.y = 48;
	layer_add_child(&window.layer, &image_containers[slot_number].layer.layer);

}

void unload_digit_image_from_slot(int slot_number) {
	if (image_slot_state[slot_number] != EMPTY_SLOT) {
		layer_remove_from_parent(&image_containers[slot_number].layer.layer);
		bmp_deinit_container(&image_containers[slot_number]);
		image_slot_state[slot_number] = EMPTY_SLOT;
	}
}

void display_time(PblTm *tick_time) {
		int hours = tick_time->tm_hour;
	int minutes = tick_time->tm_min;

	int value = ( hours * 100 + minutes) % 10000;

	for (int column_number = 3; column_number >= 0; column_number--) {
		int slot_number = column_number;
		unload_digit_image_from_slot(slot_number);
		if (!((value == 0) && (column_number == 0) && !true)) {
			load_digit_image_into_slot(slot_number, value % 10);
		}
		value = value / 10;
	}

	// Need to be static because they're used by the system later.
	static char time_text[] = "00:00";
	static char date_text[] = "00-00 ";

	char *time_format;

	// TODO: Only update the date when it's changed.
	string_format_time(date_text, sizeof(date_text), "%e-%m ", tick_time);
	text_layer_set_text(&text_date_layer, date_text);
}

void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&window, "Chiara");
	window_stack_push(&window, true /* Animated */);
	window_set_background_color(&window, GColorBlack);

	resource_init_current_app(&APP_RESOURCES);


	text_layer_init(&text_date_layer, window.layer.frame);
	text_layer_set_text_alignment(&text_date_layer, GTextAlignmentRight);
	text_layer_set_text_color(&text_date_layer, GColorWhite);
	text_layer_set_background_color(&text_date_layer, GColorClear);
	layer_set_frame(&text_date_layer.layer, GRect(0, 168-15, 144, 15));
	text_layer_set_font(&text_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(&window.layer, &text_date_layer.layer);


	// Initial time update:
	PblTm tick_time;
  get_time(&tick_time);
  display_time(&tick_time);

  layer_init(&line_layer, window.layer.frame);
  line_layer.update_proc = &line_layer_update_callback;
  layer_add_child(&window.layer, &line_layer);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

	(void)ctx;

	display_time(t->tick_time);

}




void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,

		.tick_info = {
			.tick_handler = &handle_minute_tick,
			.tick_units = MINUTE_UNIT
		}

	};
	app_event_loop(params, &handlers);
}

// Deconstruction
void handle_deinit(AppContextRef ctx) {
	(void)ctx;

	for (int i = 0; i < TOTAL_IMAGE_SLOTS; i++) {
		unload_digit_image_from_slot(i);
	}

}
