@echo on

set "MAIN_WORKSPACE=%~1"
set "THIS_WORKSPACE=%~2"

::ENVCONF_DEVENV
::ENVCONF_MSBUILD

::-------------------------------------------------------------------------------------
::compile and run
powershell %MAIN_WORKSPACE%/exec.cmd %THIS_WORKSPACE% python3 /wcore/workspace/cllio/prj/prj.cllio-test.py -a vs2019 win32

cd %THIS_WORKSPACE%/cllio/prj/build/cllio-test_win32_vs2019

"%ENVCONF_DEVENV%" _cllio_test.sln /Build Debug
"%ENVCONF_DEVENV%" _cllio_test.sln /Build Release


"bin/x32/Debug/_cllio-test.exe"

::%ENVCONF_PYTHON% -c "import ciutil; ciutil.files_equal('samples.bin','../../../tests/samples.bin')"

"bin/x32/Release/_cllio-test.exe"

::%ENVCONF_PYTHON% -c "import ciutil; ciutil.files_equal('samples.bin','../../../tests/samples.bin')"

::-------------------------------------------------------------------------------------
::run RunCodeAnalysis on Release
powershell %MAIN_WORKSPACE%/exec.cmd %THIS_WORKSPACE% python3 /wcore/workspace/cllio/prj/prj.cllio.py vs2019 win32

cd %THIS_WORKSPACE%/cllio/prj/build/cllio_win32_vs2019

"%ENVCONF_MSBUILD%" _cllio.sln /m /p:RunCodeAnalysis=true /p:Configuration=Release /p:Platform=Win32

