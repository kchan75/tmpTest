/**
 * =================================================================================================
 *                    AFEC Oltp Framework Core worker [owkr_cfg.c] File
 * =================================================================================================
 *  �ý��ۡ��� : OLTP Framework Core
 *  FILE    �� : owkr_cfg.c
 *  FILE �� �� : C File
 *  FILE �� �� : OLTP Framework worker configuration
 * =================================================================================================
 *  �� �� �� �� : ������ : �� �� �� ��
 *  -----------   ------   -------------------------------------------------------------------------
 *  #2011-12-15 :        : ���� �ű��ۼ� : ���� �� ����
 * -------------------------------------------------------------------------------------------------
 *
 *
 * =================================================================================================
**/

/***************************************************************************************************
 * 1. Include Section
 **************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#ifdef _TMAX_
#include "tmaxapi.h"
#endif

#include "afc_clbcom.h"
#include "afc_com.h"
#include "afc_cust.h"

#include "oltpwkr.h"
#include "owkr_main.h"
#include "owkr_cfg.h"

/***************************************************************************************************
 * 2. Macro Define Section
 **************************************************************************************************/

/***************************************************************************************************
 * 3. Type Define Section
 **************************************************************************************************/

/***************************************************************************************************
 * 4. Global Constants Define Section
 **************************************************************************************************/
extern char         *svrName;

/***************************************************************************************************
 * 5. Function Prototype
 **************************************************************************************************/
char    _st_owkr_cfg_str2LogFlag    ( char *str );
char    _st_owkr_cfg_str2LogLevel    ( char logFlag, char *logMod, char *logLvl );

/***************************************************************************************************
 * 6. Implementation Section
 **************************************************************************************************/

