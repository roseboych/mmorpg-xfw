/**
* css application
*
* @category		cmsvr module
* @package		
* @author		Created by Lu Jianfeng, 2009
*/
inline 
MapPane* SceneManagePaneImpl::get_mappane( int column, int row)
{
	if( column < pane_column_min_ || column > pane_column_max_)
		return 0;
	if( row < pane_row_min_ || row > pane_row_max_)
		return 0;

	return &(panes_[(row - pane_row_min_)*pane_columns_ + (column - pane_column_min_)]);
}

inline 
MapPane* SceneManagePaneImpl::get_mappanebyxy( float x, float y)
{
	int c =POSX2PANEX(x);
	int r =POSY2PANEY(y);

	return get_mappane( c, r);
}

inline 
void SceneManagePaneImpl::get_playerviewpanes( int c, int r, NS_STL::vector<MapPane*>& mp)
{
	//获取9屏信息
	int ii =r-VIEW_PLANE_RADIUS;
	if( ii < pane_row_min_) 
		ii =pane_row_min_;
	int ii2 =r + VIEW_PLANE_RADIUS;
	if( ii2 > pane_row_max_)
		ii2 =pane_row_max_;
	for( ; ii <= ii2; ++ii)
	{
		int jj =c-VIEW_PLANE_RADIUS;
		if( jj < pane_column_min_) 
			jj =pane_column_min_;
		int jj2 =c+VIEW_PLANE_RADIUS;
		if( jj2 > pane_column_max_)
			jj2 =pane_column_max_;

		for( ; jj <= jj2; ++jj)
		{
			MapPane* p =get_mappane( jj, ii);
			if( p == 0)
				continue;

			mp.push_back( p);
		}
	}
}

inline 
void SceneManagePaneImpl::get_playerviewpanes( int x1, int y1, int x2, int y2, NS_STL::vector<MapPane*>& mp)
{
	x1 =(x1 < pane_column_min_? pane_column_min_ : x1);
	x2 =(x2 > pane_column_max_? pane_column_max_ : x2);
	y1 =(y1 < pane_row_min_? pane_row_min_ : y1);
	y2 =(y2 > pane_row_max_? pane_row_max_ : y2);

	for( int ii =x1; ii <= x2; ++ii)
	{
		for( int jj =y1; jj <= y2; ++jj)
		{
			MapPane* p =get_mappane( ii, jj);
			if( p == 0)
				continue;

			mp.push_back( p);
		}
	}
}
