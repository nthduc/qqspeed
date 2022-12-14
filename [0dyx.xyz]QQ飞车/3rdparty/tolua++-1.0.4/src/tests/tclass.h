#include <stdio.h>

namespace Test {

class Tst_Dummy
{
};

class Tst_A
{
public:
	static Tst_A* last;
	Tst_A () {last = this;}
	virtual char* a () { return "A"; }
	class Tst_AA
	{
 public:
		Tst_AA () {}
		~Tst_AA () { }
		char* aa () { return "AA"; }
	};
	class Tst_BB : public Tst_AA
	{
 public:
		Tst_BB () {}
		~Tst_BB () {}
		Tst_AA* Base () { return this; }
	};
};

class Tst_B : public Tst_A
{
public:
	static Tst_B* last;
	Tst_B () {last = this;}
	virtual char* b () { return "B"; }
};

class Tst_C : public Tst_B
{
	int i;
public:
	static Tst_C* last;
	Tst_C (int n) : i(n) {last = this;}
 virtual ~Tst_C () { printf("deleting C: %d\n",i); }
	virtual char* c () { return "C"; }
};

inline Tst_A::Tst_AA* Tst_create_aa ()
{
	return new Tst_A::Tst_AA();
}

inline bool Tst_is_aa (Tst_A::Tst_AA* obj)
{
	return true;
}

class Tst_E {
public:
	enum Pete {
		ONE,
		TWO,
	} pete;
	
	void get_pete(Pete p) {};

	template <class T>
	T get_t() {T a=0; return a;};

	Tst_E& operator+(const Tst_E& rvalue) {return *this;};
	
	void pars(int a=0, int b=0) {};
	void get_pointer(void* a) {};

	Tst_A a;
	
	Tst_E(int) {};
};

}; // end of namespace

