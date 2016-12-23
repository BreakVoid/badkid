//
// readself.h
// Created by 柯嵩宇 on 2016/12/20.

#ifndef _readself_h_
#define _readself_h_

#include <ctype.h>
#include <stdio.h>

extern char *selfName;

void GetSelf();

size_t GetSelfSize();

void ReadSelf(void *buffer);

#endif