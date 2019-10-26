#ifndef __SETTINGS_H__
#define __SETTINGS_H__

typedef union {
  uint16_t data;
  struct {
    uint16_t json : 1;
    uint16_t spare1 : 1;
    uint16_t spare2 : 1;
    uint16_t spare3 : 1;
    uint16_t spare4 : 1;
    uint16_t spare5 : 1;
    uint16_t spare6 : 1;
    uint16_t spare7 : 1;
    uint16_t spare8 : 1;
    uint16_t spare9 : 1;
    uint16_t spare10 : 1;
    uint16_t spare11 : 1;
    uint16_t spare12 : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} FeatureCfg;

/*
 * The structure below must remain 4 byte aligned to be compatible with
 * Tasmota as master
 */

struct FEATURES {
  uint32_t features_version;
  FeatureCfg features;
  uint16_t spare4;
} Settings;

#endif // __SETTINGS_H__
