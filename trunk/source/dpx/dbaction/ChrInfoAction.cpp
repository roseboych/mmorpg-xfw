/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ChrInfoAction.h"

#include <corelib/corelibdef.h>
#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "../player/Player.h"
#include "../DPXModule.h"

USE_PROTOCOL_NAMESPACE;

ChrInfoAction::ChrInfoAction( S_TIMESTAMP s):inherit( DBACTION_CHRGETINFO, s),
player_( 0)
{
}

ChrInfoAction::~ChrInfoAction()
{
	release();
}

void ChrInfoAction::release()
{
	for( EQUIPITEM_LIST::iterator iter =items_.begin(); iter != items_.end(); ++iter)
		delete (*iter);
	items_.clear();

	for( PETDATA_LIST::iterator iter =pets_.begin(); iter != pets_.end(); ++iter)
		delete (*iter);
	pets_.clear();

	for( BUFFERDATA_LIST::iterator iter =buffers_.begin(); iter != buffers_.end(); ++iter)
		delete (*iter);
	buffers_.clear();
}

bool ChrInfoAction::validate()
{
	if( player_ == 0 || !player_->is_samesession( uuid_))
		return false;

	return true;
}

void ChrInfoAction::run_indbpool( otl_connect *pconn)
{
	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		//获取角色数据
		otl_stream cur( 50, DBPROC_CHRGETINFO, *pconn, otl_implicit_select);

		cur << chrid_;

		S_INT_16 sv, rc, pf, bagslots, dv;
		if( !cur.eof())
		{
			cur >> baseinfo_.name_ >> rc >> pf >> sv >>baseinfo_.moneys_ >> baseinfo_.level_
				>> bagslots >> baseinfo_.skill_[0] >> baseinfo_.skill_[1] >> baseinfo_.petid_
				>> baseinfo_.hp_ >> baseinfo_.mp_ >> baseinfo_.power_ >> baseinfo_.agile_
				>> baseinfo_.endurance_ >> baseinfo_.intellect_ >> baseinfo_.spirit_
				>> baseinfo_.armor_ >> baseinfo_.posx_ >> baseinfo_.posy_ >> baseinfo_.posz_ >> baseinfo_.facing_ >> dv
				>> baseinfo_.exp_;

			baseinfo_.race_ =rc;
			baseinfo_.profession_ =pf;
			baseinfo_.sex_ =sv;
			baseinfo_.bagslots_ =bagslots;
			baseinfo_.died_ =dv;
		}

		cur.close();

		//加载道具信息
		otl_stream cur2( 50, DBPROC_ITEMGETCHR, *pconn, otl_implicit_select);

		cur2 << chrid_;

		while( !cur2.eof())
		{
			PRO::equipment_item *pitem =FRAMEWK_NEW PRO::equipment_item;
			items_.push_back( pitem);

			cur2 >> pitem->itemid_ >> pitem->inavatar_ >> pitem->inpart_ >> pitem->itemcode_ 
				>> pitem->validatetype_ >> pitem->starttime_ >> pitem->endtime_;
		}

		cur2.close();

		//加载宠物信息
		otl_stream cur3( 50, DBPROC_PETGETLIST, *pconn, otl_implicit_select);

		cur3 << chrid_;

		while( !cur3.eof())
		{
			PRO::petdata_item *pitem =FRAMEWK_NEW PRO::petdata_item();
			pets_.push_back( pitem);

			cur3 >> pitem->petid_ >> pitem->petcode_ >> pitem->petname_;
		}

		cur3.close();

		//加载buffer信息
		otl_stream cur4( 50, DBPROC_BUFFERLIST, *pconn, otl_implicit_select);

		cur4 << chrid_;

		while( !cur4.eof())
		{
			PRO::bufferdata_item *pitem =FRAMEWK_NEW PRO::bufferdata_item;
			buffers_.push_back( pitem);

			S_INT_16 ty;
			cur4 >> pitem->bufferid_ >> ty >> pitem->createtime_ >> pitem->scopetime_;
			pitem->types_ =ty;
		}

		cur4.close();

		pconn->commit();
	}
	catch( otl_exception& e){
		pconn->rollback();
		release();

		MODULE_LOG_ERROR( MODULE_DATABASE, "ChrInfoAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
	}
}

void ChrInfoAction::run_inmainpool()
{
	//加载数据
	player_->chrdata_load( chrid_);
	player_->baseinfo_ =this->baseinfo_;
	for( EQUIPITEM_LIST::iterator iter =items_.begin(); iter != items_.end(); ++iter)
	{
		PRO::equipment_item* pitem =(*iter);
		player_->items_[pitem->itemid_] =pitem;
	}
	items_.clear();
	for( PETDATA_LIST::iterator iter =pets_.begin(); iter != pets_.end(); ++iter)
	{
		PRO::petdata_item* pitem =(*iter);
		player_->pets_[pitem->petid_] =pitem;
	}
	pets_.clear();
	for( BUFFERDATA_LIST::iterator iter =buffers_.begin(); iter != buffers_.end(); ++iter)
	{
		PRO::bufferdata_item* pitem =(*iter);
		player_->buffers_[pitem->bufferid_] =pitem;
	}
	buffers_.clear();

	//发送通知协议
	Pro_ChrLoad_ack* ack =PROTOCOL_NEW Pro_ChrLoad_ack();
	PRO_UUID_FILL( ack, player_->global_index_, player_->uuid_);

	for( Player::EQUIPITEMS_MAP::iterator iter =player_->items_.begin(); iter != player_->items_.end(); ++iter)
	{
		PRO::equipment_item* pitem =iter->second;
		ack->items_.push_back( *pitem);
	}
	for( Player::PETDATA_MAP::iterator iter =player_->pets_.begin(); iter != player_->pets_.end(); ++iter)
	{
		PRO::petdata_item* ppet =iter->second;
		ack->pets_.push_back( *ppet);
	}
	for( Player::BUFFERDATA_MAP::iterator iter =player_->buffers_.begin(); iter != player_->buffers_.end(); ++iter)
	{
		PRO::bufferdata_item* ppet =iter->second;
		ack->buffers_.push_back( *ppet);
	}

	player_->send_to_css( ack);

	Pro_ChrFin_NTF* ntf =PROTOCOL_NEW Pro_ChrFin_NTF();
	PRO_UUID_FILL( ntf, player_->global_index_, player_->uuid_);
	ntf->chrid_ =player_->chrid_;
	ntf->baseinfo_ =player_->baseinfo_;

	player_->send_to_css( ntf);
}
