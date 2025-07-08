#ifndef __TKCLASSFUNCTOR_H__
#define __TKCLASSFUNCTOR_H__

namespace tkclsfunctor
{
//
template <class ret_type, class arg_type>
class function_base
{
public:
    function_base()
    {
    }
    virtual ~function_base()
    {
    }
    virtual ret_type operator()(arg_type) = 0;
};

//
template <class CS, class ret_type, class arg_type>
class function_impl : public function_base<ret_type, arg_type>
{
public:
    typedef ret_type (CS::*PROC)(arg_type);
    function_impl(CS* obj, PROC proc) : obj_(obj), proc_(proc)
    {
    }
    ret_type operator()(arg_type arg)
    {
        return (obj_->*proc_)(arg);
    }

private:
    CS* obj_;
    PROC proc_;
};

//
template <class CS, class ret_type, class arg_type>
function_base<ret_type, arg_type>* bind(ret_type (CS::*proc)(arg_type), CS* pc)
{
    return new function_impl<CS, ret_type, arg_type>(pc, proc);
}

//
template <class ret_type, class arg_type, class param_type>
class function_base_ex
{
public:
    function_base_ex()
    {
    }
    virtual ~function_base_ex()
    {
    }
    virtual ret_type operator()(arg_type, param_type) = 0;
};

//
template <class CS, class ret_type, class arg_type, class param_type>
class function_impl_ex : public function_base_ex<ret_type, arg_type, param_type>
{
public:
    typedef ret_type (CS::*PROC)(arg_type, param_type);
    function_impl_ex(CS* obj, PROC proc) : obj_(obj), proc_(proc)
    {
    }
    ret_type operator()(arg_type arg, param_type param)
    {
        return (obj_->*proc_)(arg, param);
    }

private:
    CS* obj_;
    PROC proc_;
};

//
template <class CS, class ret_type, class arg_type, class param_type>
function_base_ex<ret_type, arg_type, param_type>* bind_ex(ret_type (CS::*proc)(arg_type, param_type), CS* pc)
{
    return new function_impl_ex<CS, ret_type, arg_type, param_type>(pc, proc);
}

}  // namespace tkclsfunctor

// end namespace tkclsfunctor

#endif  //__TKCLASSFUNCTOR_H__
