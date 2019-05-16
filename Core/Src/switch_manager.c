/*******************************************************************************
* @file  switch_manager.c
* @brief Sage Glass Switch manager implementation
*
* This file provides functions to manage and configure Switch device
*
*******************************************************************************/

#include "switch_manager.h"
#include <string.h>

extern RTC_HandleTypeDef hrtc;
extern sleep_after_state_t sleepAfterState_g;
extern osMailQId uiMsgBox_g;

static struct
{
  uint8_t general;
  uint8_t tbd1;
  uint8_t tbd2;
} switch_errors_g;

static struct
{
	uint16_t sunriseMin;
	uint16_t sunsetMin;
} sunrise_sunset_state_g;

static zone_config_t   zoneConfigs_g[SW_MAX_ZONES_NUM];
static scene_config_t  sceneConfigs_g[SW_MAX_SCENES_NUM];
static system_config_t systemConfigs_g;
static uint8_t         activeScene;
static zone_status_t   zoneStatuses_g[SW_MAX_ZONES_NUM];

static int sendUIStateMsg(ui_state_t state)
{
  osStatus status = osOK;
  ui_state_t *mail = 0;

  mail = (ui_state_t *) osMailAlloc(uiMsgBox_g, 100);

  *mail = state;

  status = osMailPut(uiMsgBox_g, mail);

  return (int) status;
}

void StartUITask(void const *argument)
{
	ui_state_t state;
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	RTC_TimeTypeDef prevTime = {1};

	for (;;)
	{
		HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		if (prevTime.Hours != time.Hours || prevTime.Minutes != time.Minutes ||
				prevTime.Seconds != time.Seconds)
		{
			prevTime = time;
			state.dateTime.time = time;
			state.dateTime.date = date;
			state.msgType = DATE_TIME_CHANGED;
			sendUIStateMsg(state);
		}
		osDelay(10);
	}
}

void sleepAfterTimerHandler(void const *argument)
{
  sleepAfterState_g.screenState = 0;
  ui_state_t state;
  state.msgType = SLEEP_AFTER_TIMER;
  sendUIStateMsg(state);
  DBG_LOG("TIM1", "Sleep after timer expired");
  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_RESET);
}

RV_t SW_GetErrorStatus(uint8_t *genErr, uint8_t *tbd1Failed, uint8_t *tbd2Failed)
{
  if ((!genErr) || (!tbd1Failed) || (!tbd2Failed))
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  *genErr = switch_errors_g.general;
  *tbd1Failed = switch_errors_g.tbd1;
  *tbd2Failed = switch_errors_g.tbd2;

  return RV_SUCCESS;
}

RV_t SW_GetZoneNum(uint8_t *zones)
{
  if (!zones)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  for (uint i = 0; i < SW_MAX_ZONES_NUM; i++)
  {
    if ((zoneConfigs_g[i].uniqueID == 0) || (zoneConfigs_g[i].uniqueID == 0xFF))
    {
      *zones = i;
      break;
    }
  }

  return RV_SUCCESS;
}

RV_t SW_GetSceneNum(uint8_t *scenes)
{
  if (!scenes)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  for (uint i = 0; i < SW_MAX_SCENES_NUM; i++)
  {
    if ((sceneConfigs_g[i].uniqueID == 0) || (sceneConfigs_g[i].uniqueID == 0xFF))
    {
      *scenes = i;
      break;
    }
  }

  return RV_SUCCESS;
}

/* Note: Zone numeration in code starts from 0 */
RV_t SW_GetZoneConfig(zone_config_t *zone, uint8_t num)
{
  if (!zone)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  if (num >= SW_MAX_ZONES_NUM)
  {
    DBG_ERR(SWITCH_MGR, "Incorrect zone number");
    return RV_FAILURE;
  }

  memcpy((char *)zone, (char *)(&zoneConfigs_g[num]), sizeof(zone_config_t));

  return RV_SUCCESS;
}

/* Note: Zone numeration in code starts from 0 */
RV_t SW_SetZoneConfig(zone_config_t *zone, uint8_t num)
{
  if (!zone)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  if (num >= SW_MAX_ZONES_NUM)
  {
    DBG_ERR(SWITCH_MGR, "Incorrect zone number");
    return RV_FAILURE;
  }

  memcpy((char *)(&zoneConfigs_g[num]), (char *)zone, sizeof(zone_config_t));

  return RV_SUCCESS;
}

