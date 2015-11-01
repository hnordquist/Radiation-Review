@echo off

echo.
echo *****************************************
echo *                                       *
echo * Initializing "TUTORIAL" database ...  *
echo *                                       *
echo *****************************************
echo.
ddlp -b1 -d -oopub tutorial.ddl
if not errorlevel 1 initdb -y tutorial
if not errorlevel 1 dbimp -n tutorial.imp
echo.
if errorlevel 1 goto Error
goto End

:Error
echo.
echo Error!  Database initialization failed!
echo.
goto End

:End
