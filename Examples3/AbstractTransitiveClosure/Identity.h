#pragma  once



template<typename DERIVED>
struct ImbueIdentity
{
    DERIVED Identity()
    {
        return static_cast<DERIVED*>(this)->Identity();
    }
};


template<typename DERIVED>
struct ImbueUnity
{
    DERIVED Unity()
    {
        return static_cast<DERIVED*>(this)->Unity();
    }
};