/* Note: Scene numeration in code starts from 0 */
RV_t SW_GetSceneConfig(scene_config_t *scene, uint8_t num)
{
  if (!scene)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  if (num >= SW_MAX_SCENES_NUM)
  {
    DBG_ERR(SWITCH_MGR, "Incorrect scene number");
    return RV_FAILURE;
  }

  memcpy((char *)scene, (char *)(&sceneConfigs_g[num]), sizeof(scene_config_t));

  return RV_SUCCESS;
}

/* Note: Zone numeration in code starts from 0 */
RV_t SW_SetSceneConfig(scene_config_t *scene, uint8_t num)
{
  if (!scene)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  if (num >= SW_MAX_SCENES_NUM)
  {
    DBG_ERR(SWITCH_MGR, "Incorrect scene number");
    return RV_FAILURE;
  }

  memcpy((char *)(&sceneConfigs_g[num]), (char *)scene, sizeof(scene_config_t));

  return RV_SUCCESS;
}

RV_t SW_GetSystemConfig(system_config_t *system)
{
  if (!system)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  memcpy((char *)system, (char *)(&systemConfigs_g), sizeof(system_config_t));

  return RV_SUCCESS;
}

RV_t SW_SetSystemConfig(system_config_t *system)
{
  if (!system)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  memcpy((char *)(&systemConfigs_g), (char *)system, sizeof(system_config_t));

  return RV_SUCCESS;
}

RV_t SW_SetSceneStatus(uint8_t status)
{
  activeScene = status;

  return RV_SUCCESS;
}

RV_t SW_SetZoneStatus(zone_status_t *status, uint8_t num)
{
  if (!status)
  {
    DBG_ERR(SWITCH_MGR, "Null pointer received");
    return RV_NULLPTR;
  }

  if (num >= SW_MAX_ZONES_NUM)
  {
    DBG_ERR(SWITCH_MGR, "Incorrect zone number");
    return RV_FAILURE;
  }

  memcpy((char *)(&zoneStatuses_g[num]), (char *)status, sizeof(zone_status_t));

  return RV_SUCCESS;
}

RV_t SW_ClockConfig(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day, uint8_t month, uint8_t year,
		uint8_t dayOfTheWeek, uint8_t sunriseHour, uint8_t sunriseMin, uint8_t sunsetHour, uint8_t sunsetMin)
{
	/* TODO: Implement RTC */
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  time.Hours = hour;
  time.Minutes = minute;
  time.Seconds = second;
  time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  time.StoreOperation = RTC_STOREOPERATION_RESET;
  time.TimeFormat = RTC_HOURFORMAT_24;

  date.Year = year;
  date.Month = month;
  date.Date = day;
  date.WeekDay = dayOfTheWeek;

  if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN))
  {
	  DBG_ERR(SWITCH_MGR, "Failed to set RTC time");
	  return RV_FAILURE;
  }
  if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN))
  {
	  DBG_ERR(SWITCH_MGR, "Failed to set RTC date");
	  return RV_FAILURE;
  }


  DBG_LOG(SWITCH_MGR, "New Date - Year %d, Month %d, Date %d, Week Day %d\r\nNew Time - Hours %d, Minutes %d, Seconds %d\r\nSunrise Hour %d, Sunrise Minute %d, Sunset Hour %d, Sunset minute %d",
		  date.Year, date.Month, date.Date, date.WeekDay, time.Hours, time.Minutes, time.Seconds, sunriseHour, sunriseMin, sunsetHour, sunsetMin);

  sunrise_sunset_state_g.sunriseMin = sunriseHour * 60 + sunriseMin;
  sunrise_sunset_state_g.sunsetMin = sunsetHour * 60 + sunsetMin;
  return RV_SUCCESS;
}

RV_t SW_GetSunriseSunsetMinutes(uint16_t *sunriseMin, uint16_t *sunsetMin)
{
	if (sunrise_sunset_state_g.sunriseMin == 0 && sunrise_sunset_state_g.sunsetMin == 0)
	{
		return RV_FAILURE;
	}

	*sunriseMin = sunrise_sunset_state_g.sunriseMin;
	*sunsetMin = sunrise_sunset_state_g.sunsetMin;
	return RV_SUCCESS;
}
