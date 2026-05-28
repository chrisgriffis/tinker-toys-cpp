#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;

template<size_t CacheSize, class K, class V>
struct Cache
{

    Cache() = default;

    Cache& operator<<(const pair<K, V>& entry)
    {
        auto to_add = table_.find(entry.first);
        if (to_add == table_.end())
        {
            //add it, evict if needed
            if (table_.size() >= CacheSize)
            {
                //evict
                table_.erase(queue_.back());
                queue_.pop_back();
            }
            //add it
            queue_.emplace_front(entry.first);
            table_[entry.first] = make_pair(entry.second, queue_.begin());
        }
        else
        {
            //update its access time
            renew(entry.first);
        }
        return *this;
    }
    bool operator()(const K& k, V& v)
    {
        auto record = table_.find(k);
        if (record == table_.end())
        {
            return false;
        }
        //update access time
        v = record->second.first;
        renew(k);
        return true;
    }

    void print()
    {
        for (auto& r : table_)
        {
            cout << "(" << r.first << "," << r.second.first << ") ";
        }
        cout << endl << endl;
    }

private:
    void renew(const K& k)
    {
        queue_.erase(table_[k].second);
        queue_.emplace_front(k);
        table_[k].second = queue_.begin();
    }
    using Q = list<K>;
    using Table = unordered_map < K, pair<V, typename Q::iterator>>;
    Table table_;
    Q queue_;
};


struct Pageable
{
    virtual int getid() const = 0;
    virtual void* getdata() = 0;
};

template<class T>
struct Page : public Pageable
{
    using value_type = T;
    Page(int id, T t):ID(id),data(t) {}
    virtual int getid() const override
    {
        return ID;
    }

    virtual void* getdata() override
    {
        return &data;
    }
    int ID;
    T data;
};


int main()
{
    Cache<3, decltype(declval<Pageable>().getid()), Pageable*> cache;

    tuple<int, float, string> t;
    Page<remove_reference_t<decltype(get<0>(t))>> int_p(0,-1);
    Page<remove_reference_t<decltype(get<1>(t))>> float_p(1,5.5f);
    Page<remove_reference_t<decltype(get<2>(t))>> string_p(2,string("foo"));

    cache << make_pair(int_p.getid(), &int_p);
    //     cache.print();

    cache << make_pair(float_p.getid(), &float_p);
    //     cache.print();

    cache << make_pair(string_p.getid(), &string_p);
    //     cache.print();

    //2 becomes recent
    Pageable* x;
    cache(2, x);
    cout << *static_cast<float*>(x->getdata());

    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}