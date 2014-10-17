// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMWinControl.cpp


#include <GTakPM.hpp>


BOOL GTPMWin::control(MPARAM mp1, MPARAM mp2)
{
 switch(SHORT1FROMMP(mp1))
  {
   case OBUTTONBAR_MOVEOVER:
    statline->setText((PSZ) PVOIDFROMMP(mp2));
    wasOutOfContainer = TRUE;
    break;
 
   case GTPM_DRIVECNR:
    switch(SHORT2FROMMP(mp1))
     {
      case CN_EXPANDTREE:
      case CN_COLLAPSETREE: {
       pDirInfo record = Drives->queryEmphasis();

       if (!record)
         break;
       if ((SHORT2FROMMP(mp1) == CN_EXPANDTREE) && (!record->wasExpanded)) {
         record->wasExpanded = TRUE; 
         expander.run(); }
       if (record->rc.pszTree[1] == ':')
         break;
       record->expanded = !record->expanded;
       if (record->expanded)
         record->rc.hptrIcon = 
         record->selected ? Drives->openfolderS_ico.hptr : Drives->openfolder_ico.hptr;
       else
         record->rc.hptrIcon = 
         record->selected ? Drives->folderS_ico.hptr : Drives->folder_ico.hptr;
       break; }

      case CN_ENTER: {
       pDirInfo record = Drives->queryEmphasis();
       pDirInfo parent;
  
       if (!record) 
         break;
       parent = Drives->queryRecord(record, CMA_PARENT, CMA_ITEMORDER);
       record->selected = !record->selected;
       Drives->select.run();
       updateListbox(record->dirItem->fullName.text);
       if ((!parent) && (record->selected))
         addInclude(record->dirItem->fullName.text);
       else if (((parent) && (parent->selected)) && (!record->selected))
         addExclude(record->dirItem->fullName.text);
       else if (((parent) && (!parent->selected)) && (record->selected))
         addInclude(record->dirItem->fullName.text);
       break; }

      case CN_EMPHASIS: { 
       PNOTIFYRECORDEMPHASIS pne = (PNOTIFYRECORDEMPHASIS) mp2;
       pDirInfo              record = Drives->queryEmphasis();

       if ((!pne) || 
           (!(pne->fEmphasisMask & CRA_SELECTED)) ||
           (!record))
         break;
       Files->scanDirectory << record->dirItem->fullName.text;
       Files->populate.run(); 
       break; }
     }
    break;

   default:
    return(FALSE);
  }
 return(TRUE);
}

// end of source
