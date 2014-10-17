// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMMain.cpp


#include <GTakPM.hpp>


int main(int argc, char *argv[])
{
 OXCPTRECORD   exceptionRecord;
 SystemHandler except;

 except.setHandler(exceptionRecord); // set system exception handler

 try
  {
   GTPMApp::GTakPM = new GTPMApp(argc, argv); 
  }
 catch(OException& ex) 
  {
   except.unsetHandler();           // release system exception handler
   _exit(ex.rc); 
  }

 GTPMApp::GTakPM->run();
 delete GTPMApp::GTakPM;

 except.unsetHandler();             // release system exception handler

 _exit(0);
}

// end of source
