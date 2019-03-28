@echo off
rem ############################################################################
rem This file is part of the argtable3 library.
rem
rem Copyright (C) 2013-2014 Tom G. Huang
rem <tomghuang@gmail.com>
rem All rights reserved.
rem
rem Redistribution and use in source and binary forms, with or without
rem modification, are permitted provided that the following conditions are met:
rem     * Redistributions of source code must retain the above copyright
rem       notice, this list of conditions and the following disclaimer.
rem     * Redistributions in binary form must reproduce the above copyright
rem       notice, this list of conditions and the following disclaimer in the
rem       documentation and/or other materials provided with the distribution.
rem     * Neither the name of STEWART HEITMANN nor the  names of its
rem       contributors may be used to endorse or promote products derived from
rem       this software without specific prior written permission.
rem
rem THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
rem AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
rem IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
rem ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
rem INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
rem (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
rem LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
rem ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
rem (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
rem THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
rem ############################################################################


setlocal

if "%1"=="" goto usage
set BUILD_CMD=%1

if "%BUILD_CMD%"=="dist" (
    goto dist
) else if "%BUILD_CMD%"=="cleanall" (
    goto build_cleanall
) else if "%BUILD_CMD%"=="help" (
    goto usage
) else (
    echo Unknown command: %BUILD_CMD%
    echo Type "build help" for available commands.
    goto :EOF
)


:usage
echo Argtable Amalgamation Package Build Script
echo.
echo Copyright (C) 2013-2014 Tom G. Huang
echo ^<tomghuang@gmail.com^>
echo All rights reserved.
echo.
echo Usage: build ^<command^>
echo.
echo Available commands:
echo   dist         build the amalgamation package
echo   cleanall     delete all generated folders/files
echo   help         display usage information
goto :EOF


:dist
cat -B amalgamation-header.txt > argtable3.c

cat -B ^
    ..\src\argtable3_private.h ^
    ..\src\arg_utils.c ^
    ..\src\arg_hashtable.c ^
    ..\src\arg_dstr.c ^
    ..\src\getopt.h ^
    ..\src\getopt_long.c ^
    ..\src\arg_date.c ^
    ..\src\arg_dbl.c ^
    ..\src\arg_end.c ^
    ..\src\arg_file.c ^
    ..\src\arg_int.c ^
    ..\src\arg_lit.c ^
    ..\src\arg_rem.c ^
    ..\src\arg_rex.c ^
    ..\src\arg_str.c ^
    ..\src\arg_cmd.c ^
    ..\src\argtable3.c ^
    >> argtable3.c

sed -i "s/$/\r/" argtable3.c
if not exist ..\dist mkdir ..\dist
move argtable3.c ..\dist
copy ..\src\argtable3.h ..\dist
copy ..\LICENSE ..\dist
copy ..\README.md ..\dist
xcopy ..\tests ..\dist\tests\ /H /K /Y
copy ..\src\argtable3_private.h ..\dist\tests
xcopy ..\examples ..\dist\examples\ /H /K /Y
goto :EOF


:build_cleanall
if exist ..\dist rmdir /s /q ..\dist
goto :EOF

endlocal

