import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

String grey(int b)
{
    if (b>170) return "11";
    if (b>85) return "01";
    return "00";
}

File f = new File(args[0]);
BufferedImage im = ImageIO.read(f);
name=args[1];

// Find source x,y (pixel coordinates)
srcxy=args[2].split(",");
srcx=Integer.valueOf(srcxy[0]);
srcy=Integer.valueOf(srcxy[1]);

// Image w,h (pixel coordinates) (max 24x64)
w=im.getWidth() - srcx;
if ( w>24 ) w=24;
h=im.getHeight() - srcy;
if ( h>64 ) h=64;
println("; arg2=$srcxy ; srcxy=$srcx,$srcy ; w,h=$w $h");

println("    .segment \"VOR_BANK\"");
println();
for(x=0;x*4<w;++x)
{
    for(y=0;y*8<h;++y)
    {
        blockname="_${name}_stripe_${x}_block_${y}";
        println(blockname+":");
        println("    .export "+blockname);
        for(dy=0;dy<8;++dy)
        {
            b1 = im.getRGB(srcx+x*4,srcy+y*8+dy)&0xff;
            b2 = im.getRGB(srcx+x*4+1,srcy+y*8+dy)&0xff;
            b3 = im.getRGB(srcx+x*4+2,srcy+y*8+dy)&0xff;
            b4 = im.getRGB(srcx+x*4+3,srcy+y*8+dy)&0xff;
            println("    .byte %"+grey(b1)+grey(b2)+grey(b3)+grey(b4));
        }
    }
}

println();

stripeindexes = [
    "1,8,0",    // start char, number of chars, 0 to end the stripe
    "9,7,26,1,0",
    "27,5,65,3,0",
    "68,8,0",
    "76,4,90,4,0",
    "94,2,123,5,32,1,0"
];

for(x=0;x<6;++x)
{
    println("_${name}_stripe_$x:");
    println("    .export _${name}_stripe_$x");
    println("    .word _${name}_stripe_${x}_block_0");
    println("    .byte ${stripeindexes[x]}");
}

println();
println("_${name}_pic:");
println("    .export _${name}_pic");
for(x=0;x<6;++x)
{
    println("    .word _${name}_stripe_$x");
}
println("    .word 0");
