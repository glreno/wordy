#
# Copyright (c) 2025 Gerald Reno, Jr.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#


#
# Your actual project stuff is at the bottom of the file.
#

all: tests disks carts
tests: md_volumeTests.rom md_dictTests.rom md_bsTests.rom md_xebsTests.xex md_spdTests.rom md_wordCount.rom md_scanTests.rom md_xescanTests.xex md_usUkTests.rom md_randTests.rom randy.rom daMetronome.rom dsTests.rom dkTests.rom fontTests.rom fontTestSmall.rom fontTestTiny.rom fontMgrTest.rom vu_letterTest.rom titleTest.rom mw_scoreTest.rom
carts: awordl8k.rom aqordl8k.rom aqordl16k.rom
disks: aqordl128k.xex

# Location of cc65 6502 cross-compiler
CC65HOME = c:/builds/cc65
CC65 = $(CC65HOME)/bin

# Because I HATE SYNTACTIC WHITESPACE IF I DIDN'T I'D BE WRITING PYTHON
.RECIPEPREFIX = &

# Locations of things

bindir = bin
srcdir = src/main
testdir = src/test
tdir = target

.SUFFIXES:
.SUFFIXES: .xex .rom .o .s .asm .c .h .cfg

vpath %.c src/main:src/test
vpath %.asm src/main:src/test:src/main/pic
vpath %.h src/main/include
vpath %.h src/main/pic
vpath %.png src/main/pic
vpath %.inc src/main/include
vpath %.inc src/main/pic
vpath %.inc $(tdir)
vpath %.lib $(tdir)
vpath %.cfg src/main/cfg:src/test/cfg
vpath %.s $(tdir)
vpath %.o $(tdir)
vpath %.xex $(bindir)
vpath %.rom $(bindir)
vpath %.dict src/main/dict:src/test

# General rules

$(bindir)/%.xex: %.cfg %.o
&   $(CC65)/ld65.exe -o $@ -vm -m $@.map -C $^ atari.lib

$(bindir)/%.xex: %.d
&   $(CC65)/ld65.exe -o $@ -m $@.map -t atari atari.lib $^

$(bindir)/%.rom: %.cfg %.o da_envelopes.lib
&   $(CC65)/ld65.exe -o $@ -vm -m $@.map -C $^ atari.lib -L $(tdir) da_envelopes.lib
# -C $^ assumes the .cfg file is first. followed by .o files!
# Order is important; %.rom: %.cfg %.o MUST precede %.rom: %.o

$(bindir)/%.rom: %.o
&   $(CC65)/ld65.exe -o $@ -vm -m $@.map -C atari-cart.cfg -D__CARTFLAGS__=4 $^ atari.lib -L $(tdir) da_envelopes.lib
# Remember, $^ is all prerequisites, $< is just the first one

$(tdir)/%.o: $(tdir)/%.s
&   $(CC65)/ca65.exe -g -t atari -o $@ $<

$(tdir)/%.o: %.asm
&   $(CC65)/ca65.exe -g -t atari -o $@ $<

$(tdir)/%.s: %.c
&   $(CC65)/cc65.exe -g -t atari -O -o $@ -I $(srcdir)/include -I $(srcdir)/pic $<

