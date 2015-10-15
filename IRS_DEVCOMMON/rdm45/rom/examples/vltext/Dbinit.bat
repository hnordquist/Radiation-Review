@echo off


echo.
echo *********************************************
echo *                                           *
echo * Initializing "VLTXT_DB" database ...      *
echo *                                           *
echo *********************************************
echo.
ddlp  -b1 -d -oopub -oor vltxt_db.ddl
if not errorlevel 1 initdb -y vltxt_db
echo.
if errorlevel 1 goto Error
goto End


:Error
echo.
echo Error!  Database initialization failed!
echo.
goto End


:End
