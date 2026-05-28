"C:\tools\Lint\Lint-nt" +v -i"C:\tools\Lint\lnt" std.lnt -os(_LINT.TMP) ..\source\*.cpp  
type _LINT.TMP | more
@echo off
echo ---
echo  output placed in _LINT.TMP
