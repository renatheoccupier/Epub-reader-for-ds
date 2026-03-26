#include "default.h"
#include "settings.h"

namespace
{

PmEventCookie gPowerCookie;
bool gPowerInitialized = false;
bool gAppShouldExit = false;
backlightMode gBacklightMode = blOverlay;

void applyCurrentBacklights()
{
	powerOff((PM_Bits)PM_BACKLIGHTS);

	if(gBacklightMode == blBoth || settings::scrConf == scBoth) {
		powerOn(PM_BACKLIGHTS);
		return;
	}

	if(gBacklightMode == blReading && settings::scrConf == scTop) {
		powerOn(PM_BACKLIGHT_TOP);
		return;
	}

	powerOn(PM_BACKLIGHT_BOTTOM);
}

void handlePowerEvent(void* user, PmEvent event)
{
	(void)user;

	if(event == PmEvent_OnSleep) {
		powerOff(PM_BACKLIGHTS);
		return;
	}

	if(event == PmEvent_OnWakeup) {
		applyBrightness();
		applyCurrentBacklights();
	}
}

}

void initPowerManagement()
{
	if(gPowerInitialized) return;

	pmAddEventHandler(&gPowerCookie, handlePowerEvent, NULL);
	gPowerInitialized = true;
}

bool pumpPowerManagement()
{
	initPowerManagement();
	if(gAppShouldExit) return false;
	if(!pmMainLoop()) {
		gAppShouldExit = true;
		return false;
	}
	return true;
}

bool appShouldExit()
{
	initPowerManagement();
	if(pmShouldReset()) gAppShouldExit = true;
	return gAppShouldExit;
}

void setBacklightMode(backlightMode mode)
{
	initPowerManagement();
	gBacklightMode = mode;
	applyCurrentBacklights();
}
