@echo off

REM This script builds the sample for x64 Windows 10. If using Win7, adjust the 'dotnet publish' command.
REM It assumes that both the dotnet CLI and cl.exe compiler are available on the path.
REM A Visual Studio Developer Command Prompt will already have cl.exe on its path.

SET SRCDIR=%~dp0\..\csharp
SET OUTDIR=%~dp0\..\\build\bin\csharpLibs

SET DIR_SRC_shaders=%~dp0\..\shaders
SET DIR_SRC_textures=%~dp0\..\textures
SET DIR_DST_shaders=%~dp0\..\build\bin\shaders
SET DIR_DST_textures=%~dp0\..\build\bin\textures


mkdir %OUTDIR%
mkdir %DIR_DST_shaders%
mkdir %DIR_DST_textures%


xcopy /E /V /F /K /Y %DIR_SRC_shaders% %DIR_DST_shaders%
xcopy /E /V /F /K /Y %DIR_SRC_textures% %DIR_DST_textures%
REM /E: recursive copy all dir/file (include empty dir) 
REM /V: verify every file be copyed
REM /F: show src/dst file's entire path
REM /K: copy file's property
REM /Y: do not ask if Y/N



REM Build managed component

REM -- echo Building Managed Library
REM -- dotnet publish --self-contained -r win10-x64 %SRCDIR%\ManagedLibrary\ManagedLibrary.csproj -o %OUTDIR%

REM -r osx-x64: set target OS
REM -o %OUTDIR%: set outExe path
REM


REM Build native component
REM cl.exe %SRCDIR%\SampleHost_origin.cpp /Fo%OUTDIR%\ /Fd%OUTDIR%\SampleHost.pdb /EHsc /Od /GS /sdl /Zi /D "WINDOWS" /link ole32.lib /out:%OUTDIR%\SampleHost.exe
REM /Fo: set outExe path
REM /Fd: Generate File Dependencies; build .PDB file
REM /EHsc /Od /GS /sdl /Zi:
REM /D "WINDOWS": create a marco
REM /link ole32.lib:
REM /out:xxx.exe
