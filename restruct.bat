@echo off
setlocal

echo === Creating target folders ===
mkdir src 2>nul
mkdir examples 2>nul
mkdir archive 2>nul

echo === Moving libraries to /src ===
if exist "UnitLib" git mv UnitLib src\UnitLib

echo === Moving active work to /examples ===
if exist "Project3" git mv Project3 examples\Project3
if exist "from-scratch" git mv from-scratch examples\from-scratch
if exist "ConsoleApplication1" git mv ConsoleApplication1 examples\ConsoleApplication1
if exist "Examples3" git mv Examples3 examples\Examples3

echo === Archiving legacy / bulk dumps ===
if exist "ExamplesArchive" git mv ExamplesArchive archive\ExamplesArchive

echo === Optional: move leftover zips ===
for %%f in (*.zip) do (
    git mv "%%f" archive\%%f
)

echo === Staging all changes ===
git add .

echo === Done ===
echo Next step: git commit -m "Restructure repo into src/examples/archive"

endlocal
