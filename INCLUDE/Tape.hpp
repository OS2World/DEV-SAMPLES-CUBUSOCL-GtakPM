// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMApp.hpp


#ifndef TAPE_HPP_INCLUDED
  #define TAPE_HPP_INCLUDED


#ifndef GTAKPM_HPP_INCLUDED
  #include <GTakPM.hpp>
#endif


typedef class Tape            *pTape;


class Tape 
{
 protected:
   pOPMPipeTool tool;

   ULONG        bsize;
   OString      device;

 public:
   Tape(pOPMPipeTool currentTool, 
        PSZ TapeDescription,  
        ULONG tape_blocksize);

   ~Tape();

   void prepare();
   void blocksize(ULONG size = 0);
   void status();
   void load();
   void unload();
   void rewind();
   void part(OString& command); 
   void format();
   void erase();
   void eraseq();
   void lock();
   void unlock();
   void batch(PSZ batchfile);
};

#endif // TAPE_HPP_INCLUDED

// end of source
