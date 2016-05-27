#ifndef __MAC_COMMAND_H__
#define __MAC_COMMAND_H__

#include <inttypes.h>
#include <stdlib.h>
#include "mac-common.h"
#include "mac-core.h"

void MAC_CmdSetFrameDstFromSrc(MAC_Frame *F, MAC_Frame *SF);
void MAC_CmdSetFrameHeader(MAC_Handle *H, MAC_Frame *F);
void MAC_CmdAssocResponseSend(MAC_Handle *H, MAC_Frame *SF,
                              MAC_FrameAssocStatus Status, uint16_t ShortAdr);
void MAC_CmdAssocRequestSend(MAC_Handle *H);
void MAC_CmdDataRequestSend(MAC_Handle *H);
void MAC_CmdAssocRequestHandler(MAC_Handle *H, MAC_Frame *F);
void MAC_CmdAssocResponseHandler(MAC_Handle *H, MAC_Frame *F,
                                 MAC_FrameCommand *C);
MAC_Status MAC_CmdExecute(MAC_Handle *H, MAC_Frame *F);

#endif // __MAC_COMMAND_H__