/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/
import groovy.io.GroovyPrintWriter;

public class BuildDict
{
    int [] freqs = new int[27];

    void countFreq(String w) {
        for (char c : w) {
            int i = c-(char)'A';
            freqs[i]++;
        }
    }

    public static class WordList
    {
        protected int bank=-1;
        protected Map<String,Integer> words = new TreeMap<>();
        protected String name;
        protected char first;

        public WordList(char c,String n)
        {
            first=c;
            name = n;
        }

        public store(String word,int flags)
        {
            words.put(word,flags);
        }

        public int size() { return words.size();}

        public String getName()
        {
            return name;
        }

        public char getFirst()
        {
            return first;
        }

        public void setBank(int b)
        {
            this.bank = b;
        }

        public int getBank()
        {
            return bank;
        }

        String compress(String s)
        {
            // 4-to-3 byte bit packing
            // We will discard all but the lowest 5 bits of each ascii character,
            // and then pack them in to 3 bytes. The remaining 4 bits will store the flags.

            // So for a given five-letter word ABCDE with flags F,
            // remembering that the first letter is discarded, we get bits:
            //   0     1         2         3         <- src index
            // bbbb bccc    ccdd ddde    eeee ffff
            // ^byte 0 ^    ^byte 1 ^    ^byte 2 ^   <- dest index

            int b= ( ((int)(s.charAt(1))) & 0x1F );
            int c= ( ((int)(s.charAt(2))) & 0x1F );
            int d= ( ((int)(s.charAt(3))) & 0x1F );
            int e= ( ((int)(s.charAt(4))) & 0x1F );
            char f=0;

            int b0 = ( (b<<3) | (c>>2) );
            int b1 = ( ((c&0x3)<<6) | (d<<1) | (e>>4) );
            int b2 = ( ((e&0xF)<<4) | (f&0xF) );

            StringBuilder buf = new StringBuilder();
            buf.append("\$");
            buf.append(Integer.toHexString(b0));
            buf.append(",\$");
            buf.append(Integer.toHexString(b1));
            buf.append(",\$");
            buf.append(Integer.toHexString(b2));

            // Return the three bytes as comma-separated hex values
            return buf.toString();
        }

        public void writeWords(GroovyPrintWriter out)
        {
            int idx = 0;
            out.println("    .import DICT_BANK_"+getBank()+"_ID");
            out.println("    .segment \"DICT_BANK_"+getBank()+"\"");
            // This is the md_wordList->list, a big pile of md_wordInternal
            out.println("_words_"+name+"_"+first+":");
            out.println("    .export _words_"+name+"_"+first);
            for(Map.Entry<String,Integer> w : words)
            {
                String compressed = compress(w.getKey());
                out.println("    .byte "+compressed+" ; "+idx+" = "+w.getKey());
                //out.println("    .byte \""+compressed+"\", "+w.getValue()+" ; "+idx);
                ++idx;
            }
        }
    }

    public class Lexicon
    {
        protected String name;
        protected int priority=99;
        protected WordList [] wordLists;

        // The volume can be in many dictionaries
        // with a different multiplier used for priority when picking random words
        protected TreeMap<String,Integer> dictList = new TreeMap<>();
        
        Lexicon(String name,var idx,TreeMap<String,Integer> dicts,int p)
        {
            int lastslash = name.lastIndexOf("/");
            int dot = name.indexOf('.',lastslash);
            this.name = name.substring(lastslash+1,dot)+idx;
            wordLists = new WordList[26];
            for(int i=0;i<26;++i)
            {
                char id = (char)(65+i);
                wordLists[i] = new WordList(id,this.name);
            }
            this.dictList.putAll(dicts);
            priority=p;
        }

        public TreeMap<String,Integer> getDictMap()
        {
            return dictList;
        }
        public Collection<String> getDicts()
        {
            return dictList.keySet();
        }

        public int getDictCount(String dname)
        {
            return dictList.get(dname);
        }

        public String getName()
        {
            return name;
        }

        protected addDict(String d)
        {
            int k;
            String [] csv = d.split(",");
            String dname = csv[0];
            dictList.put(dname,1);
            for(int i=1;i<csv.length;i++)
            {
                if ( csv[i].startsWith('x') )
                {
                    int n = Integer.parseInt(csv[i].substring(1));
                    k = dictList.get(dname);
                    dictList.put(dname,k+n-1);  
                }
            }
        }

        public int getPriority()
        {
            return priority;
        }
        public void setPriority(String d,int offset)
        {
            priority=offset+Integer.parseInt(d);
        }

