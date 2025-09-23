; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

; Store letters 2,3,4,5 of the 5-letter string
; in a 4-to-3 bit packing
    .macro entry s
;   .byte .strat(s,1), .strat(s,2), .strat(s,3), .strat(s,4)

    .byte ((.strat(s,1) & $1F)<<3 ) | (( .strat(s,2)&$1F ) >>2 )
    .byte ((.strat(s,2) & $03)<<6 ) | (( .strat(s,3)&$1F ) <<1 ) | (( .strat(s,4)&$10 )>>4)
    .byte (( .strat(s,4)&$0F ) << 4)

    .endmacro

.RODATA
    .export _wordsS
_wordsS:
    entry "SABLE" ; 279
    entry "SAGER" ; 280
    entry "SALLY" ; 281
    entry "SALON" ; 282
    entry "SALSA" ; 283
    entry "SALTY" ; 284
    entry "SALVE" ; 285
    entry "SALVO" ; 286
    entry "SAMBA" ; 287
    entry "SANER" ; 288
    entry "SAPPY" ; 289
    entry "SASSY" ; 290
    entry "SATAY" ; 291
    entry "SATIN" ; 292
    entry "SATYR" ; 293
    entry "SAUCY" ; 294
    entry "SAUNA" ; 295
    entry "SAUTE" ; 296
    entry "SAVER" ; 297
    entry "SAVOY" ; 298
    entry "SAVVY" ; 299
    entry "SCALD" ; 300
    entry "SCALP" ; 301
    entry "SCALY" ; 302
    entry "SCAMP" ; 303
    entry "SCANT" ; 304
    entry "SCARF" ; 305
    entry "SCAUP" ; 306
    entry "SCENT" ; 307
    entry "SCHWA" ; 308
    entry "SCIFI" ; 309
    entry "SCION" ; 310
    entry "SCOFF" ; 311
    entry "SCOLD" ; 312
    entry "SCONE" ; 313
    entry "SCOOP" ; 314
    entry "SCOOT" ; 315
    entry "SCORN" ; 316
    entry "SCOUR" ; 317
    entry "SCOWL" ; 318
    entry "SCRAM" ; 319
    entry "SCRAP" ; 320
    entry "SCREE" ; 321
    entry "SCRIM" ; 322
    entry "SCRIP" ; 323
    entry "SCROD" ; 324
    entry "SCRUB" ; 325
    entry "SCRUM" ; 326
    entry "SCUBA" ; 327
    entry "SCUFF" ; 328
    entry "SCULL" ; 329
    entry "SEAMY" ; 330
    entry "SEDAN" ; 331
    entry "SEDER" ; 332
    entry "SEDGE" ; 333
    entry "SEEDY" ; 334
    entry "SEGUE" ; 335
    entry "SENNA" ; 336
    entry "SEPAL" ; 337
    entry "SEPIA" ; 338
    entry "SEPTA" ; 339
    entry "SERER" ; 340
    entry "SERGE" ; 341
    entry "SERIF" ; 342
    entry "SERUM" ; 343
    entry "SERVO" ; 344
    entry "SETUP" ; 345
    entry "SEVER" ; 346
    entry "SEWER" ; 347
    entry "SHACK" ; 348
    entry "SHADY" ; 349
    entry "SHAFT" ; 350
    entry "SHAKO" ; 351
    entry "SHAKY" ; 352
    entry "SHALE" ; 353
    entry "SHANK" ; 354
    entry "SHARD" ; 355
    entry "SHAVE" ; 356
    entry "SHAWL" ; 357
    entry "SHEAF" ; 358
    entry "SHEAR" ; 359
    entry "SHEEN" ; 360
    entry "SHEIK" ; 361
    entry "SHERD" ; 362
    entry "SHILL" ; 363
    entry "SHINY" ; 364
    entry "SHIRE" ; 365
    entry "SHIRK" ; 366
    entry "SHIRR" ; 367
    entry "SHLEP" ; 368
    entry "SHOAL" ; 369
    entry "SHOJI" ; 370
    entry "SHORN" ; 371
    entry "SHOWY" ; 372
    entry "SHRED" ; 373
    entry "SHREW" ; 374
    entry "SHRUB" ; 375
    entry "SHUCK" ; 376
    entry "SHUNT" ; 377
    entry "SHUSH" ; 378
    entry "SHYLY" ; 379
    entry "SIBYL" ; 380
    entry "SIDLE" ; 381
    entry "SIEGE" ; 382
    entry "SIEVE" ; 383
    entry "SIGIL" ; 384
    entry "SIGMA" ; 385
    entry "SILKY" ; 386
    entry "SILTY" ; 387
    entry "SINEW" ; 388
    entry "SINGE" ; 389
    entry "SINUS" ; 390
    entry "SIREN" ; 391
    entry "SISAL" ; 392
    entry "SISSY" ; 393
    entry "SITAR" ; 394
    entry "SIXTY" ; 395
    entry "SIZER" ; 396
    entry "SKATE" ; 397
    entry "SKEET" ; 398
    entry "SKEIN" ; 399
    entry "SKIER" ; 400
    entry "SKIFF" ; 401
    entry "SKIMP" ; 402
    entry "SKULK" ; 403
    entry "SKUNK" ; 404
    entry "SLACK" ; 405
    entry "SLAIN" ; 406
    entry "SLAKE" ; 407
    entry "SLANG" ; 408
    entry "SLANT" ; 409
    entry "SLASH" ; 410
    entry "SLATE" ; 411
    entry "SLEEK" ; 412
    entry "SLEET" ; 413
    entry "SLICK" ; 414
    entry "SLIER" ; 415
    entry "SLILY" ; 416
    entry "SLIME" ; 417
    entry "SLIMY" ; 418
    entry "SLING" ; 419
    entry "SLINK" ; 420
    entry "SLOOP" ; 421
    entry "SLOSH" ; 422
    entry "SLOTH" ; 423
    entry "SLUMP" ; 424
    entry "SLUNG" ; 425
    entry "SLUNK" ; 426
    entry "SLURP" ; 427
    entry "SLUSH" ; 428
    entry "SLYER" ; 429
    entry "SLYLY" ; 430
    entry "SMACK" ; 431
    entry "SMASH" ; 432
    entry "SMEAR" ; 433
    entry "SMELT" ; 434
    entry "SMIRK" ; 435
    entry "SMITE" ; 436
    entry "SMOCK" ; 437
    entry "SMOKY" ; 438
    entry "SMOTE" ; 439
    entry "SNACK" ; 440
    entry "SNAFU" ; 441
    entry "SNAIL" ; 442
    entry "SNAKY" ; 443
    entry "SNARE" ; 444
    entry "SNARK" ; 445
    entry "SNARL" ; 446
    entry "SNEER" ; 447
    entry "SNICK" ; 448
    entry "SNIDE" ; 449
    entry "SNIFF" ; 450
    entry "SNIPE" ; 451
    entry "SNOOD" ; 452
    entry "SNOOK" ; 453
    entry "SNOOP" ; 454
    entry "SNORE" ; 455
    entry "SNORT" ; 456
    entry "SNOUT" ; 457
    entry "SNOWY" ; 458
    entry "SNUFF" ; 459
    entry "SOAPY" ; 460
    entry "SOBER" ; 461
    entry "SOFTY" ; 462
    entry "SOGGY" ; 463
    entry "SOMAL" ; 464
    entry "SONAR" ; 465
    entry "SONIC" ; 466
    entry "SONNY" ; 467
    entry "SOOTH" ; 468
    entry "SOOTY" ; 469
    entry "SOPPY" ; 470
    entry "SORER" ; 471
    entry "SORTA" ; 472
    entry "SOUPY" ; 473
    entry "SOUSE" ; 474
    entry "SOWER" ; 475
    entry "SPACY" ; 476
    entry "SPADE" ; 477
    entry "SPANK" ; 478

    .export _wordListS
_wordListS:
    .word 200
    .byte "S"
    .byte 0
    .word _wordsS
