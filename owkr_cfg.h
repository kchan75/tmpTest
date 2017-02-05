/**
 * =================================================================================================
 *                    AFEC Oltp Framework Core worker [owkr_cfg.h] File
 * =================================================================================================
 *  시스템　명 : OLTP Framework Core
 *  FILE    명 : owkr_cfg.h
 *  FILE 종 류 : H File
 *  FILE 개 요 : OLTP Framework worker Config
 * =================================================================================================
 *  변 경 일 자 : 변경자 : 변 경 내 역
 *  -----------   ------   -------------------------------------------------------------------------
 *  #2011-12-15 :        : 파일 신규작성 : 변경 및 정리
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
    // Node 관련 정보
    char            cfgDir       [     256  ];    // Configuration file(.cfg) 위치
    char            logDir       [     256  ];    // LOGDIR : LOG Base Dir
    char            coreDir      [     256  ];    // Core Log write Dir
        
    // Server 정보
    char            svrname      [      250  ];    // 서버명
    char            svrId        [        6  ];    // 서버 ID
    pid_t           pid                       ;    // 해당 서버의 PID ------------------------------------
    
    // Transaction 관리 정보
    char            txReq                     ;    // DB 접속 필요 여부 Y:use, N:not use
    char            xaMode                    ;    // X:XA Mode, N:non XA mode
    char            useRm                     ;    // Resource Manager 사용여부 Y:use, N:not use
    int             xaTimeout                 ;    // XA 사용 시 타임아웃 
    
    // RM 사용하지 않을 경우, DB 접속 계정 정보
    char            dbUsr        [      250  ];    // DB 계정명
    
    char            useGca                    ;    // GCA 사용여부
    
    // LOG 관련 정보
    char            dftlogFlag                ;    // Default 로그 사용 Flag
    char            dftlogLvl                 ;    // Default 로그 레벨
    
    char            logBase      [        9  ];    // 로그 베이스 일자
    FILE            *fpTxIn                   ;    // Tx In     Log File Description
    FILE            *fpTxOt                   ;    // Tx Out    Log File Description
    FILE            *fpSms                    ;    // Sms       Log File Description
} _svr_cfg;                                        // ### 서버 설정 정보 ###

typedef struct {
    int             inLen                     ;    // 입력 전문 길이
    int             outLen                    ;    // 출력 전문 길이(업무 개별부길이)
    int             totOutLen                 ;    // 출력 전문 전체 길이
    startarea_t     *pSA                      ;    // STARTAREA
    char            outputFlag                ;    // 출력 전문 구성 구분
                                                   //   'Y' Output outstream 필요
                                                   //   'N' Output 전문 없음
                                                   //   'P' Output outstream 필요없음
    char            isCallBizDirect           ;    // Worker 및 비즈니스 서비스 전/후처리 부분을 수행할지 말지 여부
                                                   //   'Y' : worker 및 비즈니스 서비스 전처리 수행안함
                                                   //   'N' : worker 및 비스니스 서비스 수행
                                                   // 업무 소스 내에서 서비스 콜 시는 'Y'으로 호출 가능                    
    char            rtnType                   ;    // 거래 리턴 타입
    char            trStTm       [       21  ];    // 거래 시작 일시
    char            trEdTm       [       21  ];    // 거래 종료 일시
    struct timeval  trStTv                    ;    // 거래 시작 일시 timeval
    struct timeval  trEdTv                    ;    // 거래 종료 일시 timeval
    
    int             hdr_stp      [      100  ];    // 데이터 셋 구조
    
    
} _tr_info;                                        // ### 해당 거래에 관련한 정보 ###

typedef struct {
    int             runStTm                   ;    // 거래 가능 시작 시간 (HHMI)
    int             runEdTm                   ;    // 거래 가능 종료 시간 (HHMI)
    char            runYn                     ;    // 거래 수행 여부
    char            svcGrpCd     [        3  ];    // duty check 를 위한 svcGrpCd
    char            reserve      [        7  ];    // 예약 영역
} _tr_ctl;                                         // ### 서비스 제어정보 ###

typedef struct {
    _FW_CM          fw                        ;    // Framework 공통 헤더
    _svr_cfg        cfg                       ;    // 설정 정보
    _tr_info        tr                        ;    // 거래 정보
    _tr_ctl         ctl                       ;    // 거래 제어 정보
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