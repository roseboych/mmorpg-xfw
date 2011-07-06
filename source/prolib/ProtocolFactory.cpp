/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include <Windows.h>
#include <vector>
#include "prolib/ProtocolFactory.h"
#include "prolib/SerializeFactoryContainer.h"
#include "prolib/Pro_all.h"

USE_PROTOCOL_NAMESPACE

ProtocolFactory::ProtocolFactory(void)
{}

//----------------------------协议映射----------------------------//
BEGIN_PRO_FACTORY()
	//pro_boot.h
	PRO_MAPPING( SVR_REGIST_REQ, Pro_SvrRegist_req)
	PRO_MAPPING( SVR_REGIST_ACK, Pro_SvrRegist_ack)
	PRO_MAPPING( SVR_ONLINETICK_NTF, Pro_SvrOnlineTick_NTF)
	PRO_MAPPING( SVR_SVRTEAMINFO_NTF, Pro_SvrTeamInfo_NTF)
	PRO_MAPPING( SVR_SELTEAMTIMEOUT_NTF, Pro_SvrSelTeamTimeout_NTF)
	PRO_MAPPING( SVR_SELSERVER_REQ, Pro_SvrSelTeam_req)
	PRO_MAPPING( SVR_SELSERVER_ACK, Pro_SvrSelTeam_ack)
	PRO_MAPPING( SVR_SELSERVERPROXY_REQ, Pro_SvrSelTeamProxy_req)
	PRO_MAPPING( SVR_SELSERVERPROXY_ACK, Pro_SvrSelTeamProxy_ack)
	PRO_MAPPING( SVR_SELSERVERCONFIRM_REQ, Pro_SvrSelTeamConfirm_req)
	PRO_MAPPING( SVR_SELSERVERCONFIRM_ACK, Pro_SvrSelTeamConfirm_ack)
	PRO_MAPPING( SVR_QUITSERVER_REQ, Pro_SvrQuitTeam_req)
	PRO_MAPPING( SVR_QUITSERVER_ACK, Pro_SvrQuitTeam_ack)
	PRO_MAPPING( SVR_QUITSERVERRECONN_REQ, Pro_SvrQuitTeamReconn_req)
	PRO_MAPPING( SVR_QUITSERVERRECONN_ACK, Pro_SvrQuitTeamReconn_ack)
	PRO_MAPPING( SVR_INSTUPDATE_NTF, Pro_SvrInstUpdate_ntf)
	PRO_MAPPING( SVR_USERLOST_NTF, Pro_SvrUserLost_NTF)
	PRO_MAPPING( GAME_LOGIN_REQ, Pro_Login_req)
	PRO_MAPPING( GAME_LOGIN_ACK, Pro_Login_ack)
	PRO_MAPPING( GAME_USERREGIST_NTF, Pro_UserRegist_ntf)
	PRO_MAPPING( GAME_LOGOUT_REQ, Pro_Logout_req)
	PRO_MAPPING( GAME_LOGOUT_NTF, Pro_Logout_ntf)
	PRO_MAPPING( SVR_DISCONNKEEP_NTF, Pro_SvrDisconnKeep_req)
	PRO_MAPPING( SVR_LNKEEPLOAD_NTF, Pro_SvrLnkkeepLoad_req)
	
	//server_controller.h
	PRO_MAPPING( CONTROLLER_LOGIN_REQ, Pro_CtrlLogin_req)
	PRO_MAPPING( CONTROLLER_LOGIN_ACK, Pro_CtrlLogin_ack)
	PRO_MAPPING( CONTROLLER_PROFILER_FILTERCTRL, Pro_CtrlProfilerFilterCtrl)
	PRO_MAPPING( CONTROLLER_PROFILER_ANALYSISCTRL, Pro_CtrlProfilerAnalysisCtrl)

	//pro_chr.h
	PRO_MAPPING( GAME_CHRLIST_REQ, Pro_ChrList_req)
	PRO_MAPPING( GAME_CHRLIST_ACK, Pro_ChrList_ack)
	PRO_MAPPING( GAME_CHRCREATE_REQ, Pro_ChrCreate_req)
	PRO_MAPPING( GAME_CHRCREATE_ACK, Pro_ChrCreate_ack)
	PRO_MAPPING( GAME_CHRSEL_REQ, Pro_ChrSel_req)
	PRO_MAPPING( GAME_CHRSEL_ACK, Pro_ChrSel_ack)
	PRO_MAPPING( GAME_CHRDEL_REQ, Pro_ChrDel_req)
	PRO_MAPPING( GAME_CHRDEL_ACK, Pro_ChrDel_ack)
	PRO_MAPPING( GAME_CHRREGTOWORLD_REQ, Pro_ChrRegistToWorld_req)
	PRO_MAPPING( GAME_CHRLOAD_REQ, Pro_ChrLoad_req)
	PRO_MAPPING( GAME_CHRLOAD_ACK, Pro_ChrLoad_ack)
	PRO_MAPPING( GAME_CHRFIN_NTF, Pro_ChrFin_NTF)

	//pro_aoi.h
	PRO_MAPPING( AOI_UNITENTER_NTF, Pro_AppUnitEnter_ntf)
	PRO_MAPPING( AOI_UNITLEAVE_NTF, Pro_AppUnitLeave_ntf)
	PRO_MAPPING( AOI_TELEPORT_REQ, Pro_AppTeleport_req)
	PRO_MAPPING( AOI_TELEPORT_ACK, Pro_AppTeleport_ack)
	PRO_MAPPING( AOI_ENTERINS_REQ, Pro_AppEnterIns_req)
	PRO_MAPPING( AOI_ENTERINS_ACK, Pro_AppEnterIns_ack)
	PRO_MAPPING( AOI_ENTERINSOT_NTF, Pro_AppEnterInsOvertime_ntf)
	
	PRO_MAPPING( AOI_ENTERINSPREALLOC_REQ, Pro_AppEnterInsReqAlloc_req)
	PRO_MAPPING( AOI_ENTERINSPREALLOC_ACK, Pro_AppEnterInsReqAlloc_ack)
	PRO_MAPPING( AOI_ENTERINSCONFIRM_REQ, Pro_AppEnterInsConfirm_req)

	//app/pro_avatar.h
	PRO_MAPPING( APP_EQUIPITEM_REQ, Pro_EquipItem_req)
	PRO_MAPPING( APP_EQUIPITEM_ACK, Pro_EquipItem_ack)
	PRO_MAPPING( APP_EQUIPITEM_NTF, Pro_EquipItem_ntf)

	//app/pro_pet.h
	PRO_MAPPING( APP_PETSUMMON_NTF, Pro_PetSummon_ntf)
	PRO_MAPPING( APP_PETSUMMON_REQ, Pro_PetSummon_req)
	PRO_MAPPING( APP_PETSUMMON_ACK, Pro_PetSummon_ack)

	//app/pro_chat.h
	PRO_MAPPING( APP_TEAMCHAT_REQ, Pro_TeamChat_req)
	PRO_MAPPING( APP_TEAMCHAT_NTF, Pro_TeamChat_ntf)
	PRO_MAPPING( APP_P2PCHAT_REQ, Pro_P2PChat_req)
	PRO_MAPPING( APP_P2PCHAT_NTF, Pro_P2PChat_ntf)

	//app/pro_buffer.h
	PRO_MAPPING( APP_BUFFCHANGE_NTF, Pro_BuffChange_ntf)

	//app/pro_skill.h
	PRO_MAPPING( APP_SKILLINIT_NTF, Pro_SkillInit_ntf)
	PRO_MAPPING( APP_SKILLSTATECHG_NTF, Pro_SkillStateChg_ntf)
	PRO_MAPPING( APP_SKILLSTUDY_REQ, Pro_SkillStudy_req)
	PRO_MAPPING( APP_SKILLSTUDY_ACK, Pro_SkillStudy_ack)
	PRO_MAPPING( APP_SKILLUSED_REQ, Pro_SkillUsed_req)
	PRO_MAPPING( APP_SKILLUSED_ACK, Pro_SkillUsed_ack)

	//app/pro_state.h
	PRO_MAPPING( DB_POSROTSAVE_NTF, Pro_DBPosRotSave_ntf)
	PRO_MAPPING( DB_PLAYERBASEINFO_SAVE, Pro_DBPlayerBaseInfo_sav)

	PRO_MAPPING( APP_PLAYERMOVE_REQ, Pro_PlayerMove_req)
	PRO_MAPPING( APP_PLAYERMOVE_ACK, Pro_PlayerMove_ack)
	PRO_MAPPING( APP_PLAYERMOVE_NTF, Pro_PlayerMove_ntf)

END_PRO_FACTORY()
