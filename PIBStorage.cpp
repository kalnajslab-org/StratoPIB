/*
 *  PIBStorage.cpp
 *  Author:  Alex St. Clair
 *  Created: August 2019
 *
 *  This class manages configuration storage in EEPROM
 */

#include "PIBStorage.h"

PIBConfigs_t pib_config = {0}; // software copy of EEPROM

bool PIBStorage::LoadFromEEPROM(void)
{
    // load stored data
    EEPROM.get(EEPROM_BASE_ADDRESS, pib_config);

    // check for a valid eeprom struct version, reflash default params if invalid
    if (pib_config.eeprom_version != EEPROM_VERSION) {
        ReconfigureEEPROM();
        return false;
    }

    return true;
}

// set to default values
void PIBStorage::ReconfigureEEPROM() {
    pib_config.eeprom_version = EEPROM_VERSION;
    pib_config.sza_minimum = 108; // start of night after astronomical twilight
    pib_config.profile_size = 8000.0f; // revolutions
    pib_config.dock_amount = 200.0f; // revolutions
    pib_config.dock_overshoot = 100.0f; // revolutions
    pib_config.deploy_velocity = 250.0f;
    pib_config.retract_velocity = 250.0f;
    pib_config.dock_velocity = 80.0f;
    pib_config.time_trigger = UINT32_MAX; // set to max for no accidental trigger
    pib_config.dwell_time = 900; // s = 15 min
    pib_config.profile_period = 7200; // s = 2 hrs
    pib_config.num_profiles = 3;
    pib_config.sza_trigger = false;

    EEPROM.put(EEPROM_BASE_ADDRESS, pib_config);
}

bool PIBStorage::Update_bool(uint16_t offset, bool data)
{
    if (EEPROM_BASE_ADDRESS + offset > EEPROM_MAX_ADDRESS) return false;
    if (offset + sizeof(data) > sizeof(PIBConfigs_t)) return false;

    // update the software struct
    *(((bool *) &pib_config) + offset) = data;

    // update eeprom
    EEPROM.put(EEPROM_BASE_ADDRESS + offset, data);

    return true;
}

bool PIBStorage::Update_uint8(uint16_t offset, uint8_t data)
{
    if (EEPROM_BASE_ADDRESS + offset > EEPROM_MAX_ADDRESS) return false;
    if (offset + sizeof(data) > sizeof(PIBConfigs_t)) return false;

    // update the software struct
    *(((uint8_t *) &pib_config) + offset) = data;

    // update eeprom
    EEPROM.put(EEPROM_BASE_ADDRESS + offset, data);

    return true;
}

bool PIBStorage::Update_uint16(uint16_t offset, uint16_t data)
{
    if (EEPROM_BASE_ADDRESS + offset + 1 > EEPROM_MAX_ADDRESS) return false;
    if (offset + sizeof(data) > sizeof(PIBConfigs_t)) return false;

    // update the software struct
    *((uint16_t *) (((uint8_t *) &pib_config) + offset)) = data;

    // update eeprom
    EEPROM.put(EEPROM_BASE_ADDRESS + offset, data);

    return true;
}

bool PIBStorage::Update_uint32(uint16_t offset, uint32_t data)
{
    if (EEPROM_BASE_ADDRESS + offset + 3 > EEPROM_MAX_ADDRESS) return false;
    if (offset + sizeof(data) > sizeof(PIBConfigs_t)) return false;

    // update the software struct
    *((uint32_t *) (((uint8_t *) &pib_config) + offset)) = data;

    // update eeprom
    EEPROM.put(EEPROM_BASE_ADDRESS + offset, data);

    return true;
}

bool PIBStorage::Update_float(uint16_t offset, float data)
{
    if (EEPROM_BASE_ADDRESS + offset + 3 > EEPROM_MAX_ADDRESS) return false;
    if (offset + sizeof(data) > sizeof(PIBConfigs_t)) return false;

    // update the software struct
    *((float *) (((uint8_t *) &pib_config) + offset)) = data;

    // update eeprom
    EEPROM.put(EEPROM_BASE_ADDRESS + offset, data);

    return true;
}