        public void store(String wordline)
        {
            int sp = wordline.indexOf(' ');
            if ( wordline.startsWith("@DICT ") )
            {
                String d = wordline.substring(sp).trim();
                addDict(d);
                return;
            }
            if ( wordline.startsWith("@PRIORITY ") )
            {
                String d = wordline.substring(sp).trim();
                setPriority(d,0);
                return;
            }
            storeWord(wordline);
        }
        protected int parseflags(String flaglist)
        {
            int ret=0;
            for(String flag : flaglist.split(","))
            {
                if ( "FOOD".equals(flag) ) ret|=1;
                if ( "CLOTHES".equals(flag) ) ret|=2;
            }
            return ret;
        }
        protected void storeWord(String wordline)
        {
            String w=wordline.trim().toUpperCase();
            if ( w.length() > 0 )
            {
                char id = w.charAt(0);
                int i = id-65;

                int comma = w.indexOf(',');
                if ( comma > -1 )
                {
                    // if the stuff after the comma matches the DFLAGS query,
                    // return the word so it can be split off into the DERIVED volume
                    wordLists[i].store(w.substring(0,comma).trim(),parseflags(w.substring(comma)));
                    countFreq(w.substring(0,comma).trim());
                }
                else
                {
                    wordLists[i].store(w,0);
                    countFreq(w);
                }
            }
        }


        public int size()
        {
            int ret=0;
            for(WordList wordList : wordLists)
            {
                ret += wordList.size();
            }
            return ret;
        }

        public void assignBanks(int [] banks)
        {
            for(WordList wordList : wordLists)
            {
                int n = wordList.size();
                if ( n > 0 )
                {
                    int bank = -1;
                    for(int i=0;i<5 && bank == -1; i++)
                    {
                        if ( banks[i] + n <= (8192-62)/3 )
                        {
                            // it fits here!
                            bank = i;
                        }
                    }
                    wordList.setBank(bank);
                    banks[bank] += n;
                }
            }
        }

        public void writeLexicon(GroovyPrintWriter out)
        {
            int validWordLists = 0;
            // This is the md_wordList->list, a big pile of md_wordInternal
            for(WordList wordList : wordLists)
            {
                int n = wordList.size();
                if ( n > 0 )
                {
                    ++validWordLists;
                    wordList.writeWords(out);
                }
                out.println();
            }

            // If there are more than 22 wordLists,
            // bump the number up to 26 and store this as a Sparse List
            if ( validWordLists > 22 )
            {
                validWordLists = 26;
            }

            // This is the md_lexicon, an array of 26 md_wordList
            out.println("    .segment \"DICT_IDX\"");
            out.println("_lex_"+name+":");
            out.println("    .export _lex_"+name);
            out.println();
            out.println("    .byte ${validWordLists} ; lexicon array length");
            out.println();
            for(WordList wordList : wordLists)
            {
                int n = wordList.size();
                if ( n == 0 )
                {
                    if (validWordLists == 26)
                    {
                        // This is a Sparse List, so we need to spit out a few blank rows
                        out.println("    .byte 0,0,\""+wordList.getFirst()+"\",0,0,0 ; "+wordList.getFirst());
                        out.println();
                    }
                }
                else
                {
                    out.println("    .word "+wordList.size()+" ; word count");
                    out.println("    .byte \""+wordList.getFirst()+"\"");
                    out.println("    .byte <DICT_BANK_"+wordList.getBank()+"_ID");
                    out.println("    .word _words_"+wordList.getName()+"_"+wordList.getFirst());
                    out.println();
                }
            }
        }
    }

    public class DerivedLexicon extends Lexicon
    {
        TreeSet<String> dflagList=new TreeSet<>();
        DerivedLexicon(String name,var idx,TreeMap<String,Integer> dicts,Collection<String> dflags,int p )
        {
            super(name,idx+"D",dicts,p);
            dflagList.addAll(dflags);
        }

        public Collection<String> getDFlags()
        {
            return dflagList;
        }

        public boolean query(String wordline)
        {
            int sp = wordline.indexOf(' ');
            // if this is a derived word flag, we want it
            if ( wordline.startsWith("@DICTD ") ) return true;
            if ( wordline.startsWith("@DFLAGS ") ) return true;
            if ( wordline.startsWith("@PRIORITY ") )
            {
                String d = wordline.substring(sp).trim();
                setPriority(d,1);
                return false;
            }

            // if dflags contains wordline's flags, we want it
            String w=wordline.trim().toUpperCase();
            if ( w.length() > 0 )
            {
                int comma = w.indexOf(',');
                if ( comma > -1 )
                {
                    for(String flag : w.split(","))
                    {
                        if (dflagList.contains(flag.trim()))
                        {
                            return true;
                        }
                    }
                }
            }

            // we don't want it
            return false;
        }
        public void store(String wordline)
        {
            int sp = wordline.indexOf(' ');
            if ( wordline.startsWith("@DICTD ") )
            {
                String d = wordline.substring(sp).trim();
                addDict(d);
                return;
            }
            if ( wordline.startsWith("@DFLAGS ") )
            {
                String d = wordline.substring(sp).trim().toUpperCase();
                dflagList.addAll(d.split(","));
System.err.println("DICTD flags = "+dflagList);
                return;
            }
            storeWord(wordline);
        }
    }

    private static class LexiconList
    {
        private Map<Integer,List<Lexicon>> priorityMap = new TreeMap<>();
    
