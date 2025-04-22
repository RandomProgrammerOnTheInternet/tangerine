#ifndef TANGERINE_H
#define TANGERINE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <termios.h>

typedef struct tge_color_t {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} tge_color_t;
	
typedef struct tge_object_t {
	char sprite;
	int x;
	int y;
	tge_color_t fg_color;
	tge_color_t bg_color;
} tge_object_t;

typedef struct tge_group_t {
	unsigned int height;
	unsigned int width;
	tge_object_t *objects;
} tge_group_t;

void tge_clear_screen();
inline tge_color_t tge_color(int r, int g, int b);
void tge_start_fg_color(tge_color_t color);
void tge_start_bg_color(tge_color_t color);
void tge_move_cursor(int x, int y);
void tge_print_object(tge_object_t object);
void tge_print_group(tge_group_t group);
void tge_enter_raw_mode();
void tge_exit_raw_mode();

#endif // TANGERINE_H

#ifdef TANGERINE_IMPL

static struct termios original_termios_settings;

void tge_clear_screen() {
	printf("\e[1;1H\e[2J");
}

tge_color_t tge_color(int r, int g, int b) {
	return (tge_color_t) { .r = r, .g = g, .b = b};
}

void tge_start_fg_color(tge_color_t color) {
	printf("\x1b[48;2;%" PRId8 ";", color.r);
    printf("%" PRId8 ";", color.g);
    printf("%" PRId8 "m", color.b);
}

void tge_start_bg_color(tge_color_t color) {
	printf("\x1b[38;2;%" PRId8 ";", color.r);
    printf("%" PRId8 ";", color.g);
    printf("%" PRId8 "m", color.b);
}

void tge_move_cursor(int x, int y) {
	printf("\x1b[%d;", x);
	printf("%dH", y);
}

void tge_print_object(tge_object_t object) {
	tge_start_fg_color(object.fg_color);
	tge_start_bg_color(object.bg_color);
	tge_move_cursor(object.x, object.y);
	putc(object.sprite, stdout);
}

void tge_print_group(tge_group_t group) {
	for(unsigned int i = 0; i < group.width * group.height; i++) {
		if(i % group.width == 0 && i) {
			putc('\n', stdout);
		}
		tge_print_object(group.objects[i]);
	}
}

void tge_enter_raw_mode() {
	tcgetattr(0, &original_termios_settings);
	atexit(tge_exit_raw_mode);
	struct termios raw = original_termios_settings;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	// raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(0, TCSAFLUSH, &raw);
}

void tge_exit_raw_mode() {
	tcsetattr(0, TCSAFLUSH, &original_termios_settings);
}

#endif // TANGERINE_IMPL
