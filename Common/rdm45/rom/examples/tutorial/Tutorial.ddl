/* database name */
database tutorial 
{
   data file "tutorial.d00" contains  system, Author;
   data file "tutorial.d01" contains Info;
   key  file "tutorial.k00" contains fullname;
   key  file "tutorial.k01" contains idcode;
  

   record Author 
   {
      char  lastname[15];
      char  firstname[15];
	  
          compound unique key fullname 
	  {
	     lastname;
             firstname;
	  }
   }

   record Info 
   {
      unique key char  idcode[16];
      char             title[80];
      char             publisher[31];
      char             pubdate[12];
      short            type;
      char             loaned;
   }
 
   set HasPublished 
   {
      order   ascending;
      owner   Author;
      member  Info by title;
   }
   set AuthorList
   {
      order ascending;
      owner system;
      member Author by lastname;
   }
}
  
