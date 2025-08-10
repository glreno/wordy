import groovy.json.JsonSlurper

this.index=0;
this.loopIndexes=new TreeMap<Integer,String>();
this.loopNames=new TreeMap<String,Integer>();
this.measureid=0;
this.measuretick=0;

void generateLoopStart(String nameline)
{
    println();
    int semi=nameline.indexOf(';');
    int comment=nameline.indexOf("//");
    if ( semi < 0 ) return;
    String name=nameline.substring(0,semi);
    String desc=(comment>0)?nameline.substring(comment).trim():"";
    println("; Animation loop ${index}: ${name} ${desc}");
    loopIndexes.put(index,name);
    loopNames.put(name,index);
}
void generatePic(Map map)
{
    int flags=0;
    String desc="";
    if ( map.measure != null )
    {
        // Starting a new measure of music!
        desc+=" (start measure "+map.measure+") "
        measureid=map.measure;
        measuretick=0;
    }
    if ( map.sound == null )
    {
        map.sound=0;
        if ( measureid != 0 )
        {
            map.sound="measure"+measureid+"_"+measuretick;
            ++measuretick;
        }
    }
    if ( map.t != null )
    {
        desc+="t:"+map.t;
    }
    if ( map.picname != null )
    {
        desc+=map.picname;
    }
    if ( "true".equals(map.exit) )
    {
        desc+=" (exit allowed)";
        flags |= 1;
    }
    if ( "debug".equals(map.exit) )
    {
        if ( false ) // set to TRUE to have an infinite loop
        {
            desc+=" (debug mode - no exit)";
        }
        else
        {
            desc+=" (exit allowed)";
            flags |= 1;
        }
    }
    if ( "countdown".equals(map.exit) )
    {
        desc+=" (exit after 3 loops)";
        flags |= 2;
    }
    if ( map.lyric != null )
    {
        desc+=" lyric=";
        desc+=map.lyric;
    }
    int next = index+1;
    if ( map.next != null )
    {
        // println("; Index ${index} looking for ${map.next}");
        if ( "self".equals(map.next) )
        {
            next = index;
            // println("; found self ${next}");
        }
        else if ( this.loopNames.containsKey(map.next) )
        {
            next = this.loopNames.get(map.next);
            // println("; found ${next}");
        }
        else
        {
            // println("; must be a forward reference");
        }
    }
    println();
    println(".word ${next} ; idx ${index} - next=${next} pic=${map.pic} ${desc}");
    println(".byte ${flags},${map.pic},${map.sound}");

    ++index;
}

//
// Read the .h file and produce all the image assembler
//

File hfile = new File(args[0]);
println("; Assembler source generated from $hfile");
println();
println("    .include \"robot0_frame.inc\"");
println("    .include \"robot0_cueball.inc\"");
println();
println("    .segment \"VOR_IDX\"");
println();
println("_vor_animFrameQueue:");
println("    .export _vor_animFrameQueue");
println("    ; The game will start at index 0. This will be the intro animation,");
println("    ; which will branch someplace else.");
println("    ; bytes: next,flag,picid");

hfile.eachLine { line ->
    if ( line.startsWith("extern int ") )
    {
        //println("; $line");
        generateLoopStart(line.substring(10).trim());
    }
    else if ( line.startsWith("//") )
    {
        String stripped = line.substring(2).trim();
        //println("; $stripped");
        if ( stripped.startsWith("{") )
        {
            def map = new JsonSlurper().parseText(stripped)
/*
            println("; pic:"+map.pic);
            println("; picname:"+map.picname);
            println("; exit:"+map.exit);
            println("; next:"+map.next);
*/
            generatePic(map);
        }
    }
}

// Generate the animation loop indexes
println();
println(";");
println("; Animation loop start points:");
println(";");
println();
loopIndexes.each{ i,n ->
    {
        println( ".export _${n}");
        println( "        _${n}: .word ${i}");
    }
}
