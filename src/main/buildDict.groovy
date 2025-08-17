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

    public static class Volume
    {
        protected String name;
        protected int bank=-1;
        protected int priority=99;
        protected Map<String,Integer> wordList = new TreeMap<>();

        // The volume can be in many dictionaries
        // (possibly multple times, and in a priority order. Eventually)
        protected Set<String> dictList = new TreeSet<>();
        protected List<String> dupDictList = new ArrayList<>();
        
        Volume(String name,var idx,Collection<String> dicts,List<String> dupDicts,int p)
        {
            int lastslash = name.lastIndexOf("/");
            int dot = name.indexOf('.',lastslash);
            this.name = name.substring(lastslash+1,dot)+idx;
            this.dictList.addAll(dicts);
            this.dupDictList.addAll(dupDicts);
            priority=p;
        }

        public Collection<String> getDicts()
        {
            return dictList;
        }

        public List<String> getDupDicts()
        {
            return dupDictList;
        }

        public String getName()
        {
            return name;
        }

        protected addDict(String d)
        {
            String [] csv = d.split(",");
            String dname = csv[0];
            dictList.add(dname);
            for(int i=1;i<csv.length;i++)
            {
                if ( csv[i].startsWith('x') )
                {
                    int n = Integer.parseInt(csv[i].substring(1));
                    // store in duplicate list n-1 times
                    for(int j=1;j<n;j++) 
                    {
                        dupDictList.add(dname);
                    }
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
                int comma = w.indexOf(',');
                if ( comma > -1 )
                {
                    // if the stuff after the comma matches the DFLAGS query,
                    // return the word so it can be split off into the DERIVED volume
                    wordList.put(w.substring(0,comma).trim(),parseflags(w.substring(comma)));
                }
                else
                {
                    wordList.put(w,0);
                }
            }
        }


        public int size()
        {
            return wordList.size();
        }

        public void setBank(int b)
        {
            this.bank = b;
        }

        public int getBank()
        {
            return bank;
        }

        public void write(GroovyPrintWriter out)
        {
            int idx = 0;
            out.println("    .import DICT_BANK_"+bank+"_ID");
            out.println("    .segment \"DICT_BANK_"+bank+"\"");
            out.println("_words_"+name+":");
            out.println("    .export _words_"+name);
            for(Map.Entry<String,Integer> w : wordList)
            {
                out.println("    .byte \""+w.getKey()+"\" ; "+idx);
                //out.println("    .byte \""+w.getKey()+"\", "+w.getValue()+" ; "+idx);
                ++idx;
            }
            out.println("    .RODATA");
            out.println("_vol_"+name+":");
            out.println("    .export _vol_"+name);
            out.println("    .word "+size()+" ; word count");
            out.println("    .byte <DICT_BANK_"+bank+"_ID ; bank ID");
            out.println("    .word _words_"+name);
            out.println();
        }
    }

    public static class DerivedVolume extends Volume
    {
        TreeSet<String> dflagList=new TreeSet<>();
        DerivedVolume(String name,var idx,Collection<String> dicts,List<String> dupDicts,Collection<String> dflags,int p )
        {
            super(name,idx+"D",dicts,dupDicts,p);
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

    private static class VolumeList
    {
        private Map<Integer,List<Volume>> priorityMap = new TreeMap<>();
    
        public void add(Volume v)
        {
            Integer p = v.getPriority();
            List<Volume> vl = priorityMap.get(p);
            if ( vl == null )
            {
                vl=new ArrayList<>();
                priorityMap.put(p,vl);
            }
            vl.add(v);
        }

        public List<Volume> getAllVolumes()
        {
            List<Volume> ret = new ArrayList<>();
            for( List<Volume> vl : priorityMap.values() )
            {
                ret.addAll(vl);
            }
            return ret;
        }
    }

    private VolumeList volumes = new VolumeList();

    public static void main(String [] args)
    {
        BuildDict m = new BuildDict();
        for(String fn : args)
        {
            // load will add one or more volumes to the dictionary
            m.load(fn);
        }
        m.write();
    }

    public void load(String fn)
    {
        Integer vcount=1;
        Integer dvcount=1;
        Volume v = new Volume(fn,vcount,Collections.emptySet(),Collections.emptyList(),99);
        DerivedVolume dv = new DerivedVolume(fn,dvcount,Collections.emptySet(),Collections.emptyList(),Collections.emptySet(),99);
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
                if ( v.size() >= 1355 )
                {
                    volumes.add(v);
                    ++vcount;
                    v=new Volume(fn,vcount,v.getDicts(),v.getDupDicts(),v.getPriority());
                }
                if ( dv.size() >= 607 )
                {
                    volumes.add(dv);
                    ++dvcount;
                    dv=new DerivedVolume(fn,dvcount,dv.getDicts(),dv.getDupDicts(),dv.getDFlags(),dv.getPriority());
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

    public void write()
    {
        // Pack the volumes best you can into five 1355-word (8KB) banks
        int [] banks = new int[5];
        banks[0]=1366; // skip bank 0
        // (Bank 0 is the copyright screen and .DATA; Bank 7 is .CODE etc)
        List<Volume> allVolumes = volumes.getAllVolumes();
        for(Volume v : allVolumes)
        {
            int n = v.size();
            int bank = -1;
            for(int i=0;i<5 && bank == -1; i++)
            {
                if ( banks[i] + n <= 1355 )
                {
                    // it fits here!
                    bank = i;
                }
            }
            v.setBank(bank);
            banks[bank] += n;
        }

        GroovyPrintWriter out = new GroovyPrintWriter("target/aqordlDict.s");

        out.println("; There are "+allVolumes.size()+" volumes.");
        for(Volume v : allVolumes)
        {
            out.println("; p="+v.getPriority()+" "+v.getName()+": "+v.size()+" words, "+v.size()*5+" bytes, bank "+v.getBank());
        }
        out.println();

        for(int i=1;i<5;i++)
        {
            out.println("; Bank "+i+" size: "+banks[i]+" words, "+(62+banks[i]*5)+" bytes");
        }
        out.println();
        // This .s file will be in target, hard to include stuff....
        out.println("    .include \"../src/main/include/version.inc\"");
        out.println();

        for(int i=1;i<5;i++)
        {
            out.println("    .import DICT_BANK_"+i+"_ID");
            out.println("    .segment \"DICT_BANK_"+i+"\"");
            out.println("    .byte copyright_dict");
            out.println("    .byte \"Bk"+i+"\"");
            out.println();
        }
        out.println();
        out.close();

        for(Volume v : allVolumes)
        {
            out = new GroovyPrintWriter("target/VOL_${v.getName()}.s");
            v.write(out);
            out.close();
        }

        // And now create the dictionaries

        Map<String,List<Volume>> dicts = new TreeMap<>();
        for(Volume v : allVolumes)
        {
            for(String s : v.getDicts())
            {
                List<Volume> vl = dicts.get(s);
                if ( vl == null )
                {
                    vl=new ArrayList<>();
                    dicts.put(s,vl);
                }
                vl.add(v);
            }
        }
        for(Volume v : allVolumes)
        {
            for(String s : v.getDupDicts())
            {
                List<Volume> vl = dicts.get(s);
                vl.add(v);
            }
        }
        
        for(Map.Entry<String,List<Volume>> e: dicts)
        {
            Set<String> donevols=new HashSet<>();
            String n = e.getKey();
            out = new GroovyPrintWriter("target/${n}.s");
            out.println("    .RODATA");
            List<Volume> vl = e.getValue();
            out.println("_"+n+":");
            out.println("    .export _"+n);
            out.println("    .word "+vl.size()+" ; number of volumes");
            int wordcount=0;
            for(Volume v : vl)
            {
                out.println("    .import _vol_"+v.getName());
                out.println("    .word _vol_"+v.getName());
                if ( !donevols.contains(v.getName()))
                {
                    donevols.add(v.getName());
                    wordcount += v.size();
                }
            }
            out.println("; Word count: ${wordcount}");
            out.println();
            out.close();
        }


    }

}
