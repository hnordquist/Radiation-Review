@echo off

echo.
echo *****************************************
echo *                                       *
echo * Initializing "MUTIMS" database ...   *
echo *                                       *
echo *****************************************
echo.
ddlp -b1 -d -oopub -oor mutims.ddl
if not errorlevel 1 initdb -y mutims
if not errorlevel 1 dbimp -n mutims.imp
echo.
if errorlevel 1 goto Error
goto End

:Error
echo.
echo Error!  Database initialization failed!
echo.
goto End

:End
