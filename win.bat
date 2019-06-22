@echo off


REM ------------- build cpp ----------------
REM before run this shell
REM go to Visual Studio first, 
REM     set Build root: ${projectDir}\..\build\publish
REM     compiler cpp dll by cmake-clang


REM ----------------- prepare dirs ---------------

SET DIR_base=%~dp0
SET DIR_out=%~dp0\build\publish

SET DIR_src_shaders=%~dp0\shaders
SET DIR_src_textures=%~dp0\textures
SET DIR_dst_shaders=%~dp0\build\publish\shaders
SET DIR_dst_textures=%~dp0\build\publish\textures

mkdir %DIR_out%
mkdir %DIR_dst_shaders%
mkdir %DIR_dst_textures%

xcopy /E /V /F /K /Y %DIR_src_shaders% %DIR_dst_shaders%
xcopy /E /V /F /K /Y %DIR_src_textures% %DIR_dst_textures%



REM ------------- build c# ----------------
REM "dotnet add package..." is optional
dotnet add package ILLink.Tasks -v 0.1.4-preview-981901 
dotnet publish -c Release -r win-x64 --self-contained true

