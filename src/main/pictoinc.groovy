import groovy.json.JsonSlurper

//
// Read the .h file and produce all the image assembler
//

File hfile = new File(args[0]);
println("; Assembler source generated from $hfile");
hfile.eachLine { line ->
    if ( line.startsWith("//") )
    {
        String stripped = line.substring(2).trim();
        if ( stripped.startsWith("{") )
        {
            def map = new JsonSlurper().parseText(stripped)
            println(".import _${map.name}");
        }
    }
}

println();
