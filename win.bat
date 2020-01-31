@echo off


REM ------------- build cpp ----------------
REM before run this shell
REM go to Visual Studio first, 
REM     set Build root: ${projectDir}\build
REM     compiler cpp-exe by cmake-clang


REM ----------------- prepare dirs ---------------

SET DIR_base=%~dp0
SET DIR_out=%~dp0\build\publish

SET DIR_tprLog=%~dp0\build\publish\tprLog

SET DIR_src_shaders=%~dp0\shaders
SET DIR_src_jsons=%~dp0\jsons
SET DIR_src_blueprintDatas=%~dp0\blueprintDatas
SET DIR_src_gameObjDatas=%~dp0\gameObjDatas


SET DIR_dst_shaders=%~dp0\build\publish\shaders
SET DIR_dst_jsons=%~dp0\build\publish\jsons
SET DIR_dst_blueprintDatas=%~dp0\build\publish\blueprintDatas
SET DIR_dst_gameObjDatas=%~dp0\build\publish\gameObjDatas

mkdir %DIR_out%
mkdir %DIR_tprLog%
mkdir %DIR_dst_shaders%
mkdir %DIR_dst_jsons%
mkdir %DIR_dst_blueprintDatas%
mkdir %DIR_dst_gameObjDatas%

xcopy /E /V /F /K /Y %DIR_src_shaders% %DIR_dst_shaders%
xcopy /E /V /F /K /Y %DIR_src_jsons% %DIR_dst_jsons%
xcopy /E /V /F /K /Y %DIR_src_blueprintDatas% %DIR_dst_blueprintDatas%
xcopy /E /V /F /K /Y %DIR_src_gameObjDatas% %DIR_dst_gameObjDatas%
