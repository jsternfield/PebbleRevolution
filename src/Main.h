#pragma once

#include <pebble.h>


// SETTINGS
#define KEY_INVERT 0

// CONSTS
#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168
#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)

#define FONT_SIZE_TIME (SCREEN_CENTER_X)
#define FONT_SIZE_DATE (SCREEN_HEIGHT - (FONT_SIZE_TIME * 2))
#define TIME_Y 0
#define DATE_X 0
#define DATE_Y (SCREEN_HEIGHT - FONT_SIZE_DATE)

static Window* window;
static Layer* canvas;
static AppTimer* timer;

static GFont mFontTime;
static GFont mFontDate;
static char* mTextTime;
static char* mTextDate;

GColor mBackgroundColor;
GColor mTextColor;

int mTextSizeDate = sizeof("DAY MM/DD");
int mTextSizeHourMin = sizeof("XX");
int mFramerate;
bool mInvertColors;