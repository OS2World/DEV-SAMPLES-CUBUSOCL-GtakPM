// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMMonitor.cpp


#include <GTakPM.hpp>

GTPMMonitor::GTPMMonitor()
 : OFrame(MONITOR_ICO, 
          FCF_SIZEBORDER |
          FCF_ICON |
          FCF_SYSMENU | 
          FCF_TITLEBAR |
          FCF_MINMAX |
          FCF_SHELLPOSITION),
   OPipeTool((PSZ)NULL, (PSZ)NULL)
{
 if (!createFrame("GTakPM Monitor"))
   throwGTPM(ERROR_FRAME_NOT_CREATED);
 box = new OListBox(this, LS_NOADJUSTPOS | LS_HORZSCROLL);
 if (!box->createListBox(0,0,0,0)) 
   throw GTPMException(ERROR_LISTBOX_CREATION, OException::fatal);
 box->setBackColor(SYSCLR_WINDOW);
 box->setForeColor(SYSCLR_WINDOWTEXT);
 box->setFont("8.Helv");
}

      

GTPMMonitor::~GTPMMonitor()
 {}

BOOL GTPMMonitor::OCommand(ULONG msg, MPARAM mp1, MPARAM mp2)
{
 switch(msg)
  {
   case WM_SIZE:
     if (!box) break; 
     WinSetWindowPos(box->hwnd, HWND_TOP, 0, 0, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2),
                     SWP_SIZE | SWP_MOVE);
     break;

   case WM_CLOSE:
     cancelClose();
     hideFrame();
     break;

   default:
     return(FALSE);
  }
 return(TRUE);
}


void GTPMMonitor::printout()
{
 OPMinit init;

 while(pipe.getline(readBuffer.text, bufferSize))
  {
   readBuffer - '\r';
   box->insertItem(readBuffer);
  }
}

// end of source
