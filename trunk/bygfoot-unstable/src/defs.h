/************************************************************
 * the global definitions file containing everything        *
 * that's needed in more than 1 .c file                     *
 ***********************************************************/

#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gtk/gtk.h>

/* 'define's and enums */
#include "enums.h"
/* custom structures */
#include "structures.h"
/* global variables */
#include "variables.h"

#endif