/**
 * -------------------------------------------------------------------------------------------------
 *  #�Լ�     �� : _owkr_cfg_cfg
 *  #�Լ� ��  �� : OLTP Application �� ���� cfg ��������
 *  #�Լ� Input  : _OLTP_CM    *olt    : oltp ���� ���
 *                 int         argc   : �Է� �ƱԸ�Ʈ ����
 *                 char      **argv : �Է� �ƱԸ�Ʈ ����
 *  #�Լ� Output : NONE
 *  #�Լ� Return : ���� / ���� ����
 * -------------------------------------------------------------------------------------------------
**/
int        _owkr_cfg_cfg    ( _OLTP_CM *olt, int argc, char **argv )
{
    time_t t;
    char tmp1[200];
    char tmp2[200];
    int     i;

    char *node_cfg, path[256];

    // ����ID ��������
#ifdef _TMAX_
    TMAXSVRINFO  tmax_svrinfo;
    tmax_my_svrinfo ( &tmax_svrinfo );

    sprintf(olt->cfg.svrId, "%05d",(tmax_svrinfo.svri + 1000) * 100 + tmax_svrinfo.spr_seqno);
    fprintf(stdout, "server svri      : [%d]\n", tmax_svrinfo.svri);
    fprintf(stdout, "server spr_seqno : [%d]\n", tmax_svrinfo.spr_seqno);
    fprintf(stdout, "server id        : [%s]\n", olt->cfg.svrId);

#else
    memset( olt->cfg.svrId, 0x00, sizeof( olt->cfg.svrId ) );
    for( i = 0; i < argc ; i++ ) {
        if( 0 == strcmp(argv[i], "-i") ){
            i++;
            strcpy(olt->cfg.svrId, argv[i]);
        }
    }
    printf("#######1111[%s]\n" ,olt->cfg.svrId );
#endif

    // ���� Name
    sprintf(olt->cfg.svrname, "%s", svrName);

    // ���� ���μ��� ID
    olt->cfg.pid     = getpid();

    /** --------------------------------------------------------------------------------------------
     *    AFEC.ini ���� Application ��ü ���� ���� ��������
     *  --------------------------------------------------------------------------------------------
     */
    if ( !(node_cfg = getenv("SMIDCFG_AFC"))){
        fprintf(stdout, "_owkr_cfg_cfg) env SMIDCFG_AFC does not exist!\n");
        return -1;
    }

    /* node ���� */
    if (SUCC != afc_getProfileStr(tmp1, node_cfg, "NODE", "node_name")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "NODE", "node_name", node_cfg);
        return -1;
    }
    memcpy(olt->fw.sFwPrm.nodeName, tmp1, strlen(tmp1));

    /* Opmode ���� */
    if (SUCC != afc_getProfileStr(tmp1, node_cfg, "NODE", "op_mode")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "NODE", "op_mode", node_cfg);
        return -1;
    }
    olt->fw.sFwPrm.nodeType = tmp1[0];

    /* OLTP ���� cfg dir ���� */
    if (SUCC != afc_getProfileStr(olt->cfg.cfgDir, node_cfg, "OLTP", "cfg_dir")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "OLTP", "cfg_dir", node_cfg);
        return -1;
    }

    /* OLTP log base dir ���� */
    if (SUCC != afc_getProfileStr(olt->cfg.logDir, node_cfg, "OLTP", "log_dir")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "OLTP", "log_dir", node_cfg);
        return -1;
    }

    /* OLTP Core log dir ���� */
    if (SUCC != afc_getProfileStr(olt->cfg.coreDir, node_cfg, "OLTP", "core_dir")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "OLTP", "core_dir", node_cfg);
        return -1;
    }
    // �ھ� �α� ���丮 ����
    chdir(olt->cfg.coreDir);

    //���� cfg ���� ��ġ
    sprintf(path, "%s/%s.cfg", olt->cfg.cfgDir, olt->cfg.svrname);

    // DB ���� �ʿ� ����
    if (SUCC != afc_getProfileStr(tmp1, path, "TX", "tx_req")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "TX", "tx_req", path);
        return -1;
    }
    olt->cfg.txReq = tmp1[0];

    // XA ��� ��� ����
    if (SUCC != afc_getProfileStr(tmp1, path, "TX", "xa_mode")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "TX", "xa_mode", path);
        return -1;
    }
    olt->cfg.xaMode = tmp1[0];

    // XA Timeout
    olt->cfg.xaTimeout = afc_getProfileInt(path, "TX", "xa_timeout", 0);

    // RM(Resource manager) ��� ����
    if (SUCC != afc_getProfileStr(tmp1, path, "TX", "use_rm")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "TX", "use_rm", path);
        return -1;
    }
    olt->cfg.useRm = tmp1[0];

    // Native DB Connection Name
    if (SUCC != afc_getProfileStr(olt->cfg.dbUsr, path, "TX", "db_usr")) {
        if( olt->cfg.txReq == 'Y' && olt->cfg.useRm == 'N' ){
            fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
                "TX", "db_usr", path);
            return -1;
        }
    }

    // GCA ��� ����
    if (SUCC != afc_getProfileStr(tmp1, path, "FEATURE", "gca")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "FEATURE", "gca", path);
        return -1;
    }
    olt->cfg.useGca = tmp1[0];


    // Default Log flag
    if (SUCC != afc_getProfileStr(tmp1, path, "LOG", "log_flag")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "LOG", "log_flag", path);
        return -1;
    }
    olt->cfg.dftlogFlag = _st_owkr_cfg_str2LogFlag(tmp1);


    // Default Log Level : log module
    if (SUCC != afc_getProfileStr(tmp1, path, "LOG", "log_module")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "LOG", "log_module", path);
        return -1;
    }

    // Default Log Level
    if (SUCC != afc_getProfileStr(tmp2, path, "LOG", "log_level")) {
        fprintf(stdout, "_owkr_cfg_cfg) no %s in section [%s] of file(%s)!\n",
            "LOG", "log_level", path);
        return -1;
    }

    olt->cfg.dftlogLvl = _st_owkr_cfg_str2LogLevel(olt->cfg.dftlogFlag, tmp1, tmp2);
    afc_setUsrLogLvl(olt->cfg.dftlogLvl);

    // �α� �������� SETTING
    afc_get_date(olt->cfg.logBase);

    // �α� ���� OPEN
    if( 0 > _owkr_cfg_logfd_open(olt) )
        return -1;

    return 0;
}


/**
 * -------------------------------------------------------------------------------------------------
 *  #�Լ�     �� : _st_owkr_cfg_str2LogFlag
 *  #�Լ� ��  �� : �α� Flag�� String �� log flag�� ��ȯ�Ѵ�.
 *  #�Լ� Input  : char *str ( OLTP logflag string )
 *                 ������� Tx In / Tx Out / Sms / Int / Trace / User /
 *  #�Լ� Output : NONE
 *  #�Լ� Return : char    logFlag
 * -------------------------------------------------------------------------------------------------
**/
char    _st_owkr_cfg_str2LogFlag    ( char *str )
{
    int        i;
    int     strLen     =     strlen(str);
    char    ret        =    0x00;

    if( strLen > 6 ) strLen = 6;

    for( i = 0 ; i < strLen ; i++ ){
        if( *(str + i) == 'Y' ){
            switch( i ){
                case 0:
                    ret = ret | AFC_OLTP_LOG_TXIN;
                    break;
                case 1:
                    ret = ret | AFC_OLTP_LOG_TXOUT;
                    break;
                case 2:
                    ret = ret | AFC_OLTP_LOG_SMS;
                    break;
                case 3:
                    ret = ret | AFC_OLTP_LOG_INTF;
                    break;
                case 4:
                    ret = ret | AFC_OLTP_LOG_TRC;
                    break;
                case 5:
                    ret = ret | AFC_OLTP_LOG_USER;
                    break;
                default:
                    break;
            }
        }
    }

    return ret;
}

