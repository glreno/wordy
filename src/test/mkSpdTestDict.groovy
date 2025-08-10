/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

import java.util.ArrayList;
import java.util.List;

public class mkSpdTstDict
{
    public static void main(String [] args)
    {
        mkSpdTstDict m = new mkSpdTstDict();
        m.create();
        m.write();
    }

    public void create()
    {
        char [] buf = new char[5];
        for(buf[0] = 'A'; buf[0] <= 'A'; buf[0]++ )
        {
            for(buf[1] = 'A'; buf[1] <= 'A'; buf[1]++ )
            {
                for(buf[2] = 'A'; buf[2] <= 'G'; buf[2]++ )
                {
                    for(buf[3] = 'A'; buf[3] <= 'Z'; buf[3]++ )
                    {
                        for(buf[4] = 'A'; buf[4] <= 'Z'; buf[4]++ )
                        {
                            String s=new String(buf);
                            // If you just print all the words from AAAAA to ZZZZZ, it's 80M of 11,881,376 words, so store them all and split them up later.
                            store(s);
                            // AAAAA-AAEZZ = 3380 words, 20280 bytes, 
                            // which is well within the allowed 24KB.
                        }
                    }
                }
            }
        }
    }

    private List<String> big = new ArrayList<>();
    private List<String> aaa = new ArrayList<>();
    private List<String> aa = new ArrayList<>();
    private List<String> a = new ArrayList<>();

    public void store(String s)
    {
        // While the words are being generated, store them in
        // 2.5 8KB dictionaries, and three more smaller ones.
        // The smallest dictionaries are words that end in AAA, AA, and A.
        if ( s.endsWith("AAA") )
        {
            aaa.add(s);
        }
        else if ( s.endsWith("AA") )
        {
            aa.add(s);
        }
        else if ( s.endsWith("A") )
        {
            a.add(s);
        }
        else
        {
            big.add(s);
        }
    }

    public void write()
    {
        System.out.println("; Big: "+big.size()+" words, "+big.size()*6+" bytes");
        System.out.println(";   A: "+a.size()+" words, "+a.size()*6+" bytes");
        System.out.println(";  AA: "+aa.size()+" words, "+aa.size()*6+" bytes");
        System.out.println("; AAA: "+aaa.size()+" words, "+aaa.size()*6+" bytes");
        // Max number of words in an 8K bank: 1365
        write(0,"Big0", big.subList(0,1365));
        write(1,"Big1", big.subList(1365,2730));
        write(2,"Big2", big.subList(2730,4095));
        write(3,"Big3", big.subList(4095,big.size()));
        write(3,"A", a);
        write(3,"AA", aa);
        write(3,"AAA", aaa);
        System.out.println("    .RODATA");
        System.out.println("_CVDICT:");
        System.out.println("    .export _CVDICT");
        System.out.println("    .word 7 ; number of volumes");
        System.out.println("    .word _vol_Big0");
        System.out.println("    .word _vol_Big1");
        System.out.println("    .word _vol_Big2");
        System.out.println("    .word _vol_Big3");
        System.out.println("    .word _vol_A");
        System.out.println("    .word _vol_AA");
        System.out.println("    .word _vol_AAA");
        System.out.println();
        System.out.println("    .segment \"CARTBANK4\"");
        System.out.println("    .byte \"This is bank 4\"");
        System.out.println("    .segment \"CARTBANK5\"");
        System.out.println("    .byte \"This is bank 5\"");
        System.out.println("    .segment \"CARTBANK6\"");
        System.out.println("    .byte \"This is bank 6\"");
    }

    int idx = 0;
    public void write(int bank, String name, List<String> words)
    {
        System.out.println("    .segment \"CARTBANK"+bank+"\"");
        System.out.println("_dict_"+name+":");
        System.out.println("    .export _dict_"+name);
        for(String s : words)
        {
            System.out.println("    .byte \""+s+"\", \$1 ; "+idx);
            ++idx;
        }
        System.out.println("    .RODATA");
        System.out.println("_vol_"+name+":");
        System.out.println("    .export _vol_"+name);
        System.out.println("    .word "+words.size()+" ; n words");
        System.out.println("    .byte "+bank+" ; bank ID");
        System.out.println("    .word _dict_"+name);
        System.out.println();
    }
}
