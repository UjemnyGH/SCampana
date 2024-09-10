#pragma once
#ifndef __CORE_
#define __CORE_

#define SP_CONFIG_FILE                          "scampanaSavedSettings"
#define SP_OPENGL_VERSION                       "#version 430"

#define SP_ADD_CONFIG_ID                        "Dodaj konfiguracje"
#define SP_LOAD_CONFIG_ID                        "Zaladuj konfiguracje"
#define SP_DELETE_CONFIG_ID                        "Usun konfiguracje"

#define SP_LESSON_LENGTH_VAR_NAME               "lessonLength"
#define SP_PAUSE_1_LENGTH_VAR_NAME              "pause1Length"
#define SP_PAUSE_2_LENGTH_VAR_NAME              "pause2Length"
#define SP_PAUSE_3_LENGTH_VAR_NAME              "pause3Length"
#define SP_FIRST_LESSON_START_HOUR_VAR_NAME     "firstLessonStartHour"
#define SP_FIRST_LESSON_START_MINUTE_VAR_NAME   "firstLessonStartMinute"

#define SP_LAST_CONFIG_SECTION                  "LastConfig"
#define SP_CURRENT_CONFIG_VAR                   "currentConfig"
#define SP_LESSONS_AMOUNT_VAR                   "lessonsAmount"

IniLoader gConfigFile;

#endif