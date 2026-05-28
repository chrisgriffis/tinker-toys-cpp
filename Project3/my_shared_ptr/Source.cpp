#include <iostream>
#include <mutex>

using namespace std;

namespace my {

	template<class T>
	class shared_ptr
	{
		T* _pobj;
		struct cb
		{
			cb() :_refcount(1) {}
			std::mutex _m;
			int _refcount;
		} *_cb;
	public:
		void stats() const
		{
			std::lock_guard<std::mutex> l(_cb->_m);
			cout << "_cb->_refcount: " << _cb->_refcount << endl;
		}
		shared_ptr(T* pobj) : _pobj(pobj), _cb(new cb) {}
		shared_ptr(const shared_ptr& rhs)
		{
			std::lock_guard<std::mutex> l(rhs._cb->_m);
			if (rhs._cb->_refcount < 1)
				throw std::exception("shared ptr incr non pos ref count");
			else
			{
				_pobj = rhs._pobj;
				_cb = rhs._cb;
				++_cb->_refcount;
			}
		}
		~shared_ptr()
		{
			bool do_del = false;
			{
				std::lock_guard<std::mutex> l(_cb->_m);
				if (_cb->_refcount > 1)
					--_cb->_refcount;
				else if (_cb->_refcount == 1)
				{
					_cb->_refcount = 0;
					do_del = true;
				}
			}
			stats();
			//this is going to race because _cb->refcount could incr immediately when lock is released
			if (do_del)
			{
				delete _cb;
				delete _pobj;
			}
		}
		T& operator*()
		{
			std::lock_guard<std::mutex> l(_cb->_m);
			if (_cb->_refcount > 0)
			{
				return *_pobj;
			}
			else throw std::exception("shared ptr deref non pos ref count");
		}
		T *operator->() const
		{
			std::lock_guard<std::mutex> l(_cb->_m);
			if (_cb->_refcount > 0)
			{
				return _pobj;
			}
			else throw std::exception("shared ptr deref non pos ref count");
		}

	};

}


int main()
{
	struct foo
	{
		foo(int) {}
		void act() const { cout << "foo!" << endl; }
	};
	my::shared_ptr<foo> pfoo(new foo(5));
	pfoo.stats();
	my::shared_ptr<foo> pfoo2(pfoo);
	pfoo.stats();
	pfoo2.stats();

	pfoo2->act();

	int sdfgwert;
	cin >> sdfgwert;
	return 0;
}
