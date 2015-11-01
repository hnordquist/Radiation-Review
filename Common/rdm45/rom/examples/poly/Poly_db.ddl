database poly_db 
{
   data file "poly_db.d01" contains _Display;
   data file "poly_db.d02" contains _Rectangle, _Circle, _Triangle;
   key  file "poly_db.k01" contains DisplayName;
   
   record _Display
   {
      key   char DisplayName[21];
   }
   
   record _Rectangle
   {
      short UpLfX;
      short UpLfY;
      short Height;
      short Width;
   }
   record _Circle
   {
      short CentX;
      short CentY;
      short Diameter;
   }
   record _Triangle
   {
      short Xa;
      short Ya;
      short Xb;
      short Yb;
      short Xc;
      short Yc;
   }

   set AllShapes
   {
      order last;
      owner _Display;
      member _Rectangle;
      member _Circle;
      member _Triangle;
   }
}