/**
 * -------------------------------------------------------------------------------------------------
 *  #�Լ�     �� : _st_owkr_cfg_str2LogLevel
 *  #�Լ� ��  �� : �α� write module�� �α� level�� String�� log level�� ��ȯ�Ѵ�.
 *  #�Լ� Input  : char    logFlag  : �α� �÷��� ��
 *                 char *logMod  : Log write module
 *                     A:AMM, B:BAM, C:CAM, D:DAM
 *                 char *logLvl  : Log level
 *                     E:Error, W:Warning, I:Information, D:Debug
 *  #�Լ� Output : NONE
 *  #�Լ� Return : char    log Level
 * -------------------------------------------------------------------------------------------------
**/
char    _st_owkr_cfg_str2LogLevel    ( char logFlag, char *logMod, char *logLvl )
{
    int        i;
    int     strLen     =     strlen(logMod);
    char    ret        =    0x00;

    if( !(logFlag && AFC_OLTP_LOG_USER) ){
        // Log flag���� ����ڷα׸� ������� ������..
        // log level�� 0x00���� ����
        return ret;
    }

    if( strLen > 4 ) strLen = 4;

    // Module ���� �� Log level
    for( i = 0 ; i < strLen ; i++ ){
        switch( *(logMod + i) ){
            case 'A':
                ret = ret | LOG_AMM;
                break;
            case 'B':
                ret = ret | LOG_BAM;
                break;
            case 'C':
                ret = ret | LOG_CAM;
                break;
            case 'D':
                ret = ret | LOG_DAM;
                break;
            default :
                break;
        }
    }

    // �α� ������ �α׷���
    switch( *logLvl ){
        case 'E':
            ret = ret | LOG_ERROR;
            break;
        case 'W':
            ret = ret | LOG_WARN;
            break;
        case 'I':
            ret = ret | LOG_INFO;
            break;
        case 'D':
            ret = ret | LOG_DEBUG;
            break;
        default:
            break;
    }

    return ret;
}


/**
 * -------------------------------------------------------------------------------------------------
 *  #�Լ�     �� : _owkr_cfg_logfd_close
 *  #�Լ� ��  �� : �α׿� FD�� �ݴ´�.
 *  #�Լ� Input  : _OLTP_CM *olt : OLTP ���� ���
 *  #�Լ� Output : NONE
 *  #�Լ� Return : ���� ���� ����
 * -------------------------------------------------------------------------------------------------
**/
int        _owkr_cfg_logfd_close    ( _OLTP_CM *olt )
{
    // TX IN LOG
    if( NULL != olt->cfg.fpTxIn )    fclose( olt->cfg.fpTxIn );

    // TX OUT LOG
    if( NULL != olt->cfg.fpTxOt )    fclose( olt->cfg.fpTxOt );

    // SMS LOG
    if( NULL != olt->cfg.fpSms )    fclose( olt->cfg.fpSms  );

    // USER LOG
    if( NULL != olt->fw.fpUsr  )    fclose( olt->fw.fpUsr   );

    if( 0 < olt->fw.fdUsr      )    close( olt->fw.fdUsr    );

    // TRACE LOG
    if( 'D' == olt->fw.sFwPrm.nodeType ){
        if( NULL != olt->fw.fpTrc )    fclose( olt->fw.fpTrc   );
    }

    // INTERFACE LOG
    if( NULL != olt->fw.fpItf )        fclose( olt->fw.fpItf   );

    return 0;
}

