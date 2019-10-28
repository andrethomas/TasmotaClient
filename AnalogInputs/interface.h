#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#define CMND_START                     0xFC
#define CMND_END                       0xFD

#define CMND_FEATURES                  0x01
#define CMND_JSON                      0x02
#define CMND_SECOND_TICK               0x03

#define PARAM_DATA_START               0xFE
#define PARAM_DATA_END                 0xFF

struct COMMAND {
  uint8_t command;
  uint8_t parameter;
  uint8_t unused2;
  uint8_t unused3;
} Command;

#endif // __INTERFACE_H__
