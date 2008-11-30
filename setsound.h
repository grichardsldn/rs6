#ifndef __SETSOUND_H_
#define __SETSOUND_H_

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <stdio.h>

#ident "$Id: setsound.h,v 1.3 2000/11/28 21:47:57 graham Exp graham $"

extern int set_frag(int fd);
extern int set_card(int cfd, int rate, int format, int channels);
extern FILE *dsp_open(char *type, int rate, int format, int channels);

#endif /* __SETSOUND_H_ */	
