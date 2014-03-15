#pragma once

#ifdef DEBUG
#define LOG_FILENAME "log.txt"
#endif

#ifdef RELEASE
#define LOG_FILENAME ""
#endif

#define SETTINGS_FILENAME "settings.xml"
