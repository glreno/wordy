/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* ds_screendriver.h
 *
 * Declarations for wordl screen driver.
 * Really low level stuff.
 * There might be some inlines in here.
 *
 */

#ifndef __DS_SCREENDRIVER_H__
#define __DS_SCREENDRIVER_H__

#include <stdlib.h>

/* ######################################################### */
/* #                  SCREEN DRIVER                        # */
/* ######################################################### */

/* Header for a screen page.
* The page contains at minimum 2K ($800) of player/missile data;
* PMBASE will be set to the start of the page,
* and this structure is in the 768 byte 'unused' PM area.
*/
typedef struct ds_pageHeaderStruct
{
    /* Address of this page; technically a *ds_pageHeader */
    // OFFSET: +0
    void *myAddress;
    /* If you need an ID number, put it here */
    // OFFSET: +2
    int id;
    /* those four bytes are in the union as page->id */

    /* Status flags for your convenience. Available as an int in ds_colormap.flag */
    // OFFSET: +4
    unsigned char screenState; // Set to 1 by ds_flipToPageNextVBI; set to 0 when displayed; set to -1 when removed from screen
    unsigned char queueState;  // for application use

    // OFFSET: +6
    void *next; // pointer to next ds_page to display (to be used by the application's animation timer handler)

    // OFFSET: +8
    int spare[3];  // padding so that dlist starts at +192 (0xC0)

    /* Player-missile positions and sizes */
    // OFFSET: +14 (0x0E)
    unsigned char pmpos[13];

    /* Colors to be loaded during page switch */
    // OFFSET: +27 (0x1B)
    unsigned char colors[9];


    /* A pointer to a font for every value of VCOUNT $D40B.
    * The length is 156 for PAL; on NTSC, VCOUNT only goes up to 130.
    * Every DLI, this value will be poked in to CHBASE $D409,
    * so a good default value for the entire thing is $E0.
    * Note that the first few fonts are not in the visible portion
    * of the screen, so that is a handy place to store the
    * page's assigned font IDs.
    */
    // OFFSET +36 (0x24)
    char fonts[156];

    /* The display list. Size doesn't really matter here,
    * since it's defined to be last in the struct.
    * But the default Gr.0 display list is 32 bytes,
    * and so this is room to do a lot more than that.
    * Just not enough for Gr.7.
    * Keep in mind that a display list cannot cross a 1K (0x400) boundary,
    * so starting at 0xC0 you have 832 bytes, really.
    */
    // OFFSET +192 (0xC0)
    char displaylist[64];

    // OFFSET +256 (0x100)
    // 512 bytes unused space until +768 (0x300)
    unsigned char freespace[0x200];
} ds_pageHeader;

typedef struct ds_colormapStruct
{
    unsigned char unused[4];
    unsigned int flags;
    unsigned char spare[8];
    // these aren't colors, they're the previous array, pm size & position
    unsigned char hposp0; // 53248 $D000
    unsigned char hposp1; // 53249 $D001
    unsigned char hposp2; // 53250 $D002
    unsigned char hposp3; // 53251 $D003
    unsigned char hposm0; // 53252 $D004
    unsigned char hposm1; // 53253 $D005
    unsigned char hposm2; // 53254 $D006
    unsigned char hposm3; // 53255 $D007
    unsigned char sizep0; // 53256 $D008
    unsigned char sizep1; // 53257 $D009
    unsigned char sizep2; // 53258 $D00a
    unsigned char sizep3; // 53259 $D00b
    unsigned char sizem;  // 53260 $D00c
    // The colors
    unsigned char pcolr0; // 53266 $D012 shadow 704 $2C0
    unsigned char pcolr1; // 53267 $D013 shadow 705 $2C1
    unsigned char pcolr2; // 53268 $D014 shadow 706 $2C2
    unsigned char pcolr3; // 53269 $D015 shadow 707 $2C3
    unsigned char color0; // 53270 $D016 shadow 708 $2C4 default 40
    unsigned char color1; // 53271 $D017 shadow 709 $2C5 default 202
    unsigned char color2; // 53272 $D018 shadow 710 $2C6 default 148
    unsigned char color3; // 53273 $D019 shadow 711 $2C7 default 70
    unsigned char color4; // 53274 $D01A shadow 712 $2C8 default 0
} ds_colormap;

/** structure of high-res PM graphics.
* A graphics page is actually a union of this and ds_page_headerStruct!
*/
typedef struct ds_pmDataStruct
{
    unsigned char pageheader[256];
    unsigned char freespace[512];
    unsigned char missiles[256];
    unsigned char player0[256];
    unsigned char player1[256];
    unsigned char player2[256];
    unsigned char player3[256];
} ds_pmData;

typedef union ds_pageUnion
{
    ds_pageHeader page;
    ds_pmData pm;
    ds_colormap colors;
    unsigned char id[4];
} ds_page;

/* ######################################################### */
/* #           SCREEN PAGE STATE VALUES                    # */
/* ######################################################### */

#define DS_SCREENSTATE_INITIAL 3
#define DS_SCREENSTATE_QUEUED 2
#define DS_SCREENSTATE_ISNEXT 1
#define DS_SCREENSTATE_ONSCREEN 0
#define DS_SCREENSTATE_OFFSCREEN 0xFF

