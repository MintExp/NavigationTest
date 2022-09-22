#pragma once

template<class T>
class TSingleton
{
public:
    //创建单件
    static void Create()
    {
        if ( !ms_pObject )
        {
            ms_pObject = new T;
        }
    }

    //销毁单件
    static void Destroy()
    {
        if ( ms_pObject )
        {
            delete ms_pObject;
            ms_pObject = NULL;
        }
    }

    //取得单件实例
    static T* Get()
    {
        return ms_pObject;
    }

protected:
    static T*	ms_pObject;
};

template<class T> 
T* TSingleton<T>::ms_pObject = NULL;
