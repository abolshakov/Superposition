echo off
set /a q=1

setlocal ENABLEDELAYEDEXPANSION

for %%a IN (fog\*.png) do (
  
  set b=fog\fog%q%.png

  REM ren %%a %%b
  echo %%a
  echo %b%
  echo %q%
  call :increaseby1
)
goto :alldone


:increaseby1
set /a q=%q%+1
goto :eof

:alldone
endlocal