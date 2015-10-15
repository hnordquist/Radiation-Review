// This example illustrates the creation and use of
// a Multi Membered Set

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "polymrph.hpp"

void RunProgram();

DB_INIT(poly_db);           // Initialize database locator

PolyTask *pTask = 0;


main()
{
	pTask = new PolyTask("o", ".\\");
   if (pTask && pTask->Okay())
      RunProgram();
   if (pTask)
      delete pTask;
   return 0;
}


void load_database()
{
   // make sure that there is something in the database
   //  to begin with.
   Display display;
   
   display.PreInit();
}

// The display_all() shows how to use the polymorphic capabilities
//  to iterate thru all members of the multi-member set using the
//  Polymorph-based AllShapes class.  The AllShapes::Show()
//  gets a pointer to the correct member type and calls it's Show().

void display_all()
{
   short      ct = 0;
   Display    display;
   AllShapes  shapes;
   
   GroupLock  L('r', &display, POMNULL);
   L.AddRead(SetObj(ALLSHAPES), shapes);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }

   // navigate thru all display objects sequentially
   for (display[FIRST]; display.Okay(); display++)
   {
      // navigate from display object to shape object 
      //  thru multi-member set and display the shape
      //  that was found.
      for (display >> shapes; shapes.Okay(); shapes++, ct++)
      {
         // call virtual Show that will call the correct
         //  Show function polymorphically
         shapes.Show();
      }
   }

   printf("\nTotal Shapes Displayed: %d\n\n", ct);
}


// The display_triangles() (as well as rect and circ) demonstrates
//  new functionality for ROM 3.0.  You are now able to iterate over
//  a multi-member set with one of the member-type objects and it
//  will only load those member objects that match it's type.  In 
//  previous versions of ROM, you would have had to load each member
//  and then determine if it was a type you wanted to display.

void display_triangles()
{             
   short      ct = 0;
   Display    display;
   Triangle   triangle(display);

   GroupLock  L('r', &display, &triangle, POMNULL);
   L.AddRead(SetObj(ALLSHAPES), triangle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }

   // navigate thru all display objects sequentially
   for (display[FIRST]; display.Okay(); display++)
   {
      // navigate from display object to shape object 
      //  of type Triangle thru multi-member set and 
      //  display the triangle.
      // this navigation actually skips the other members
      //  of the multi-member set that are not triangles
      for (display >> triangle; triangle.Okay(); triangle++, ct++)
         triangle.Show();
   }

   printf("\nTotal Triangles Displayed: %d\n\n", ct);
}             


void display_rectangles()
{             
   short       ct = 0;
   Display     display;
   Rectangl    rectangle(display);

   GroupLock   L('r', &display, &rectangle, POMNULL);
   L.AddRead(SetObj(ALLSHAPES), rectangle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }

   for (display[FIRST]; display.Okay(); display++)
   {
      for (display >> rectangle; rectangle.Okay(); rectangle++, ct++)
         rectangle.Show();
   }

   printf("\nTotal Rectangles Displayed: %d\n\n", ct);
}


void display_circles()
{
   short     ct = 0;
   Display   display;
   Circle    circle(display);

   GroupLock   L('r', &display, &circle, POMNULL);
   L.AddRead(SetObj(ALLSHAPES), circle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }

   for (display[FIRST]; display.Okay(); display++)
   {
      for (display >> circle; circle.Okay(); circle++, ct++)
         circle.Show();
   }

   printf("\nTotal Circles Displayed: %d\n\n", ct);
}


// The new_shape() demonstrates what I (the current author) consider
//  to be a non-intuitive way of adding member objects to a multi-member
//  set.  Thru the use of the Polymorph-based AllShapes class, you
//  can Select() which member type to make current and use it to create
//  your new member object.  This was the method demonstrated in previous
//  versions of ROM.  I present a different (preferable) method in the 
//  new_triangle(), new_rectangle() and new_circle().

void new_shape()
{
   Display     display;
   AllShapes   shapes;

   // because of the Transaction and M/U locking, this code has been moved
   //  into the AllShapes::UserNew() function further down in this file.
   shapes.UserNew(display);
}


