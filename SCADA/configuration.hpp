#pragma once

#include "syslog.hpp"

#ifdef _DEBUG
static WarGrey::SCADA::Log default_logging_level = WarGrey::SCADA::Log::Debug;
#else
static WarGrey::SCADA::Log default_logging_level = WarGrey::SCADA::Log::Info;
#endif

// static Platform::String^ remote_test_server = "172.20.10.2";
static Platform::String^ remote_test_server = "192.168.1.255";

/*************************************************************************************************/
static const unsigned int frame_per_second = 4;
static const unsigned int screen_navigator_foreground = 0xFEFEFE;
static const unsigned int screen_status_label_color = 0xB0B0B0;
static const unsigned int screen_status_parameter_color = 0xFEFEFE;

static const float screen_width = 1920.0F;
static const float screen_height = 1080.0F;

static const float sketch_width = 1920.0F;
static const float sketch_height = 1200.0F;
static const float sketch_navigator_height = 90.0F;
static const float sketch_statusbar_height = 215.0F;

static const float sketch_copyright_xoff = 288.0F;
static const float sketch_copyright_yoff = 24.0F;

static const float sketch_status_label_xoff = 148.0F;
static const float sketch_status_label_yoff = 24.0F;
static const float sketch_status_parameter_yoff = 120.0F;
static const float sketch_status_alarm_x = 1206.0F;
static const float sketch_status_alarm_width = 160.0F;

static const float indicator_thickness = 16.0F;
