/*
**  OSSP uuid - Universally Unique Identifier
**  Copyright (c) 2004-2008 Ralf S. Engelschall <rse@engelschall.com>
**  Copyright (c) 2004-2008 The OSSP Project <http://www.ossp.org/>
**
**  This file is part of OSSP uuid, a library for the generation
**  of UUIDs which can found at http://www.ossp.org/pkg/lib/uuid/
**
**  Permission to use, copy, modify, and distribute this software for
**  any purpose with or without fee is hereby granted, provided that
**  the above copyright notice and this permission notice appear in all
**  copies.
**
**  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
**  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
**  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**  IN NO EVENT SHALL THE AUTHORS AND COPYRIGHT HOLDERS AND THEIR
**  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
**  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
**  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
**  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
**  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
**  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
**  SUCH DAMAGE.
**
**  uuid_time.c: Time Management
*/

/* own headers (part (1/2) */
#include "uuid_ac.h"

/* system headers */
#include <stdlib.h>
#ifndef WIN32
    #include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#ifndef WIN32
    #include <sys/time.h>
#endif
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SELECT_H
#ifndef WIN32
    #include <sys/select.h>
#endif
#endif

/* own headers (part (1/2) */
#include "uuid_time.h"


/* POSIX gettimeofday(2) abstraction (without timezone) */
int time_gettimeofday(struct tm *tv)
{
#ifndef WIN32
//    #if defined(HAVE_GETTIMEOFDAY)
//        /* Unix/POSIX pass-through */
//        return gettimeofday(tv, NULL);
//    #elif defined(HAVE_CLOCK_GETTIME)
        /* POSIX emulation */
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
            return -1;
        if (tv != NULL) {
            tv->tm_sec = (long)ts.tv_sec;
            //tv->tv_usec = (long)ts.tv_nsec / 1000;
        }
        return 0;
//    #endif
#else
    /* Windows emulation */
    FILETIME ft;
    LARGE_INTEGER li;
    __int64 t;
    static int tzflag;
    #if !defined(__GNUC__)
        #define EPOCHFILETIME 116444736000000000i64
    #else
        #define EPOCHFILETIME 116444736000000000LL
    #endif
    if (tv != NULL) {
        GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;
        t -= EPOCHFILETIME;
        t /= 10;
        //tv->tv_sec  = (long)(t / 1000000);
        //tv->tv_usec = (long)(t % 1000000);
        tv->tm_sec  = (long)(t / 1000000);
    }
    return 0;

#endif
}

/* BSD usleep(3) abstraction */
int time_usleep(long usec)
{
#if defined(WIN32) && defined(HAVE_SLEEP)
    /* Win32 newer Sleep(3) variant */
    Sleep(usec / 1000);
#elif defined(WIN32)
    /* Win32 older _sleep(3) variant */
    Sleep(usec / 1000);
#elif defined(HAVE_NANOSLEEP)
    /* POSIX newer nanosleep(3) variant */
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 1000 * usec;
    nanosleep(&ts, NULL);
#else
    /* POSIX older select(2) variant */
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = usec;
    select(0, NULL, NULL, NULL, &tv);
#endif
    return 0;
}

