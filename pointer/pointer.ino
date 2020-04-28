#include <Servo.h>
#include <LiquidCrystal.h>

#include "limits.h"
#include "debounce.h"
#include "ui.h"
#include "types.h"
#include "dataset.h"

// Wiring pins
const int pinLeft = A2;
const int pinRight = A1;
const int pinSelect = A0;

const int pinServoAzimuth = 4;
const int pinServoAltitude = 5;

const int pinLCDrs = A5;
const int pinLCDen = 2;
const int pinLCDd4 = 6;
const int pinLCDd5 = 7;
const int pinLCDd6 = 8;
const int pinLCDd7 = 3;

LiquidCrystal lcd(pinLCDrs, pinLCDen, pinLCDd4, pinLCDd5, pinLCDd6, pinLCDd7);

Debounce keyLeft(pinLeft);
Debounce keyRight(pinRight);
Debounce keySelect(pinSelect);

Servo servoAzimuth;
Servo servoAltitude;

TwoRowUI ui(keyLeft, keyRight, keySelect);

const char* calibrateOptions[4] = {
    "Set to 0        ",
    "Set to 90       ",
    "Set to 180      ",
    "Back            "
};

void setup()
{
    servoAzimuth.attach(pinServoAzimuth);
    servoAltitude.attach(pinServoAltitude);

    lcd.begin(16, 2);
    ui.setup();

    draw();
}

struct SkyPosition getSkyPosition(int planetIndex, int timeIndex)
{
    SkyPosition p;

    p.azimuth = dataSet.planets[planetIndex].position[timeIndex].azimuth;
    p.altitude = dataSet.planets[planetIndex].position[timeIndex].altitude;

    return p;
}

void draw()
{

    lcd.setCursor(0, 0);
    lcd.print(ui.row == 0 ? "> " : "  ");
    lcd.print(ui.sectionIndex);
    lcd.print(" : ");
    lcd.print(uiGetSectionFromIndex(ui.sectionIndex));
    lcd.print(" ");

    lcd.setCursor(0, 1);
    lcd.print(ui.row == 1 ? "> " : "  ");
    lcd.print(uiGetOptionFromIndex(ui.sectionIndex, ui.optionIndex));
}

void loop()
{
    if (ui.update()) {
        draw();
    }
}

void setPointer(int azimuth, int altitude)
{
    servoAzimuth.write(azimuth);
    servoAltitude.write(altitude);
}

/*
 * UI Action handler
 * 
 */
void uiInvokeOption(int section, int option)
{
    // The first option is the calibration...
    if (section == 0) {
        if (option == 0) {
            setPointer(0, 0);
        }
        else if (option == 1) {
            setPointer(90, 90);
        }
        else if (option == 2) {
            setPointer(180, 180);
        }
        return;
    }

    // The rest are planets
    SkyPosition pos = getSkyPosition(section - 1, option);
    if (pos.azimuth > 180) {
        pos.azimuth -= 180;
    }

    setPointer(pos.azimuth, pos.altitude);
}

/*
 * UI Display handlers
 * 
 */
int uiGetSectionCount()
{
    return 1 /* calibrate */ + sizeof(dataSet.planets) / sizeof(dataSet.planets[0]);
}

const char* uiGetSectionFromIndex(int sectionIndex)
{
    if (sectionIndex == 0) {
        return "Calibrate ";
    }
    return dataSet.planets[sectionIndex - 1].name;
}

char* angleCat(char* buffer, int angle)
{
    char* ptr = strchr(buffer, '\0');

    *ptr++ = '0' + (angle / 100);
    angle -= (angle / 100) * 100;

    *ptr++ = '0' + (angle / 10);
    angle -= (angle / 10) * 10;

    *ptr++ = '0' + angle;
    *ptr++ = '\0';

    return ptr;
}

int uiGetOptionCount(int sectionIndex)
{
    if (sectionIndex == 0) {
        return sizeof(calibrateOptions) / sizeof(calibrateOptions[0]);
    }
    return MAX_TIME_SLOTS;
}

const char* uiGetOptionFromIndex(int sectionIndex, int optionIndex)
{
    static char buffer[20];

    if (sectionIndex == 0) { // calibrate
        return calibrateOptions[optionIndex];
    }
    else { // planet
        SkyPosition pos = getSkyPosition(sectionIndex - 1, optionIndex);
        strcpy(buffer, dataSet.timeSlots[optionIndex].name);
        strcat(buffer, " ");

        angleCat(buffer, pos.azimuth);
        if (pos.azimuth > 180) {
            strcat(buffer, "S");
        }
        else {
            strcat(buffer, "N");
        }

        strcat(buffer, " ");
        if (pos.altitude < 0) {
            strcat(buffer, "sub ");
        }
        else {
            angleCat(buffer, pos.altitude);
            strcat(buffer, " ");
        }

        return buffer;
    }
}
