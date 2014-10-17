// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMWinCommand.cpp


#include <GTakPM.hpp>


BOOL GTPMWin::command(MPARAM mp1, MPARAM mp2)
{
 switch(SHORT1FROMMP(mp1))
  {
   case CMD_QUIT:
    WinPostMsg(hwnd, WM_CLOSE, NULL, NULL);
    break;

   case CMD_RESCAN:
    Excludes->deleteAll();
    Includes->deleteAll();
    Drives->setupItems();
    break;

   case CMD_BACKUP:
    if(!showMonitor)
      command(MPFROM2SHORT(CMD_MONITOR, 0), NULL);
    backupThr.run();
    break;

   case CMD_RESTORE:
    if(!showMonitor)
      command(MPFROM2SHORT(CMD_MONITOR, 0), NULL);
    restoreThr.run();
    break;

   case CMD_MONITOR:
    WinQueryWindowRect(hwnd, &rcl);
    showMonitor = !showMonitor;
    resizeClients(MPFROM2SHORT(rcl.xRight - rcl.xLeft, rcl.yTop - rcl.yBottom)); 
    break;

   case TAPEBAR_LOAD:
    processing = TRUE;
    tape->load();
    break;
   case TAPEBAR_UNLOAD:
    processing = TRUE;
    tape->unload();
    break;
   case TAPEBAR_REWIND:
    processing = TRUE;
    tape->rewind();
    break;
   case TAPEBAR_PARTITIONS: {
    processing = TRUE;
    OString command("SIZE=10");
    tape->part(command); 
    break; }
   case TAPEBAR_FORMAT:
    processing = TRUE;
    tape->format();
    break;
   case TAPEBAR_ERASE:
    processing = TRUE;
    tape->erase();
    break;
   case TAPEBAR_ERASEQ:
    processing = TRUE;
    tape->eraseq();
    break;
   case TAPEBAR_LOCK:
    processing = TRUE;
    tape->lock();
    break;
   case TAPEBAR_UNLOCK:
    processing = TRUE;
    tape->unlock();
    break;
   case TAPEBAR_STOP_ASAP:
    stopChild();
    break;
 
   default:
    return(FALSE);
  }
 return(TRUE);
}


// end of source
