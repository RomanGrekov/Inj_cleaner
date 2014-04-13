#ifndef GLOBS_H
#define GLOBS_H
#include <stdint.h>

#define F_CPU 96000000
struct enc_struct{
    uint32_t h;
    uint32_t l;
};

#pragma pack(push,1)
struct SavedDomain{
	uint32_t enc[20];
};
#pragma pack(pop)
struct SavedDomain SysConf;

enum {DISARM, HOME, AWAY};
#endif  /* GLOBS_H */
