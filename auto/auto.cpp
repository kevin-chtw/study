#include <map>
#include <string>
#include <iostream>
#include <list>

using namespace std;

class T1
{
public:
    static int get()
    {
        return 10;
    }
};

class T2
{
public:
    static string get()
    {
        return "hello, world";
    }
};

template <class T>
void func(void)
{
    auto val = T::get();
    cout << "val: " << val << endl;
}

void container()
{
    map<int, string> mp = {
        {1, "ace"},
        {2, "sabo"},
        {3, "luffy"},
    };

    for (auto it = mp.begin(); it != mp.end(); ++it)
    {
        cout << "key:" << it->first << " value:" << it->second << endl;
    }
}

template <typename T>
class Container
{
public:
    void print(T &t)
    {
        for (m_it = t.begin(); m_it != t.end(); ++m_it)
        {
            cout << "value: " << *m_it << endl;
        }
    }

private:
    decltype(T().begin()) m_it;
};

template <typename T, typename U>
auto add(T t, U u) -> decltype(t)
{
    return t + u;
}

int main()
{
    // auto
    // 1、用于容器遍历
    container();

    // 2、用于泛型编程，获取返回值类型进行自动推导
    func<T1>();
    func<T2>();

    // decltype 用于泛型编程
    list<int> ls{1, 2, 3, 4, 5, 6, 7};
    Container<list<int>> c;
    c.print(ls);

    // 返回类型后置
    int x = 520;
    double y = 13.14;
    auto ret = add(x, y);
    cout << "ret:" << ret << endl;
    return 0;
}
