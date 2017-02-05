/**
 * =================================================================================================
 *                    AFEC Oltp Framework Core worker [owkr_cfg.h] File
 * =================================================================================================
 *  �ý��ۡ��� : OLTP Framework Core
 *  FILE    �� : owkr_cfg.h
 *  FILE �� �� : H File
 *  FILE �� �� : OLTP Framework worker Config
 * =================================================================================================
 *  �� �� �� �� : ������ : �� �� �� ��
 *  -----------   ------   -------------------------------------------------------------------------
 *  #2011-12-15 :        : ���� �ű��ۼ� : ���� �� ����
 * -------------------------------------------------------------------------------------------------
 *
 *
 * =================================================================================================
**/
#include "startarea.h"

#ifndef __FW_OLTP_WKR_CFG_H__
#define __FW_OLTP_WKR_CFG_H__

/***************************************************************************************************
 * 1. Include Section
 **************************************************************************************************/

/***************************************************************************************************
 * 2. Constants Define Section
 **************************************************************************************************/
typedef struct {
    // Node ���� ����
    char            cfgDir       [     256  ];    // Configuration file(.cfg) ��ġ
    char            logDir       [     256  ];    // LOGDIR : LOG Base Dir
    char            coreDir      [     256  ];    // Core Log write Dir
        
    // Server ����
    char            svrname      [      250  ];    // ������
    char            svrId        [        6  ];    // ���� ID
    pid_t           pid                       ;    // �ش� ������ PID ------------------------------------
    
    // Transaction ���� ����
    char            txReq                     ;    // DB ���� �ʿ� ���� Y:use, N:not use
    char            xaMode                    ;    // X:XA Mode, N:non XA mode
    char            useRm                     ;    // Resource Manager ��뿩�� Y:use, N:not use
    int             xaTimeout                 ;    // XA ��� �� Ÿ�Ӿƿ� 
    
    // RM ������� ���� ���, DB ���� ���� ����
    char            dbUsr        [      250  ];    // DB ������
    
    char            useGca                    ;    // GCA ��뿩��
    
    // LOG ���� ����
    char            dftlogFlag                ;    // Default �α� ��� Flag
    char            dftlogLvl                 ;    // Default �α� ����
    
    char            logBase      [        9  ];    // �α� ���̽� ����
    FILE            *fpTxIn                   ;    // Tx In     Log File Description
    FILE            *fpTxOt                   ;    // Tx Out    Log File Description
    FILE            *fpSms                    ;    // Sms       Log File Description
} _svr_cfg;                                        // ### ���� ���� ���� ###

typedef struct {
    int             inLen                     ;    // �Է� ���� ����
    int             outLen                    ;    // ��� ���� ����(���� �����α���)
    int             totOutLen                 ;    // ��� ���� ��ü ����
    startarea_t     *pSA                      ;    // STARTAREA
    char            outputFlag                ;    // ��� ���� ���� ����
                                                   //   'Y' Output outstream �ʿ�
                                                   //   'N' Output ���� ����
                                                   //   'P' Output outstream �ʿ����
    char            isCallBizDirect           ;    // Worker �� ����Ͻ� ���� ��/��ó�� �κ��� �������� ���� ����
                                                   //   'Y' : worker �� ����Ͻ� ���� ��ó�� �������
                                                   //   'N' : worker �� �񽺴Ͻ� ���� ����
                                                   // ���� �ҽ� ������ ���� �� �ô� 'Y'���� ȣ�� ����                    
    char            rtnType                   ;    // �ŷ� ���� Ÿ��
    char            trStTm       [       21  ];    // �ŷ� ���� �Ͻ�
    char            trEdTm       [       21  ];    // �ŷ� ���� �Ͻ�
    struct timeval  trStTv                    ;    // �ŷ� ���� �Ͻ� timeval
    struct timeval  trEdTv                    ;    // �ŷ� ���� �Ͻ� timeval
    
    int             hdr_stp      [      100  ];    // ������ �� ����
    
    
} _tr_info;                                        // ### �ش� �ŷ��� ������ ���� ###

typedef struct {
    int             runStTm                   ;    // �ŷ� ���� ���� �ð� (HHMI)
    int             runEdTm                   ;    // �ŷ� ���� ���� �ð� (HHMI)
    char            runYn                     ;    // �ŷ� ���� ����
    char            svcGrpCd     [        3  ];    // duty check �� ���� svcGrpCd
    char            reserve      [        7  ];    // ���� ����
} _tr_ctl;                                         // ### ���� �������� ###

typedef struct {
    _FW_CM          fw                        ;    // Framework ���� ���
    _svr_cfg        cfg                       ;    // ���� ����
    _tr_info        tr                        ;    // �ŷ� ����
    _tr_ctl         ctl                       ;    // �ŷ� ���� ����
} _OLTP_CM;
 
/***************************************************************************************************
 * 3. Function Prototype Section
 **************************************************************************************************/
int        _owkr_cfg_cfg    ( _OLTP_CM *olt, int argc, char **argv );
int        _owkr_cfg_logfd_close    ( _OLTP_CM *olt );
int        _owkr_cfg_logfd_open    ( _OLTP_CM *olt );

/***************************************************************************************************
 * 4. etc Section
 **************************************************************************************************/

#endif//__FW_OLTP_WKR_CONST_H__
/**
 * =================================================================================================
 *     >>>>>>>>>>    AFEC Oltp Framework Core worker [owkr_cfg.h] File   E N D !!     <<<<<<<<<<
 * =================================================================================================
**/