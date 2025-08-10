import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import groovy.json.JsonSlurper

this.bankids = new TreeSet();

String grey(int b)
{
    b=b&0xffffff;
    // This is the basic five-color colormap
    // (well, four; yellow requires inverting the char)
    if (b==0x837e86) { return "11"; }// light
    if (b==0x3e3b3f || b==0x9f8300) { return "01"; } // med grey, or yellow
    if (b==0x005b3c) { return "10"; } // green
    if ( b==0 ) { return "00"; } // black
    println("; Don't know what color "+Integer.toHexString(b)+" is");
    return "00";             // black for errors
}

void generatePic(Map map)
{
    File dir = new File("src/main/pic");
    File f = new File(dir,map.fn);
    BufferedImage im = ImageIO.read(f);
    name=map.name;
    bankName = "VOR_"; // TODO do not hardcode!
    picseg="PICS";
    idxseg="IDX";
    quot='\"';

    // Find source x,y (pixel coordinates)
//srcxy=args[2].split(",");
    srcx=Integer.valueOf(map.src[0]);
    srcy=Integer.valueOf(map.src[1]);

    // Find dest w,h (char coordinates) (max 6x8)
//dstwh=args[3].split("x");
    dstw=Integer.valueOf(map.size[0]);
    dsth=Integer.valueOf(map.size[1]);

    // Find dest x,y (char coordinate) (max 5,7)
//dstxy=args[4].split(",");
    dstx=Integer.valueOf(map.dst[0]);
    dsty=Integer.valueOf(map.dst[1]);

    // Calculate image w,h (pixel coordinates) (max 24x64)
    srcw=im.getWidth() - srcx;
    if ( srcw>dstw*4 ) srcw=dstw*4;
    srch=im.getHeight() - srcy;
    if ( srch>dsth*8 ) srch=dsth*8;

    println("    .segment $quot$bankName$picseg$map.bank$quot");
    println();
    int blockbytecount=0;
    int stripebytecount=0;
    for(x=0;x*4<srcw;++x)
    {
        for(y=0;y*8<srch;++y)
        {
            blockname="_${name}_stripe_${dstx+x}_block_${dsty+y}";
            println(blockname+":");
            println("    .export "+blockname);
            for(dy=0;dy<8;++dy)
            {
                String p1 = grey(im.getRGB(srcx+x*4,srcy+y*8+dy));
                String p2 = grey(im.getRGB(srcx+x*4+1,srcy+y*8+dy));
                String p3 = grey(im.getRGB(srcx+x*4+2,srcy+y*8+dy));
                String p4 = grey(im.getRGB(srcx+x*4+3,srcy+y*8+dy));
                println("    .byte %$p1$p2$p3$p4");
                ++blockbytecount;
            }
        }
    }

    println();

    stripeindexes = [
        [1:8],    // start char:number of chars
        [9:7,26:1],
        [27:5,65:3],
        [68:8],
        [76:4,90:4],
        [94:2,123:5,32:1]
    ];

    for(x=dstx;x<dstx+dstw;++x)
    {
        idx = stripeindexes[x]; // Map [start:nchar]
        buf="";
        y=0;
        skip=dsty;
        // expand the index into a byte array
        // but skip dsty characters -- so if dsty=2 and idx is [9:7,26:1]
        // append 11,5,26,1,0 -- add to start, subtract from nchar
        // (and if nchar is 0, skip this one!)
        idx.findAll { key, value ->
            start=key+skip;
            nchar=value-skip;
            if ( nchar <= 0 )
            {
                skip-=value;
            }
            else
            {
                skip=0;
                if ( y+nchar > dsth )
                {
                    nchar=dsth-y;
                }
                if ( nchar > 0 && y+nchar <= dsth )
                {
                    buf+="$start,$nchar,"
                    y+=nchar;
                    stripebytecount+=2;
                }
            }
        }
        buf+="0";
        stripebytecount+=1;

        println("_${name}_stripe_$x:");
        println("    .export _${name}_stripe_$x");
        println("    .word _${name}_stripe_${x}_block_${dsty}");
        stripebytecount+=2;
        println("    .byte ${buf}");
    }
    println("; Blocks: ${blockbytecount} bytes  Stripes: ${stripebytecount} bytes  ${name}");

    println();
    println("    .segment $quot$bankName$idxseg$quot");
    println();
    println("_${name}:");
    println("    .export _${name}");
    bankid = "${bankName}${picseg}${map.bank}_BANK_ID";
    println("    .byte <${bankid}");
    bankids.add(bankid);
    for(x=dstx;x<dstx+dstw;++x)
    {
        println("    .word _${name}_stripe_$x");
    }
    println("    .word 0");

}

//
// Read the .h file and produce all the image assembler
//

File hfile = new File(args[0]);
println("; Assembler source generated from $hfile");
hfile.eachLine { line ->
    if ( line.startsWith("//") )
    {
        String stripped = line.substring(2).trim();
        println("; $stripped");
        if ( stripped.startsWith("{") )
        {
            def map = new JsonSlurper().parseText(stripped)
/*
            println("; name:"+map.name);
            println("; fn:"+map.fn);
            println("; bank:"+map.bank);
            println("; srcx:"+map.src[0]);
            println("; srcy:"+map.src[1]);
            println("; width:"+map.size[0]);
            println("; height:"+map.size[1]);
            println("; dstx:"+map.dst[0]);
            println("; dsty:"+map.dst[1]);
*/
            generatePic(map);
        }
    }
}

println();
println("; Segments used:");
bankids.each( X -> println(".import $X") );