/**
 * -------------------------------------------------------------------------------------------------
 *  #�Լ�     �� : _owkr_cfg_logfd_open
 *  #�Լ� ��  �� : �α׿� FD�� ����.
 *  #�Լ� Input  : _OLTP_CM *olt : OLTP ���� ���
 *  #�Լ� Output : NONE
 *  #�Լ� Return : ���� ���� ����
 * -------------------------------------------------------------------------------------------------
**/
int        _owkr_cfg_logfd_open    ( _OLTP_CM *olt )
{
    char    fileName [ 255 ];

    umask(022);

    // TX IN LOG
    sprintf(fileName, "%s/%s/%s_%s_%s.log"
        , olt->cfg.logDir, AFC_OLTP_LOG_TXIN_STR, olt->cfg.svrname, olt->cfg.svrId, olt->cfg.logBase);
    if ( (olt->cfg.fpTxIn = fopen(fileName, "a")) == NULL )
    {
        olt->cfg.fpTxIn = NULL;
        fprintf(stdout, "Tx In Log File Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }

    // TX OUT LOG
    sprintf(fileName, "%s/%s/%s_%s_%s.log"
        , olt->cfg.logDir, AFC_OLTP_LOG_TXOUT_STR, olt->cfg.svrname, olt->cfg.svrId, olt->cfg.logBase);
    if ( (olt->cfg.fpTxOt = fopen(fileName, "a")) == NULL )
    {
        olt->cfg.fpTxOt = NULL;
        fprintf(stdout, "Tx Out Log File Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }

    // SMS LOG
    sprintf(fileName, "%s/%s/%s_%s_%s.log"
        , olt->cfg.logDir, AFC_OLTP_LOG_SMS_STR, olt->cfg.svrname, olt->cfg.svrId, olt->cfg.logBase);
    if ( (olt->cfg.fpSms = fopen(fileName, "a")) == NULL )
    {
        olt->cfg.fpSms = NULL;
        fprintf(stdout, "SMS Log File Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }

    // USER LOG
    sprintf(fileName, "%s/%s/%s_%s_%s.log"
        , olt->cfg.logDir, AFC_OLTP_LOG_USER_STR, olt->cfg.svrname, olt->cfg.svrId, olt->cfg.logBase);
    /* stdout, stderr dbglog�� redirect - START - */
    /* stdout -> dbg_log ���Ϸ� �ϱ� ���Ͽ� userlog�� FD open */
    if ( 0 > ( olt->fw.fdUsr = open( fileName, O_RDWR | O_CREAT | O_APPEND, 0644))){
        olt->fw.fdUsr = -1;
        fprintf(stdout, "USER Log File FD Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }
    close(1);
    dup2(olt->fw.fdUsr, 1);
    close(2);
    dup2(olt->fw.fdUsr, 2);
    /* stdout, stderr dbglog�� redirect - END - */

    //if ( (olt->fw.fpUsr = fopen(fileName, "a")) == NULL )
    if( (olt->fw.fpUsr = fdopen( olt->fw.fdUsr, "a+" ) ) == NULL )
    {
        olt->fw.fpUsr = NULL;
        fprintf(stdout, "USER Log File Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }
    afc_setUsrLogFP(olt->fw.fpUsr);
    afc_setUsrLogger(afc_usrLogger_file);

    // TRACE LOG
    sprintf(fileName, "%s/%s/%s_%s_%s.log"
        , olt->cfg.logDir, AFC_OLTP_LOG_TRC_STR, olt->cfg.svrname, olt->cfg.svrId, olt->cfg.logBase);
    if ( (olt->fw.fpTrc = fopen(fileName, "a")) == NULL )
    {
        olt->fw.fpTrc = NULL;
        fprintf(stdout, "TRACE Log File Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }
    afc_setTrcLogFP(olt->fw.fpTrc);
    afc_setTrcLogger(afc_trcLogger_file);

    // INTERFACE LOG
    sprintf(fileName, "%s/%s/%s_%s_%s.log"
        , olt->cfg.logDir, AFC_OLTP_LOG_INTF_STR, olt->cfg.svrname, olt->cfg.svrId, olt->cfg.logBase);
    if ( (olt->fw.fpItf = fopen(fileName, "a")) == NULL )
    {
        olt->fw.fpItf = NULL;
        fprintf(stdout, "Interface Log File Open Error(%s)\n", fileName); fflush(stdout);
        return -1;
    }
    afc_setItfLogFP(olt->fw.fpItf);
    afc_setItfLogger(afc_itfLogger_file);

    return 0;
}

/**
 * =================================================================================================
 *     >>>>>>>>>>     AFEC Oltp Framework Core worker [owkr_cfg.c] File   E N D !    <<<<<<<<<<
 * =================================================================================================
**/
