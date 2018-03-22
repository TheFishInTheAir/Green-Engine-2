@echo off

set /a includeNumLines = 0
for /r ./include %%F in (*.cpp *.h *.hpp *.cs *.c) do (
  for /f %%N in ('find /v /c "" ^<"%%F"') do set /a includeNumLines+=%%N
)
echo Total number of code lines for ./include = %includeNumLines%

set /a srcNumLines = 0
for /r ./src %%F in (*.cpp *.h *.hpp *.cs *.c) do (
  for /f %%N in ('find /v /c "" ^<"%%F"') do set /a srcNumLines+=%%N
)
echo Total number of code lines for ./src = %srcNumLines%

set /a "bothNumLines=%srcNumLines%+%srcNumLines%"

echo Total number of code lines for ./src and ./include = %bothNumLines%

set /a testNumLines = 0
for /r ./test %%F in (*.cpp *.h *.hpp *.cs *.c) do (
  for /f %%N in ('find /v /c "" ^<"%%F"') do set /a testNumLines+=%%N
)
echo Total number of code lines for ./tests = %testNumLines%

set /a "allLines=%srcNumLines%+%srcNumLines%+%testNumLines%"

echo Total number of code lines %allLines%