/*
* Zero out a block of RAM, and tell the OS that it's the screen.
* Does not update the display list; call ds_init or ds_switchToPageNextVBI to do that.
*
* Note that if you set __RESERVED_MEMORY__ , that the cc65 library will
* actually reserve that amount plus the 992 bytes needed for Gr.0 ram+dlist.
* And stuck the screen at the END of that memory.
* So if your cart starts at $A000 and you reserve $2000 (8K), it is safe
* to start the screen RAM at A000-23E0 = 7C20 and put a ds_pageHeader at 8000.
*
* This can also be used to clear the screen; NEVER print CHR$(125), that
* wipes out ALL ram from SAVMSC to MEMTOP!!!
*/
void __fastcall__ ds_initScreenRam(unsigned char *const location, size_t length);

/*
* Disable the screen while the initial setup is going on.
* ds_init() will reenable it.
* Also disables the DLI and VBI callbacks, in case they are already enabled.
*/
void __fastcall__ ds_disableScreen();

/*
* page is a block of at least 2K, since it is ALSO the
*     PMbase for that page.
* the page will actually start with a short header (struct ds_pageheader)
* after the header, the font array (size in header), and the dlist.
*
* Screen location is implicit in the display list.
* The screen is NOT cleared in this function, and
* none of the screen pointers (i.e.SAVMSC) are set here.
* call ds_initScreenRam() first.
*
* 1. Load the page address into the 'next page' location.
*    (it just calls ds_switchToNextPage)
* 2. Enable the DLI and PM graphics
* 3. Enable the VBI
* 4. The first VBI will load SDLSTL and PMBASE.
* The font will be changed on the first DLI; this means that
* CHBAS $2F4 will NOT be honored unless the page's displaylist contains no interrupts.
*/
void __fastcall__ ds_init(ds_page *page);

/*
* Initialize a page. Stash the page address, zero out all PM data,
* load the system default colors, and set the font to the system font $E000.
* Does not load anything into the display list, since you'll replace it anyway
*/
void __fastcall__ ds_init_page(ds_page *page);

/*
* Load a font into a page.
* Update a range in the fontlist to point to a specific font.
* Range is given in text rows, but the data structure stores in VCOUNT rows -- 4 per text row.
* Row 0 is the first row of text on the screen in the normal 46 46 C6 82 display list.
* Note that the third block of blank rows has the DLI bit set!
* offset_vcount should be zero, but you may need to add a line or two to match your display list.
* Max value for end is 34 (15 + 4*34 + 3 = 154)
* Min value for start is -3 (15 + 4*-3 = 3)
* (Note that means that rows 0,1,2 and 155 can't be set with this function.
* But those are definitely off-screen in all cases.)
*/
void __fastcall__ ds_setFont(ds_page *page, unsigned char fontbase, signed char offset_vcount, char start, char end);

/*
* The page flip will occur in the VBI, and a callback will be called.
* There is no queue. Just one pointer, and it will be set next VBI.
*/
void __fastcall__ ds_flipToPageNextVBI(ds_page *page);

/*
* The page flip will occur in the VBI, and a callback will be called.
* Busy-waits until the page actually changes.
* This is a good thing to call before major screen redraws.
*/
void __fastcall__ ds_flipToPageNextVBIandWait(ds_page *page);

/*
* Set an ML callback routine to call
* end of the DLI.
* Keep it short! Maybe set a timer or a flag.
* This will be called while the next scanline is being drawn!
* Set to NULL to disable the callback.
*/
void __fastcall__ ds_setDliCallback(void *const cb);

/*
* Set an ML callback routine to call after flipping pages.
* This is called at the end of the VBI.
* Keep it short! Just set a flag or something.
* Register A will contain the new page ID
* Register X will contain the old page ID
* Set to NULL to disable the callback.
*/
void __fastcall__ ds_setFlipCallback(void *const cb);

/*
* Set an ML callback routine to call every N jiffies.
* Timers are decremented at the end of every VBI. When CDTMV1
* reaches zero, the callback will be called.
* The callback is thus called in the VBI context.
*
* Uses system timer 1 (CDTMV1 $218,$219 and CDTMA2 $226,$227)
* Keep it short! Maybe request another page flip,
* or set a flag to inform the main application thread.
*
* Set to NULL or DS_NOCB to disable the callback.
*/
void __fastcall__ ds_setAnimTimerCallback(unsigned int jiffies,void *const cb);
extern void *DS_NOCB;

/*
* Restart the animation timer.
* Shouldn't be necessary, setting the callback will restart it.
*/
void ds_restartAnimTimer();

/*
* Prints to shared screen RAM.
* Assumes 40 char rows and contiguous data.
* These are for INTERNAL code strings.
* For ASCII, use cputcxy(x,y,char c) and cputsxy(x,y,char *s)
*
* May need variants that do their work during VBI, or at least
* an ML version that can be called in the page flip callback.
*/
void __fastcall__ ds_puts(const char *const s,char len,char x,char y);
void __fastcall__ ds_putc(char c,char x,char y);
/*
* Print TWO characters. Note that they are in the opposite order.
*/
void __fastcall__ ds_put2c(char right,char left,char x,char y);

/*
* Selects the color palette from a hardcoded list of ten.
*/
void __fastcall__ ds_setStandardPalette(char p,ds_page *page);
extern unsigned char *ds_defaultColors;

/*
* Selects the color palette for PAL or NTSC
*/
void __fastcall__ ds_setDefaultPalette(ds_page *page);

/*
* Copy a pic (see pngtoasmpic.groovy) into a font
*/
void __cdecl__ ds_copyPicToFont(void *pic[], unsigned char fontbase);

/*
* Clear all the pic characters from TWO fonts
*/
void __fastcall__ ds_clearFontPic( unsigned char fontbase1, unsigned char fontbase2);

/*
* Copy the first row of a bank from font1 to font2
*/
void __fastcall__ ds_copyBelt( unsigned char fontbase1, unsigned char fontbase2);

extern unsigned char DS_PICIDX_BANKID;

#endif
