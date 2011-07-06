/**
* css application
*
* @category		scene module
* @package		
* @author		Created by Lu Jianfeng, 2010
*/
inline 
MapPane* ActorSceneExtPane::get_curpane()
{
	return cur_pane_;
}

inline
void ActorSceneExtPane::clear_viewinout_tmp()
{
	in_view_objs_.clear();
	out_view_objs_.clear();
}

inline 
void ActorSceneExtPane::change_oldpane()
{
	oldpanex_ =POSX2PANEX( owner_actor_->get_positionx());
	oldpaney_ =POSY2PANEY( owner_actor_->get_positiony());
}

inline
void ActorSceneExtPane::join_inview( UnitBase* pobj)
{
	in_view_objs_[pobj->get_uuid()] =pobj;
}

inline
void ActorSceneExtPane::join_outview( UnitBase* pobj)
{
	S_INT_64 iid =pobj->get_uuid();

	//如果已经在in视图中那么清除out视图
	UNITOBJECT_MAP::iterator fiter =in_view_objs_.find( iid);
	if( fiter == in_view_objs_.end())
		out_view_objs_[iid] =pobj;
	else
		in_view_objs_.erase( fiter);
}

inline 
bool ActorSceneExtPane::get_moveinregion( pane_region* r_enter)
{
	S_INT_32 panex1 =POSX2PANEX( owner_actor_->get_positionx());
	S_INT_32 paney1 =POSY2PANEY( owner_actor_->get_positiony());

	//没有移动位置
	if( panex1 == oldpanex_ && paney1 == oldpaney_)
		return false;

	//计算4个矩形区域
	int oldx =oldpanex_, oldy =oldpaney_;
	int curx =panex1, cury =paney1;
	int ux =::abs(curx - oldx);
	int uy =::abs(cury - oldy);

	//left
	if( curx > oldpanex_)
	{
		r_enter[0].x1 =curx + VIEW_PLANE_RADIUS + 1;
		r_enter[0].y1 =cury - VIEW_PLANE_RADIUS;
		r_enter[0].x2 =curx + VIEW_PLANE_RADIUS;
		r_enter[0].y2 =cury + VIEW_PLANE_RADIUS;
		r_enter[0].x1 -= ux;
	}
	else
	{
		r_enter[0].x1 =curx - VIEW_PLANE_RADIUS;
		r_enter[0].y1 =cury - VIEW_PLANE_RADIUS;
		r_enter[0].x2 =curx - VIEW_PLANE_RADIUS - 1;
		r_enter[0].y2 =cury + VIEW_PLANE_RADIUS;
		r_enter[0].x2 += ux;
	}
	//top
	if( cury > oldy)
	{
		r_enter[1].x1 =curx - VIEW_PLANE_RADIUS;
		r_enter[1].y1 =cury + VIEW_PLANE_RADIUS + 1;
		r_enter[1].x2 =curx + VIEW_PLANE_RADIUS;
		r_enter[1].y2 =cury + VIEW_PLANE_RADIUS;
		r_enter[1].y1 -= uy;
		if( curx > oldx)
			r_enter[1].x2 -= ux;
		else
			r_enter[1].x1 += ux;
	}
	else
	{
		r_enter[1].x1 =curx - VIEW_PLANE_RADIUS;
		r_enter[1].y1 =cury - VIEW_PLANE_RADIUS;
		r_enter[1].x2 =curx + VIEW_PLANE_RADIUS;
		r_enter[1].y2 =cury - VIEW_PLANE_RADIUS - 1;
		r_enter[1].y2 += uy;
		if( curx > oldx)
			r_enter[1].x2 -= ux;
		else
			r_enter[1].x1 += ux;
	}

	return true;
}

inline 
bool ActorSceneExtPane::get_moveoutregion( pane_region* r_exit)
{
	S_INT_32 panex1 =POSX2PANEX( owner_actor_->get_positionx());
	S_INT_32 paney1 =POSY2PANEY( owner_actor_->get_positiony());

	//没有移动位置
	if( panex1 == oldpanex_ && paney1 == oldpaney_)
		return false;

	//计算4个矩形区域
	int oldx =oldpanex_, oldy =oldpaney_;
	int curx =panex1, cury =paney1;
	int ux =::abs(curx - oldx);
	int uy =::abs(cury - oldy);

	//left
	if( curx > oldpanex_)
	{
		r_exit[0].x1 =oldx - VIEW_PLANE_RADIUS;
		r_exit[0].y1 =oldy - VIEW_PLANE_RADIUS;
		r_exit[0].x2 =oldx - VIEW_PLANE_RADIUS - 1;
		r_exit[0].y2 =oldy + VIEW_PLANE_RADIUS;
		r_exit[0].x2 += ux;
	}
	else
	{
		r_exit[0].x1 =oldx + VIEW_PLANE_RADIUS + 1;
		r_exit[0].y1 =oldy - VIEW_PLANE_RADIUS;
		r_exit[0].x2 =oldx + VIEW_PLANE_RADIUS;
		r_exit[0].y2 =oldy + VIEW_PLANE_RADIUS;
		r_exit[0].x1 -= ux;
	}
	//top
	if( cury > oldy)
	{
		r_exit[1].x1 =oldx - VIEW_PLANE_RADIUS;
		r_exit[1].y1 =oldy - VIEW_PLANE_RADIUS;
		r_exit[1].x2 =oldx + VIEW_PLANE_RADIUS;
		r_exit[1].y2 =oldy - VIEW_PLANE_RADIUS - 1;
		r_exit[1].y2 += uy;
		if( curx > oldx)
			r_exit[1].x1 += ux;
		else
			r_exit[1].x2 -= ux;
	}
	else
	{
		r_exit[1].x1 =oldx - VIEW_PLANE_RADIUS;
		r_exit[1].y1 =oldy + VIEW_PLANE_RADIUS + 1;
		r_exit[1].x2 =oldx + VIEW_PLANE_RADIUS;
		r_exit[1].y2 =oldy + VIEW_PLANE_RADIUS;
		r_exit[1].y1 -= uy;
		if( curx > oldx)
			r_exit[1].x1 += ux;
		else
			r_exit[1].x2 -= ux;
	}

	return true;
}