        public void add(Lexicon v)
        {
            Integer p = v.getPriority();
            List<Lexicon> vl = priorityMap.get(p);
            if ( vl == null )
            {
                vl=new ArrayList<>();
                priorityMap.put(p,vl);
            }
            vl.add(v);
        }

        public List<Lexicon> getAllVolumes()
        {
            List<Lexicon> ret = new ArrayList<>();
            for( List<Lexicon> vl : priorityMap.values() )
            {
                ret.addAll(vl);
            }
            return ret;
        }
    }

    private LexiconList volumes = new LexiconList();

    public static void main(String [] args)
    {
        BuildDict m = new BuildDict();
        for(String fn : args)
        {
            // load will add one or more volumes to the dictionary
            m.load(fn);
        }
        m.writeDict();
    }

    public void load(String fn)
    {
        String vcount="";
        String dvcount="";
        TreeMap<String,Integer> emptyMap = new TreeMap<>();
        Lexicon v = new Lexicon(fn,vcount,emptyMap,99);
        DerivedLexicon dv = new DerivedLexicon(fn,dvcount,emptyMap,Collections.emptySet(),99);
        char [] buf = new char[5];
        try(BufferedReader in = new BufferedReader(new FileReader(fn)))
        {
            String s;
            while ( ( s = in.readLine() ) != null )
            {
                int semi=s.indexOf(';');
                String w = (semi>-1)?s.substring(0,semi):s;

                if (dv.query(w))
                {
                    dv.store(w);
                }
                else
                {
                    v.store(w);
                }
            }
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
        if ( v.size() > 0 )
        {
            volumes.add(v);
        }
        if ( dv.size() > 0 )
        {
            volumes.add(dv);
        }
    }

    public void writeDict()
    {
        // Pack the volumes best you can into five 1355-word (8KB) banks
        int [] banks = new int[5];
        banks[0]=10000; // skip bank 0
        // (Bank 0 is the copyright screen and .DATA; Bank 7 is .CODE etc)
        List<Lexicon> allVolumes = volumes.getAllVolumes();
        for(Lexicon v : allVolumes)
        {
            v.assignBanks(banks);
        }


        GroovyPrintWriter out = new GroovyPrintWriter("target/aqordlDict.s");

        out.println("; There are "+allVolumes.size()+" volumes.");
        for(Lexicon v : allVolumes)
        {
            out.println("; p="+v.getPriority()+" "+v.getName()+": "+v.size()+" words, "+v.size()*5+" bytes");
        }
        out.println();

        for(int i=1;i<4;i++)
        {
            out.println("; Bank "+i+" size: "+banks[i]+" words, 62+"+(banks[i]*3)+" bytes");
        }

        out.println();
        out.println("; Letter frequencies:");
        int totalLetters=0;
        for(int i=0;i<26;i++)
        {
            char c = ((char)'A')+(char)i;
            out.println("; "+c+" : "+freqs[i]);
            totalLetters+=freqs[i];
        }
        out.println("; Total:  "+totalLetters);
        out.println();
        // This .s file will be in target, hard to include stuff....
        out.println("    .include \"../src/main/include/version.inc\"");
        out.println();

        for(int i=1;i<4;i++)
        {
            out.println("    .segment \"DICT_BANK_C_"+i+"\"");
            out.println("    .byte copyright_dict");
            out.println();
        }
        out.println();
        out.close();

        for(Lexicon v : allVolumes)
        {
            out = new GroovyPrintWriter("target/LEX_${v.getName()}.s");
            v.writeLexicon(out);
            out.close();
        }

        // And now create the dictionaries

        Map<String,List<Lexicon>> dicts = new TreeMap<>();
        for(Lexicon v : allVolumes)
        {
            for(String s : v.getDicts())
            {
                List<Lexicon> vl = dicts.get(s);
                if ( vl == null )
                {
                    vl=new ArrayList<>();
                    dicts.put(s,vl);
                }
                vl.add(v);
            }
        }
        
        for(Map.Entry<String,List<Lexicon>> e: dicts)
        {
            Set<String> donevols=new HashSet<>();
            String n = e.getKey();
            out = new GroovyPrintWriter("target/${n}.s");
            out.println("    .segment \"DICT_IDX\"");
            List<Lexicon> vl = e.getValue();
            int wordcount=0;
            for(Lexicon v : vl)
            {
                if ( !donevols.contains(v.getName()))
                {
                    donevols.add(v.getName());
                    wordcount += v.size();
                    out.println("    .import _lex_"+v.getName());
                }
            }
            out.println("_"+n+":");
            out.println("    .export _"+n);
            out.println("    .byte "+vl.size()+" ; number of volumes");
            for(Lexicon v : vl)
            {
                out.println("    .byte "+v.getDictCount(n)+" ; multiplier");
                out.println("    .word _lex_"+v.getName());
            }
            out.println("; Word count: ${wordcount}");
            out.println();
            out.close();
        }


    }

}
