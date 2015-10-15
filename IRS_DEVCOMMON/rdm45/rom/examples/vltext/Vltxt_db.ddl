database vltxt_db
{
   data file [512]  "vltxt_db.d01" contains Document;
   data file [8192] "vltxt_db.d02" contains VLText_4096;
   data file [4096] "vltxt_db.d03" contains VLText_2048;
   data file [4096] "vltxt_db.d04" contains VLText_512;
   data file        "vltxt_db.d05" contains VLText_128;
   key  file        "vltxt_db.k01" contains Name;
   
   record Document
   {  
      key char Name[21];
   }
   
   record VLText_4096
   {  
      /* 4076 bytes data plus 20 bytes overhead */
      char bits_4096[4076][1];
   }
   
   record VLText_2048
   {  
      /* 2028 bytes data plus 20 bytes overhead */
      char bits_2048[2028][1];
   }
   
   record VLText_512
   {  
      /* 492 bytes data plus 20 bytes overhead */
      char bits_512[492][1];
   }

   record VLText_128
   {  
      /* 108 bytes data plus 20 bytes overhead */
      char bits_128[108][1];
   }

   /* notice that we are telling ddlp.exe that we are creating a VareLenText class */
   varilen set VLText_Set
   {       
      order last;
      owner Document;
      member VLText_4096;
      member VLText_2048;
      member VLText_512;
      member VLText_128;
   }
}