clean:
&   rm $(tdir)/* $(bindir)/*

#
# Actual project definitions
#

###################################

# The World's Most Useless Cartridge
$(bindir)/useless1.rom: useless1.o
$(bindir)/useless2.xex: useless2.o
$(bindir)/useless2.rom: useless2.o
$(bindir)/useless3.rom: useless3.cfg useless3.o
$(bindir)/useless4.rom: useless4.cfg useless4.o
$(bindir)/useless5.xex: useless5.cfg useless5.o
$(tdir)/useless1.o: useless1.asm
$(tdir)/useless2.o: useless2.c
$(tdir)/useless3.o: useless3.c
$(tdir)/useless4.o: useless4.asm
$(tdir)/useless5.o: useless5.asm
$(bindir)/xegs32test.rom: xegs32test.cfg xegs32test.o xegs32testA.o
$(tdir)/xegs32test.o: xegs32test.c
$(tdir)/xegs32testA.o: xegs32testA.asm
$(bindir)/xegs64test.rom: xegs64test.cfg xegs64test.o xegs64testA.o
$(tdir)/xegs64test.o: xegs64test.c
$(tdir)/xegs64testA.o: xegs64testA.asm


###################################

#
# Wordl
#

# Title screen
$(tdir)/titleTest.o: titleTest.c title.h bankswitch.h
$(tdir)/bankswitch.o: bankswitch.asm
$(tdir)/noBankswitch.o: noBankswitch.asm
$(tdir)/xeBankswitch.o: xeBankswitch.asm
$(tdir)/title_awordl8k.o: title_awordl8k.asm title_template.inc title.h bankswitch.h version.inc
$(tdir)/title_aqordl8k.o: title_aqordl8k.asm title_template.inc title.h bankswitch.h version.inc
$(tdir)/title_aqordl128k.o: title_aqordl128k.asm title_template_disk.inc title.h bankswitch.h version.inc
$(bindir)/titleTest.rom: titleTest.o title_awordl8k.o bankswitch.o
$(tdir)/copydata.o: copydata.asm
$(tdir)/staxptr1.o: staxptr1.asm
$(tdir)/title_aqordl16k.o: title_aqordl16k.asm title_template.inc title.h bankswitch.h version.inc
$(tdir)/options.o: options.asm

# Dictionary Model
$(tdir)/yield.o: yield.asm
$(tdir)/md_wordA.o: md_wordA.asm
$(tdir)/md_volume.o: md_volume.c md_dict.h
$(tdir)/md_dict.o: md_dict.c md_dict.h yield.h
$(tdir)/md_volumeTests.o: md_volumeTests.c md_dict.h
$(tdir)/md_dictTests.o: md_dictTests.c md_dict.h
$(tdir)/md_bsTests.o: md_bsTests.c md_dict.h
$(tdir)/md_xebsTests.o: md_xebsTests.asm
$(tdir)/md_spdTests.o: md_spdTests.c md_dict.h
$(tdir)/md_wordCount.o: md_wordCount.c md_dict.h
$(tdir)/md_scanTests.o: md_scanTests.c md_dict.h
$(tdir)/md_xescanTests.o: md_xescanTests.asm
$(tdir)/md_usUkTests.o: md_usUkTests.c md_dict.h
$(tdir)/md_randTests.o: md_randTests.c md_dict.h
$(tdir)/md_pickA.o: md_pickA.asm
$(tdir)/randy.o: randy.c
$(tdir)/cvcvcDict.o: cvcvcDict.asm
$(tdir)/cvcvcDict1.o: cvcvcDict1.asm
$(tdir)/cvcvcDict2.o: cvcvcDict2.asm
$(tdir)/cvcvcDict3.o: cvcvcDict3.asm
$(tdir)/spdTestDict.o: spdTestDict.s
$(bindir)/md_volumeTests.rom: md_volumeTests.o md_volume.o md_wordA.o cvcvcDict.o staxptr1.o
$(bindir)/md_dictTests.rom: md_dictTests.o md_dict.o yield.o noBankswitch.o md_volume.o md_wordA.o cvcvcDict1.o cvcvcDict2.o cvcvcDict3.o staxptr1.o
$(bindir)/md_bsTests.rom: md_bsTests.cfg md_bsTests.o md_dict.o yield.o bankswitch.o md_volume.o md_wordA.o cvcvcDict1.o cvcvcDict2.o cvcvcDict3.o staxptr1.o
$(bindir)/md_xebsTests.xex: md_xebsTests.cfg md_xebsTests.o md_bsTests.o md_dict.o yield.o xeBankswitch.o md_volume.o md_wordA.o cvcvcDict1.o cvcvcDict2.o cvcvcDict3.o staxptr1.o
$(bindir)/md_spdTests.rom: md_spdTests.o md_dict.o yield.o bankswitch.o md_volume.o md_wordA.o spdTestDict.o staxptr1.o
$(bindir)/md_wordCount.rom: md_wordCount.cfg md_wordCount.o md_dict.o yield.o bankswitch.o md_volume.o md_wordA.o aqordlDict.o staxptr1.o -ldict
$(bindir)/md_scanTests.rom: md_scanTests.cfg md_scanTests.o md_dict.o yield.o bankswitch.o md_volume.o md_wordA.o aqordlDict.o staxptr1.o -ldict
$(bindir)/md_xescanTests.xex: md_xescanTests.cfg md_xescanTests.o md_scanTests.o md_dict.o yield.o xeBankswitch.o md_volume.o md_wordA.o aqordlDict.o staxptr1.o -ldict
$(bindir)/md_usUkTests.rom: md_usUkTests.o md_dict.o yield.o bankswitch.o md_volume.o md_wordA.o aqordlDict.o staxptr1.o -ldict
$(bindir)/md_randTests.rom: md_randTests.o md_dict.o yield.o bankswitch.o md_volume.o md_wordA.o aqordlDict.o staxptr1.o -ldict
$(bindir)/randy.rom: randy.o

# Screen driver
$(tdir)/ds_screendriver.o: ds_screendriver.c ds_screendriver.h
$(tdir)/dsTests.o: dsTests.c ds_screendriver.h
$(tdir)/ds_screendriverA.o: ds_screendriverA.asm ds_screendriverA.inc
$(tdir)/ds_copyPic.o: ds_copyPic.asm
$(tdir)/ds_screendriverI.o: ds_screendriverI.asm ds_screendriverA.inc
$(bindir)/dsTests.rom: dsTests.cfg dsTests.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o

# Keyboard driver
$(tdir)/dkTests.o: dkTests.c dk_kbdriver.h ds_screendriver.h yield.h
$(tdir)/dk_kbdriver.o: dk_kbdriver.asm dk_kbdriver.h ds_screendriver.h yield.h
$(bindir)/dkTests.rom: dkTests.cfg dkTests.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o dk_kbdriver.o yield.o

# Audio driver
$(tdir)/da_audioDriverI.o: da_audioDriverI.asm
$(tdir)/da_audioDriverA.o: da_audioDriverA.asm trampoline.inc
$(tdir)/da_audioDriverB.o: da_audioDriverB.asm
$(tdir)/ae_abrupt.o: ae_abrupt.asm
$(tdir)/ae_simple.o: ae_simple.asm
$(tdir)/ae_trailing.o: ae_trailing.asm
$(tdir)/ae_drum.o: ae_drum.asm
$(tdir)/daMetronome.o: daMetronome.asm ae_simple.inc
$(bindir)/daMetronome.rom: daMetronome.cfg daMetronome.o da_audioDriverI.o da_audioDriverA.o noBankswitch.o staxptr1.o
$(tdir)/da_envelopes.lib: ae_simple.o ae_abrupt.o ae_drum.o ae_trailing.o
&   $(CC65)/ar65.exe r $@ $^

# Single Puzzle View
$(tdir)/vw_base.o: vw_base.c vw_base.h ds_screendriver.h vfm_fontmanager.h
$(tdir)/vw_baseA.o: vw_baseA.asm
$(tdir)/vw_small.o: vw_small.c vw_small.h vw_base.h ds_screendriver.h vfm_fontmanager.h
$(tdir)/vw_tinyA.o: vw_tinyA.asm
$(tdir)/vu_letters.o: vu_letters.asm trampoline.inc
$(tdir)/vfm_fontmanager.o: vfm_fontmanager.c vfm_fontmanager.h ds_screendriver.h
$(tdir)/vfm_fontmanagerA.o: vfm_fontmanagerA.asm
$(tdir)/vfm_fontmanagerB.o: vfm_fontmanagerB.asm
$(tdir)/fontTests.o: fontTests.c vw_small.h vw_base.h vfm_fontmanager.h ds_screendriver.h
$(tdir)/awordlFont.o: awordlFont.c awordlFont.h
$(tdir)/aqordlFont.o: aqordlFont.c aqordlFont.h
$(tdir)/interactFont.o: interactFont.asm
$(bindir)/fontTests.rom: fontTests.cfg fontTests.o vw_small.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o noBankswitch.o staxptr1.o
$(tdir)/fontTestSmall.o: fontTestSmall.c vw_small.h vw_base.h vfm_fontmanager.h ds_screendriver.h awordlFont.h
$(tdir)/fontTestTiny.o: fontTestTiny.c vw_tiny.h vw_base.h vfm_fontmanager.h ds_screendriver.h aqordlFont.h
$(bindir)/fontTestSmall.rom: fontTestSmall.cfg fontTestSmall.o vw_small.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o awordlFont.o noBankswitch.o staxptr1.o
$(bindir)/fontTestTiny.rom: fontTestTiny.cfg fontTestTiny.o vw_tinyA.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o aqordlFont.o interactFont.o noBankswitch.o staxptr1.o
$(tdir)/fontMgrTest.o: fontMgrTest.c vw_small.h vw_base.h vfm_fontmanager.h ds_screendriver.h awordlFont.h
$(tdir)/fontMgrTestA.o: fontMgrTestA.asm ds_screendriverA.inc
$(bindir)/fontMgrTest.rom: fontMgrTest.cfg fontMgrTest.o fontMgrTestA.o vw_small.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o vfm_fontmanagerB.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o awordlFont.o noBankswitch.o staxptr1.o
$(tdir)/vu_letterTest.o: vu_letterTest.c vu_letters.h ds_screendriver.h
$(bindir)/vu_letterTest.rom: vu_letterTest.cfg vu_letterTest.o vu_letters.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o interactFont.o noBankswitch.o staxptr1.o

# Single Puzzle Model
$(tdir)/mw_puz.o: mw_puz.c mw_puz.h vw_small.h vw_base.h vfm_fontmanager.h
$(tdir)/mw_puzA.o: mw_puzA.asm trampoline.inc
$(tdir)/mw_scoreTest.o: mw_scoreTest.c mw_puz.h vw_small.h vw_base.h vfm_fontmanager.h vfm_fontmanagerA.o ds_screendriver.h awordlFont.h
$(bindir)/mw_scoreTest.rom: mw_scoreTest.o mw_puz.o mw_puzA.o vw_small.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o awordlFont.o noBankswitch.o staxptr1.o

# Four Puzzle Model
$(tdir)/mq_puzA.o: mq_puzA.asm
$(tdir)/mq_puz.o: mq_puz.c mq_puz.h mw_puz.h vw_tiny.h vw_base.h vfm_fontmanager.h

# Simple opponent view
$(tdir)/vo_base.o: vo_base.asm
$(tdir)/vo_textA.o: vo_textA.asm
$(tdir)/vo_text.o: vo_text.c vo_text.h vo_base.h da_audioDriver.h

# Wordl only, 8K RAM version
$(tdir)/mo_wordl.o: mo_wordl.c mo_wordl.h mw_puz.h mw_puz.h vw_tiny.h vw_base.h vfm_fontmanager.h ds_screendriver.h da_audioDriver.h dk_kbdriver.h aqordlFont.h title.h bankswitch.h vo_base.h vu_letters.h
$(tdir)/awordl8k.o: awordl8k.c mo_wordl.h mw_puz.h vw_small.h vw_base.h vfm_fontmanager.h ds_screendriver.h awordlFont.h title.h bankswitch.h vo_text.h vo_base.h
$(bindir)/awordl8k.rom: awordl8k.cfg awordl8k.o mo_wordl.o mw_puz.o mw_puzA.o vw_small.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o da_audioDriverI.o da_audioDriverA.o dk_kbdriver.o title_awordl8k.o bankswitch.o md_dict.o yield.o vo_base.o vo_textA.o vo_text.o vu_letters.o md_volume.o md_wordA.o aqordlDict.o interactFont.o awordlFont.o staxptr1.o -ldict

# Qordl, 8K RAM version
$(tdir)/mo_qordlA.o: mo_qordlA.asm
$(tdir)/mo_qordl.o: mo_qordl.c mo_qordl.h mq_puz.h mw_puz.h vw_tiny.h vw_base.h vfm_fontmanager.h ds_screendriver.h da_audioDriver.h dk_kbdriver.h aqordlFont.h title.h bankswitch.h vo_text.h vo_base.h vu_letters.h
$(tdir)/aqordl8k.o: aqordl8k.c mo_qordl.h mq_puz.h mw_puz.h vw_tiny.h vw_base.h vfm_fontmanager.h ds_screendriver.h dk_kbdriver.h aqordlFont.h title.h bankswitch.h vo_text.h vo_base.h
$(bindir)/aqordl8k.rom: aqordl8k.cfg aqordl8k.o md_pickA.o mo_qordlA.o mo_qordl.o mq_puzA.o mq_puz.o mw_puz.o mw_puzA.o vw_tinyA.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_screendriverI.o ds_screendriver.o da_audioDriverI.o da_audioDriverA.o dk_kbdriver.o title_aqordl8k.o bankswitch.o md_dict.o yield.o vo_base.o vo_textA.o vo_text.o vu_letters.o md_volume.o md_wordA.o aqordlDict.o aqordlFont.o interactFont.o copydata.o staxptr1.o options.o -ldict


# Robot opponent view
$(tdir)/vo_anim.o: vo_anim.c ds_screendriver.h vo_robot.h vo_anim.h vo_base.h da_audioDriver.h
$(tdir)/vo_animA.o: vo_animA.asm
$(tdir)/vo_robotA.o: vo_robotA.asm ds_screendriverA.inc
$(tdir)/vo_robot.o: vo_robot.c vo_robot.h vo_anim.h vo_base.h da_audioDriver.h

$(tdir)/cat_top.o: cat_top.s
$(tdir)/cat_bot.o: cat_bot.s
$(tdir)/cat_top.s: src/main/pic/cat_24x128.png src/main/cat.groovy
&    groovy src/main/cat.groovy src/main/pic/cat_24x128.png cat_top 0,0 > $(tdir)/cat_top.s
$(tdir)/cat_bot.s: src/main/pic/cat_24x128.png src/main/cat.groovy
&    groovy src/main/cat.groovy src/main/pic/cat_24x128.png cat_bot 0,64 > $(tdir)/cat_bot.s

$(tdir)/robot0_pic.o: robot0_pic.s
$(tdir)/robot0_pic.s: robot0_pic.h robot0.24x128.png robot0.angry.png robot0.wrist1.png robot0.wrist2.png robot0.wrist3.png robot0.left.png robot0.leftpoint.png robot0.leftkick.png robot0.leftkickback.png robot0.right.png robot0.rightpoint.png robot0.rightkick.png robot0.rightkickback.png robot0.jump1.png robot0.jump2.png robot0.jump5.png
&    groovy src/main/pictoasm.groovy $^ > $(tdir)/robot0_pic.s

$(tdir)/robot0_pic.inc: robot0_pic.h
&    groovy src/main/pictoinc.groovy $^ > $(tdir)/robot0_pic.inc

$(tdir)/robot0_frame.o: robot0_frame.asm robot0_pic.inc
$(tdir)/robot0_frame.inc: robot0_frame.asm
&    groovy src/main/frametoinc.groovy $^ > $(tdir)/robot0_frame.inc

$(tdir)/robot0_anim.o: robot0_anim.s robot0_frame.asm robot0_frame.inc robot0_cueball.inc
$(tdir)/robot0_anim.s: robot0_anim.h
&    groovy src/main/animtoasm.groovy $^ > $(tdir)/robot0_anim.s

$(tdir)/robot0_envelopes.o: robot0_envelopes.asm
$(tdir)/robot0_cueball.o: robot0_cueball.asm robot0_envelopes.inc cueballmac.inc ae_abrupt.inc ae_simple.inc ae_trailing.inc ae_drum.inc

$(tdir)/robot0_cueball.inc: robot0_cueball.asm cueballmac.inc
&    groovy src/main/cueballtoinc.groovy $^ > $(tdir)/robot0_cueball.inc

# Qordl, 16K RAM version
$(tdir)/aqordl16k.o: aqordl16k.c mo_qordl.h mq_puz.h mw_puz.h vw_tiny.h vw_base.h vfm_fontmanager.h ds_screendriver.h dk_kbdriver.h aqordlFont.h title.h bankswitch.h vo_robot.h vo_anim.h vo_base.h
$(bindir)/aqordl16k.rom: aqordl16k.cfg aqordl16k.o md_pickA.o mo_qordlA.o mo_qordl.o mq_puzA.o mq_puz.o mw_puz.o mw_puzA.o vw_tinyA.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_copyPic.o ds_screendriverI.o ds_screendriver.o da_audioDriverI.o da_audioDriverA.o dk_kbdriver.o title_aqordl16k.o bankswitch.o md_dict.o yield.o vo_base.o vo_anim.o vo_animA.o vo_robotA.o vo_robot.o vu_letters.o md_volume.o md_wordA.o aqordlDict.o aqordlFont.o interactFont.o copydata.o robot0_pic.o robot0_frame.o robot0_anim.o robot0_envelopes.o robot0_cueball.o staxptr1.o options.o -ldict

# Qordl, 128K RAM version
$(tdir)/xlBasicDisable.o: xlBasicDisable.asm
$(tdir)/xeCheck.o: xeCheck.asm
$(tdir)/aqordl128k.o: aqordl128k.c mo_qordl.h mq_puz.h mw_puz.h vw_tiny.h vw_base.h vfm_fontmanager.h ds_screendriver.h dk_kbdriver.h aqordlFont.h title.h bankswitch.h vo_robot.h vo_anim.h vo_base.h
$(bindir)/aqordl128k.xex: aqordl128k.cfg aqordl128k.o md_pickA.o mo_qordlA.o mo_qordl.o mq_puzA.o mq_puz.o mw_puz.o mw_puzA.o vw_tinyA.o vw_base.o vw_baseA.o vfm_fontmanager.o vfm_fontmanagerA.o ds_screendriverA.o ds_copyPic.o ds_screendriverI.o ds_screendriver.o da_audioDriverI.o da_audioDriverA.o dk_kbdriver.o title_aqordl128k.o xeBankswitch.o md_dict.o yield.o vo_base.o vo_anim.o vo_animA.o vo_robotA.o vo_robot.o vu_letters.o md_volume.o md_wordA.o aqordlDict.o aqordlFont.o interactFont.o copydata.o robot0_pic.o robot0_frame.o robot0_anim.o robot0_envelopes.o robot0_cueball.o staxptr1.o xlBasicDisable.o options.o xeCheck.o -ldict -lda_envelopes

# Dictionaries

$(tdir)/spdTestDict.s: src/test/mkSpdTestDict.groovy
&    groovy src/test/mkSpdTestDict.groovy > $(tdir)/spdTestDict.s

$(tdir)/aqordlDict.s: main.dict main_us.dict main_uk.dict top.dict top_us.dict top_uk.dict bad.dict naughty.dict rare.dict
&    groovy src/main/buildDict.groovy $^

# VOL_*.s and DICT_*.s are all made from the same buildDict.groovy, at the same time
# The .o is automatically built from the .s, but we need a couple empty rules to say where to get the .s from

$(tdir)/DICT_%.s: aqordlDict.s
&
$(tdir)/VOL_%.s: aqordlDict.s
&

$(tdir)/aqordlDict.o: aqordlDict.s version.inc

$(tdir)/dict.lib: $(tdir)/DICT_EA.o $(tdir)/DICT_HA.o $(tdir)/DICT_MA.o $(tdir)/DICT_SA.o $(tdir)/DICT_ES.o $(tdir)/DICT_ES_US.o $(tdir)/DICT_ES_UK.o $(tdir)/DICT_MS.o $(tdir)/DICT_MS_US.o $(tdir)/DICT_MS_UK.o $(tdir)/DICT_HS.o $(tdir)/DICT_HS_US.o $(tdir)/DICT_HS_UK.o $(tdir)/DICT_SS.o $(tdir)/DICT_SS_US.o $(tdir)/DICT_SS_UK.o $(tdir)/VOL_main1.o $(tdir)/VOL_main2.o $(tdir)/VOL_main_us1.o $(tdir)/VOL_main_uk1.o $(tdir)/VOL_main1D.o $(tdir)/VOL_main2D.o $(tdir)/VOL_main_us1D.o $(tdir)/VOL_main_uk1D.o $(tdir)/VOL_top1.o $(tdir)/VOL_top_us1.o $(tdir)/VOL_top_uk1.o $(tdir)/VOL_top1D.o $(tdir)/VOL_top_us1D.o $(tdir)/VOL_top_uk1D.o $(tdir)/VOL_rare1.o $(tdir)/VOL_naughty1.o $(tdir)/VOL_bad1.o
&   $(CC65)/ar65.exe r $@ $^

###################################
