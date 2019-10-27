#ifndef EXTINTER_H_
#define EXTINTER_H_

#include "std_types.h"
#include "ExtInter_conf.h"


Std_Func_t INTP_vidInit(void);

void INTP0_vidEnabled(void);
void INTP1_vidEnabled(void);
void INTP2_vidEnabled(void);

void INTP0_vidDisabled(void);
void INTP1_vidDisabled(void);
void INTP2_vidDisabled(void);

void INTP0_VidSelectEvent(uint8 COPY_u8EventState);
void INTP1_VidSelectEvent(uint8 COPY_u8EventState);
void INTP2_VidSelectEvent(uint8 COPY_u8EventState);

void INTP_VidDisableGlobal(void);

void INTP0_vidSetCallBack(void(*COPY_pvidCallBackFunction)(void));
void INTP1_vidSetCallBack(void(*COPY_pvidCallBackFunction)(void));
void INTP2_vidSetCallBack(void(*COPY_pvidCallBackFunction)(void));

#endif
