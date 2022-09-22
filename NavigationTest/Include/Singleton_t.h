#pragma once

template<class T>
class TSingleton
{
public:
    //��������
    static void Create()
    {
        if ( !ms_pObject )
        {
            ms_pObject = new T;
        }
    }

    //���ٵ���
    static void Destroy()
    {
        if ( ms_pObject )
        {
            delete ms_pObject;
            ms_pObject = NULL;
        }
    }

    //ȡ�õ���ʵ��
    static T* Get()
    {
        return ms_pObject;
    }

protected:
    static T*	ms_pObject;
};

template<class T> 
T* TSingleton<T>::ms_pObject = NULL;
