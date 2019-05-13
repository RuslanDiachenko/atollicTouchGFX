/*******************************************************************************
* @file  switch_manager.h
* @brief Sage Glass switch manager header file
*
* This file provides firmware functions prototypes 
* to manage switch UI and configuration
*
*******************************************************************************/
#ifndef __SWITCH_MANAGER_H
#define __SWITCH_MANAGER_H

#include "main.h"

#define SW_MAX_NUMBER_OF_LEVELS 8
#define SW_MAX_NAME_LEN         24
#define SW_MAX_ZONES_NUM        32 /* TODO: Adjust later */
#define SW_MAX_SCENES_NUM       32

typedef struct
{
  char displayedName[SW_MAX_NAME_LEN];
  uint8_t type;
  uint8_t uniqueID;
  uint8_t numberOfLevels;
  uint8_t level[SW_MAX_NUMBER_OF_LEVELS];
} zone_config_t;

typedef struct
{
  char displayedName[SW_MAX_NAME_LEN];
  uint8_t uniqueID;
} scene_config_t;

typedef struct
{
  char backgroundImage[SW_MAX_NAME_LEN];
  uint8_t panelBrightness;
  uint8_t sleepAfter;
  uint8_t screenWaitDelay;
} system_config_t;

typedef struct
{
  uint8_t uniqueID;
  uint8_t level;
  uint8_t state;
  uint8_t mode;
} zone_status_t;

RV_t SW_GetErrorStatus(uint8_t *genErr, uint8_t *tbd1Failed, uint8_t *tbd2Failed);

RV_t SW_GetZoneNum(uint8_t *zones);
RV_t SW_GetZoneConfig(zone_config_t *zone, uint8_t num);
RV_t SW_SetZoneConfig(zone_config_t *zone, uint8_t num);

RV_t SW_GetSceneNum(uint8_t *scenes);
RV_t SW_GetSceneConfig(scene_config_t *scene, uint8_t num);
RV_t SW_SetSceneConfig(scene_config_t *scene, uint8_t num);

RV_t SW_GetSystemConfig(system_config_t *system);
RV_t SW_SetSystemConfig(system_config_t *system);

RV_t SW_SetSceneStatus(uint8_t status);
RV_t SW_SetZoneStatus(zone_status_t *status, uint8_t num);

RV_t SW_ClockConfig(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day, uint8_t month, uint8_t year, 
		uint8_t dayOfTheWeek, uint8_t sunriseHour, uint8_t sunriseMin, uint8_t sunsetHour, uint8_t sunsetMin);

#endif /*__SWITCH_MANAGER_H */
