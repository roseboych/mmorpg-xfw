xcopy .\datas\* ..\publish\datas /E /C 

copy /Y .\rgs\*.exe ..\publish\rgs
copy /Y .\rgs\svrno ..\publish\rgs
copy /Y .\opt\rgs.properties ..\publish\rgs

copy /Y .\lgs\*.exe ..\publish\lgs
copy /Y .\lgs\svrno ..\publish\lgs
copy /Y .\opt\lgs.properties ..\publish\lgs

copy /Y .\cts\*.exe ..\publish\cts
copy /Y .\cts\svrno ..\publish\cts
copy /Y .\opt\cts.properties ..\publish\cts

copy /Y .\css\*.exe ..\publish\css
copy /Y .\css\svrno ..\publish\css
copy /Y .\opt\css.properties ..\publish\css

copy /Y .\dpx\*.exe ..\publish\dpx
copy /Y .\dpx\svrno ..\publish\dpx
copy /Y .\opt\dpx.properties ..\publish\dpx

copy /Y .\gts\*.exe ..\publish\gts
copy /Y .\gts\svrno ..\publish\gts
copy /Y .\opt\gts.properties ..\publish\gts

copy /Y ..\doc\db\*.sql ..\publish\db