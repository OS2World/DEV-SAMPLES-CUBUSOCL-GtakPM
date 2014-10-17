.all: .\GTAKPM.RSL .\GTAKPM.EXE

.SUFFIXES:

.SUFFIXES: .rc .c .cpp

.rc.res:
  rc -r %s %|fF.RES

.c.obj:
  icc /Tdc /Q /Fi /Si /Ti /Gm /G5 /C %s

.cpp.obj:
  icc /Tdp /Q /Fi /Si /Ti /Gm /G5 /C %s


.\GTAKPM.RSL: \
  .\RSL.obj \
  .\RSL.res \
  Makefile
    icc @<<
/Tdc /Q /Fi /Si /Ti /Gm /G5
/B"/nod /pmtype:pm "
/FeGTAKPM.RSL 
 .\RSL.obj
 .\RSL.def 
<<
   rc.exe $(RCPACK) .\RSL.res GTAKPM.RSL


.\GTAKPM.EXE: \
  .\GTPMException.obj \
  .\GTPMMain.obj \
  .\GTPMApp.obj \
  .\GTPMWin.obj \
  .\GTPMWinControl.obj \
  .\GTPMWinCommand.obj \
  .\FilesCnr.obj \
  .\DrivesCnr.obj \
  .\Tape.obj \
  .\Backup.obj \
  .\Restore.obj \
  .\GTakPM.res \
  {$(LIB)}OCLI.lib \
  Makefile
    icc @<<
/Tdp /Q /Fi /Si /Ti /Gm /G5 
/B"/pmtype:pm"
/FeGTAKPM.EXE 
 OCLI.lib 
 .\GTPMException.obj
 .\GTPMMain.obj
 .\GTPMApp.obj
 .\GTPMWin.obj
 .\GTPMWinControl.obj
 .\GTPMWinCommand.obj
 .\FilesCnr.obj 
 .\DrivesCnr.obj
 .\Tape.obj
 .\Backup.obj
 .\Restore.obj
<<
   rc.exe $(RCPACK) .\GTakPM.res GTAKPM.EXE


!include Makefile.Dep