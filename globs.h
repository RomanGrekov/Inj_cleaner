#ifndef GLOBS_H
#define GLOBS_H
#include <stdint.h>

#define F_CPU 96000000

#pragma pack(push,1)
struct enc_struct{
    uint32_t h;
    uint32_t l;
};

struct SavedDomain{
	uint32_t enc[40];
};
#pragma pack(pop)
extern struct SavedDomain SysConf;

enum {DISARM, HOME, AWAY};
#endif  /* GLOBS_H */
