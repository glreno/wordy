import groovy.json.JsonSlurper

//
// Read the .asm file and generate one-up numbers for all the generated items
//

File hfile = new File(args[0]);
println("; Assembler source generated from $hfile");
int oneup=0;
hfile.eachLine { line ->
    if ( line.trim().startsWith(";") )
    {
        String stripped = line.trim().substring(1).trim();
        if ( stripped.startsWith("frame") )
        {
            def name=stripped.substring(6).trim();
            def semi=name.indexOf(';');
            if ( semi > 0 )
            {
                name=name.substring(0,semi).trim();
            }
            println(".export ${name}=${oneup}");
            ++oneup;
        }
    }
}

println();