// The new_triangle() demonstrates () preferred method of inserting
//  member objects into a multi-member set.  Since we know what type
//  of object we will be creating, simply create it explicitly.  There
//  isn't any need to use the Polymorph-based AllShapes class to do this
//  task.
void new_triangle()
{
   Display     display;
   Triangle  triangle;

   // get the new triangle information
   triangle.UserNew();

   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(SetObj(ALLSHAPES), triangle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   // make sure we have a display object in the database and 
   //  navigate to the first one (for simplicity)
   display[FIRST];
   if (!display.Okay())
      return;

   // specifically create a new Triangle
   triangle.NewObj();

   // connect this triangle to the polymorphic set
   triangle.Connect(display);
}


void new_rectangle()
{
   Display     display;
   Rectangl    rectangle;

   rectangle.UserNew();
   
   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(SetObj(ALLSHAPES), rectangle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   display[FIRST];
   if (!display.Okay())
      return;

   // same as NewObj, Connect
   rectangle.NewMemberOf(display);
}


void new_circle()
{
   Display     display;
   Circle    circle;

   circle.UserNew();
   
   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(SetObj(ALLSHAPES), circle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   display[FIRST];
   if (!display.Okay())
      return;

   circle.NewObj();
   circle.Connect(display);
}


// The delete_all() shows how the Polymorph-bases AllShapes class is
//  more or less a vessel to iterate thru all the members of a multi-
//  member set.  Because we don't want to do any functions for any
//  member type in particular, we can simply run thru all members deleting
//  them.
void delete_all()
{
   short       ct = 0;
   Display     display;
   AllShapes   shapes;

   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(shapes);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   // navigate thru all display objects sequentially
   for (display[FIRST]; display.Okay(); display++)
   {
      // navigate from display object to a shape object 
      //  thru multi-member set and delete the shape.
      for (display >> shapes; shapes.Okay(); shapes++, ct++)
      {
         shapes.DisConnect(display);
         shapes.Delete();
      }
   }

   printf("\nTotal Shapes Deleted: %d\n\n", ct);
}


// The delete_triangle() demonstrates new functionality in ROM 3.0
//  that allows you to iterate over member objects of your specified
//  type.  By using the specified object type, you will only access those
//  member objects that match the type, thus allowing you to simply delete
//  all Triangles.
void delete_triangles()
{
   short     ct = 0;
   Display   display;
   Triangle  triangle(display);

   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(SetObj(ALLSHAPES), triangle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   // navigate thru all display objects sequentially
   for (display[FIRST]; display.Okay(); display++)
   {
      // navigate from display object to shape object 
      //  of type Triangle thru multi-member set and 
      //  delete the triangle.
      // this navigation actually skips the other members
      //  of the multi-member set that are not triangles
      for (display >> triangle; triangle.Okay(); triangle++, ct++)
      {
         triangle.DisConnect(display);
         triangle.Delete();
      }
   }

   printf("\nTotal Triangles Deleted: %d\n\n", ct);
}


void delete_rectangles()
{
   short         ct = 0;
   Display       display;
   Rectangl      rectangle(display);

   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(SetObj(ALLSHAPES), rectangle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   for (display[FIRST]; display.Okay(); display++)
   {
      for (display >> rectangle; rectangle.Okay(); rectangle++, ct++)
      {
         rectangle.DisConnect(display);
         rectangle.Delete();
      }
   }

   printf("\nTotal Rectangles Deleted: %d\n\n", ct);
}


void delete_circles()
{
   short      ct = 0;
   Display    display;
   Circle   circle(display);

   TransAction T("new shape", pTask);
   GroupLock   L;
   L.AddRead(display);
   L.AddWrite(SetObj(ALLSHAPES), circle);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      return;
   }
   
   for (display[FIRST]; display.Okay(); display++)
   {
      for (display >> circle; circle.Okay(); circle++, ct++)
      {
         circle.DisConnect(display);
         circle.Delete();
      }
   }

   printf("\nTotal Circles Deleted: %d\n\n", ct);
}

void loop_operation()
{
   short ct;
   for (ct = 0; ct < 10; ct++) {
       display_all();
       display_rectangles();
       display_circles();
       delete_triangles();   
       delete_rectangles();  
       delete_all();         
       load_database();      
   }
}

void RunProgram()
{
   char cmd[20];  /* command entry string */
   int quit = 0;

   // make sure we have objects in the database
   load_database();

   while (!quit) 
   {  
      printf("\nPolymorph Demo Commands:\n");
      printf("   a - Display all shapes\n");
      printf("   b - Display all Triangles\n");
      printf("   c - Display all Rectangles\n");
      printf("   d - Display all Circles\n");
      printf("   e - Enter new Shape\n");
      printf("   f - Enter new Triangle\n");
      printf("   g - Enter new Rectangle\n");
      printf("   h - Enter new Circle\n");
      printf("   j - Delete all Shapes\n");
      printf("   k - Delete all Triangles\n");
      printf("   l - Delete all Rectangles\n");
      printf("   m - Delete all Circles\n");
      printf("   n - Load Database with Shapes\n");
      printf("   o - Loop thru all operations\n");
      printf("   q - Quit\n");

      printf("enter command: ");
      gets(cmd);

      switch (cmd[0])
      {
         case  'a': display_all();        break;
         case  'b': display_triangles();  break;
         case  'c': display_rectangles(); break;
         case  'd': display_circles();    break;
         case  'e': new_shape();          break;
         case  'f': new_triangle();       break;
         case  'g': new_rectangle();      break;
         case  'h': new_circle();         break;
         case  'j': delete_all();         break;
         case  'k': delete_triangles();   break;
         case  'l': delete_rectangles();  break;
         case  'm': delete_circles();     break;
         case  'n': load_database();      break;
         case  'o': loop_operation();     break;
         case  'q': 
         case  'Q': quit = 1;             break;
         default:
            printf("*** incorrect command -- re-enter\n");
      }
   }
}



//-----------------------------------------------------
// class functions
//-----------------------------------------------------

    void
  AllShapes :: 
UserNew(const RDisplay display)
{
   char         str[10];
   REC_TYPE     rectype = 0;
   T_F          rtn = False;

   printf ("Which Shape would you like to create?\n\tt) Triangle\n\tc) Circle\n\tr) Rectangle\n\tq) Quit\n>>");
   gets(str);

   switch (*str)
   {
      case 't':   rectype = _TRIANGLE;  break;
      case 'c':   rectype = _CIRCLE;    break;
      case 'r':   rectype = _RECTANGLE; break;
      case 'q':                         break;
      default :   printf("Invalid option\n");  break;
   }

   if (rectype)
   {
      // select into 'this' the correct member object type to use
      //  for creating a new member object
      Select(rectype);

      // call the UserNew() polymorphically for the currently 
      //  Select()ed member object type
      GetShape()->UserNew();

      TransAction T("new shape", pTask);
      GroupLock   L;
      L.AddRead(display);
      L.AddWrite(*this);
      if (L.Lock() != S_OKAY) {
        printf("Could not get a lock.\n");
        return;
      }

      // make sure we have a display object in the database and 
      //  navigate to the first one (for simplicity)
      display[FIRST];
      if (!display.Okay())
         return;

      // Create a new member object
      GetShape()->NewObj();
   
      // Using this method for creating a new member-object, you must
      //  always call the Sync() if you are going to continue using
      //  the shape object.  Sync() sets the shape objects 
      //  database address and status equal to whatever the currently
      //  Select()ed member-object's database address and status are.
      Sync();

      // The Sync() function has set 'this' database address and
      //  status equal to the current member-objects database address
      //  and status.  So, we are able to do a Connect() with the 
      //  shapes object.  (Very non-intuitive, I think).
      Connect(display);
      
      // Did everyone follow that?  If not, look at new_triangle for 
      //  a simpler approach!
   }
}


//-----------------------------------------------------
// Triangle class functions
//-----------------------------------------------------
static char tnames[] = { "XaYaXbYbXcYc" };

    void
  Triangle :: 
UserNew()
{
   short *pi = &Xa;
   char temp[20];
   
   printf("Enter new Triangle coordinates\n");
   for (char *p=tnames; *p != 0; p+=2, pi++)
   {
      printf("\t%2.2s: ",p);
      gets(temp);
      *pi = (short)atoi(temp);
   }
}


    void
  Triangle ::
Show() 
{ 
   printf("I'm a Triangle,  coordinates are:\n");
   printf("   Xa,Ya: %d,%d\n", Xa, Ya);
   printf("   Xb,Yb: %d,%d\n", Xb, Yb);
   printf("   Xc,Yc: %d,%d\n\n", Xc, Yc);
}


    void 
  Triangle :: 
NewTri(short xa,short ya,short xb, short yb, short xc, short yc) 
{
   Xa=xa; 
   Ya=ya; 
   Xb=xb; 
   Yb=yb; 
   Xc=xc; 
   Yc=yc; 
   NewObj();
}

    
//-----------------------------------------------------
// Circle class functions
//-----------------------------------------------------
static char cnames[] = { "XYD" };

    void
  Circle ::
UserNew()
{
   char temp[20];
   short *pi= &CentX;
   
   printf("Enter new Circle coordinates\n");
   for (char *p=cnames; *p != 0; p++, pi++)
   {
      printf("\t%1.1s: ",p);
      gets(temp);
      *pi= (short)atoi(temp);
   }
}


    void 
  Circle ::
NewCircle(short x,short y,short d) 
{
   CentX = x; 
   CentY = y;
   Diameter = d; 
   NewObj(); 
}


    void
  Circle ::
Show() 
{   
   printf("I'm a Circle,  coordinates are:\n");
   printf("   Center X,Y: %d,%d\n", CentX, CentY);
   printf("     Diameter: %d\n\n", Diameter);
}


//-----------------------------------------------------
// Rectangle class functions
//-----------------------------------------------------
static char rnames[] = { "XYHW" };

    void
  Rectangl :: 
UserNew()
{ 
   char temp[20];
   short *pi= &UpLfX;
   
   printf("Enter new Rectangle coordinates\n");
   for (char *p=rnames; *p != 0; p++, pi++)
   {
      printf("\t%1.1s: ",p);
      gets(temp);
      *pi= (short)atoi(temp);
   }
}

    void
  Rectangl ::
NewRect(short x,short y,short h, short w )
{
   UpLfX = x; 
   UpLfY = y; 
   Height = h; 
   Width = w; 
   NewObj();
}


    void
  Rectangl ::
Show() 
{ 
   printf("I'm a Rectangle,  coordinates are:\n");
   printf("   Upper Left X,Y: %d,%d\n", UpLfX, UpLfY);
   printf("           Height: %d\n", Height);
   printf("            Width: %d\n\n", Width);
}


    void
  Display ::
PreInit()
{
   TransAction T("preinit", pTask);
   GroupLock   L('w', *this, ALLSHAPES, POMNULL);
   if (L.Lock() != S_OKAY) {
      printf("Could not get a lock.\n");
      exit(1);
   }
   
   (*this)[FIRST];
   if (!Okay())
   {
      // create a new Display object (owner object of multi-member set)
      NewObj();
   }

   Triangle triangle;
   *this >> triangle;
   if (!triangle.Okay())
   {
      short tx, ty, g=50;

      /*for (tx=100,ty=300; tx < 300; tx+=3,ty -= 2, g++)
      {
         triangle.NewTri(tx, ty, tx+g, ty+g, tx-g, ty+g);
         triangle.Connect(*this);
      }	*/
	  for (tx=100,ty=300; tx < 102; tx+=3,ty -= 2, g++)
      {
         triangle.NewTri(tx, ty, tx+g, ty+g, tx-g, ty+g);
         triangle.Connect(*this);
      }	
   }

   
   Circle circle;
   *this >> circle;
   if (!circle.Okay())
   {
      //for (short q = 10; q < 250; q+=4)
	  for (short q = 10; q < 14; q+=4)
      {
         circle.NewCircle(q, q, q);
         circle.Connect(*this);
      }
   }

   Rectangl rectangle;
   *this >> rectangle;
   if (!rectangle.Okay())
   {
      for (short q=400; q > 0; q -= 10)
      {
         short xx, yy;

         xx = 400 - q;
         yy = 400 - q;
         rectangle.NewRect(xx, yy, short(q/2), short(q/2));
         rectangle.Connect(*this);
      }
   }
}

