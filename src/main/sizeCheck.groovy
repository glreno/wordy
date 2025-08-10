
def bankFor =
    [
        "COPYRIGHT": 0,
        "TRAMPOLINED": 0,
        "CUEBALL": 0,
        "VOR_PICS0": 0,
        "CART_BANK_0": 0,

        "DICT_BANK_1": 1,
        "VOR_PICS1": 1,
        "CART_BANK_1": 1,

        "DICT_BANK_2": 2,
        "VOR_PICS2": 2,
        "CART_BANK_2": 2,

        "DICT_BANK_3": 3,
        "VOR_PICS3": 3,
        "CART_BANK_3": 3,

        "DICT_BANK_4": 4,
        "VOR_PICS4": 4,
        "CART_BANK_4": 4,

        "VOT_BANK": 5,
        "VOR_PICS5": 5,
        "VOR_VIEW": 5,
        "VOR_IDX": 5,
        "VOR_CODE": 5,
        "CART_BANK_5": 5,

        "TITLEPAGE": 6,
        "STARTUP": 6,
        "ONCE": 6,
        "GAMEINIT": 6,
        "FONTDATA": 6,
        "DATA": [6,8],
        "RODATA": [6,8],
        "VOR_PICS6": 6,
        "CART_BANK_6": 6,

        "COPYRIGHT_ROM": 7,
        "LOWCODE": 7,
        "CODE": 7,
        "TRAMPOLINES": 7,
        "APP": 7,
        "DA": 7,
        "DS": 7,
        "MD": 7,
        "VIEW": 7,
        "VOR_PICS7": 7,
        "TITLECARTHDR": 7,
    ];

List [] banks=new List[9];
for(int i=0;i<9;++i)
    banks[i]=new ArrayList();

int state=0;
System.in.eachLine { line ->
    if ( state==2) state=3;
    if ( line.equals("Segment list:") ) state=1;
    if ( line.equals("") ) state=0;
    if ( state==1 && line.equals("----------------------------------------------------") ) state=2;

    if ( state==3 )
    {
        name=line.split(' ')[0];
        var id = bankFor[name];
        if ( id==null ) id=8;
        if ( id instanceof List )
        {
            for(int x : id)
            {
                banks[x].add(line);
            }
        }
        else
        {
            banks[id].add(line);
        }
    }
}

for(int i=0;i<9;++i) {
    System.out.println("----   Bank $i -------");
    System.out.println("Name                   Start     End    Size  Align");
    for(String s : banks[i]) System.out.println(s);
}
