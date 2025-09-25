import java.util.zip.*;

try {
    // Encode a String into bytes
    String inputString = "one fish two fish red fish blue fish";
    byte[] input = inputString.getBytes("UTF-8");

    // Compress the bytes
    byte[] output = new byte[100];
    Deflater compresser = new Deflater(Deflater.DEFAULT_COMPRESSION,true);
    compresser.setInput(input);
    compresser.finish();
    int compressedDataLength = compresser.deflate(output);
    compresser.end();

    // Decompress the bytes
    Inflater decompresser = new Inflater(true);
    decompresser.setInput(output, 0, compressedDataLength);
    byte[] result = new byte[100];
    int resultLength = decompresser.inflate(result);
    decompresser.end();

    // Decode the bytes into a String
    String outputString = new String(result, 0, resultLength, "UTF-8");
    System.out.println("//" + outputString);
    System.out.println("// Source length: ${input.length}  Compressed: ${compressedDataLength}");
    System.out.println("const unsigned char compressed [] = {");
    for(int i=0;i<compressedDataLength;++i)
    {
        System.out.print("0x"+Integer.toHexString(output[i]&0xff));
        System.out.print(", ");
    }
    System.out.println();
    System.out.println("};");
    System.out.println("const int compressedLen=${compressedDataLength};");
} catch(java.io.UnsupportedEncodingException e) {
    e.printStackTrace();
} catch (java.util.zip.DataFormatException e) {
    e.printStackTrace();
}
