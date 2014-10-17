// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// Tape.cpp


#include <GTakPM.hpp>



Tape::Tape(pOPMPipeTool currentTool, PSZ tape_description, ULONG tape_blocksize)
  : tool(currentTool),
    bsize(tape_blocksize),
    device(tape_description)
  {}

Tape::~Tape()
  {}


void Tape::prepare()
{
 system("TAPE.EXE ST >NUL 2>&1");
}

void Tape::blocksize(ULONG size)
{
 CHAR    str[10];
 OString command("TAPE.EXE BLOCKSIZE=");

 _itoa(size, str, 10);
 command + str;
 command + " >NUL 2>&1";
 system(command.text);
}


void Tape::status()
{
 prepare();
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST";
 tool->detachChild();
}

void Tape::load()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST LOAD";
 tool->detachChild();
}

void Tape::unload()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST UNLOAD";
 tool->detachChild();
}

void Tape::rewind()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST REW";
 tool->detachChild();
}

void Tape::part(OString& command)
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST ";
 tool->childParms + command;
 tool->detachChild();
}

void Tape::format()
{
 throw OVioException("Tape: operation not supported for SCSI-devices", 0);
}

void Tape::erase()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST ERASE";
 tool->detachChild();
}

void Tape::eraseq()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST ERASEQ";
 tool->detachChild();
}

void Tape::lock()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST RESERVE";
 tool->detachChild();
}

void Tape::unlock()
{
 tool->childName  << "TAPE.EXE";
 tool->childParms << "ST RELEASE";
 tool->detachChild();
}

void Tape::batch(PSZ batchfile)
{
 tool->childName  << "CMD.EXE";
 tool->childParms << "/c ";
 tool->childParms  + batchfile;
 tool->detachChild();
}

// end of source
