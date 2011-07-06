/**
* share module
* 
* 单利实现
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SINGLETONT__H__
#define __SINGLETONT__H__

/**
* @class SingletonT
* 
* @brief 单例类
**/
template< class T>
class SingletonT
{
private:
	struct object_creator{
		object_creator(){ SingletonT<T>::instance();}

		inline void do_nothing()const{}
	};

	static object_creator creator_;

public:
	SingletonT();

	inline static T& instance();
};

template< class T>
SingletonT<T>::SingletonT()
{
}

template< class T>
T& SingletonT<T>::instance()
{
	static T ins;

	//确保在main入口之前执行
	creator_.do_nothing();

	return ins;
}

#endif	//__SINGLETONT__H__
