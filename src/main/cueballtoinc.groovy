import groovy.json.JsonSlurper

//
// Read the .asm file and generate one-up numbers for all the generated items
//

File hfile = new File(args[0]);
println("; Assembler source generated from $hfile");
int oneup=0;
int measureid=0;
int tick=0;

hfile.eachLine { line ->

    if ( line.trim().startsWith(";") )
    {
        String stripped = line.trim().substring(1).trim();
        if ( stripped.startsWith("cue") )
        {
            def name=stripped.substring(4).trim();
            def semi=name.indexOf(';');
            if ( semi > 0 )
            {
                name=name.substring(0,semi).trim();
            }
            println(".export ${name}=${oneup}");
            ++oneup;
        }
    }
    else if ( line.trim().startsWith("measure") )
    {
        measureid=Integer.valueOf(line.substring(8).trim());
        println("; Start of measure ${measureid}");
        tick=0;
    }
    else if ( line.trim().startsWith("note") )
    {
        String stripped = line.trim().substring(5).trim();
        String [] margs = stripped.split(",");
        int duration=Integer.valueOf(margs[0].trim());
        println("; note ${margs[8]} duration ${duration}");
            // First param of a note is the duration of the note.
            // The measure is 16 ticks (i.e. a tick is a sixteenth note)
            // and duration is the notational size of the note.
            // So a duration of 8 is an 8th note, which lasts 16/8 = 2 ticks
            int nticks=16/duration;
            println(".export measure${measureid}_${tick}=${oneup}");
            ++oneup;
            ++tick;
            for(int i=1;i<nticks;++i)
            {
                println(".export measure${measureid}_${tick}=0");
                ++tick;
            }
    }
}

println();
