@echo off

echo.
echo ********************************************
echo *                                          *
echo * Initializing "POLY_DB" database ...      *
echo *                                          *
echo ********************************************
echo.
ddlp -b1 -d -oopub -oor poly_db.ddl
if not errorlevel 1 initdb -y poly_db
echo.
if errorlevel 1 goto Error
goto End


:Error
echo.
echo Error!  Database initialization failed!
echo.
goto End



:End
