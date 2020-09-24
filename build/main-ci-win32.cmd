@echo on

set "MAIN_WORKSPACE=%~1"
set "THIS_WORKSPACE=%~2"

::ENVCONF_DEVENV 
::ENVCONF_MSBUILD

::-------------------------------------------------------------------------------------
::compile and run
powershell %MAIN_WORKSPACE%/exec.cmd %THIS_WORKSPACE% python3 /wcore/workspace/cllio/prj.cllio-test.py vs2019 win32

cd %THIS_WORKSPACE%/cllio/build/cllio-test_win32_vs2019

"%ENVCONF_DEVENV%" _this_.sln /Build Debug

"bin/x32/Debug/_cllio-test.exe"

::-------------------------------------------------------------------------------------
::run RunCodeAnalysis on Release
powershell %MAIN_WORKSPACE%/exec.cmd %THIS_WORKSPACE% python3 /wcore/workspace/cllio/prj.cllio.py vs2019 win32

cd %THIS_WORKSPACE%/cllio/build/cllio_win32_vs2019

"%ENVCONF_MSBUILD%" _this_.sln /m /p:RunCodeAnalysis=true /p:Configuration=Release /p:Platform=Win32

