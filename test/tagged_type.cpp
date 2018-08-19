#include <tagged.h>
#include <cmath>
#include <iostream>

using tagged::get;

DEFINE_TAG(from);
DEFINE_TAG(to);
DEFINE_TAG(a);
DEFINE_TAG(b);
DEFINE_TAG(default_value);

template <typename DST, typename SRC>
// void copy(DST& to: dst, SRC from: src)
void copy(to<DST&> dst, from<SRC> src)
{
	*dst = *src;
}

class Sumthing
{
    int a = 0;
    float _b = 0;

public:
    template <class... Args>
    Sumthing(Args&&... args) : 
        a{get<::a>(4, std::forward<Args>(args)...)}, _b{get<b>(0, std::forward<Args>(args)...)}
    { 
    }

    float sum() const { return a + _b; }
};

DEFINE_TAG(radius);
DEFINE_TAG(angle);

struct Vector
{
	float x;
	float y;

	constexpr Vector(float x, float y) : x(x), y(y) { }
	Vector(radius<float> r, angle<float> m) : x(*r*std::cos(*m)), y(*r*std::sin(*m)) { }
};


DEFINE_TAG(guests);
DEFINE_TAG(wear_hats);

void party(guests<int> guests, wear_hats<bool> wear_hats)
{
	for ( ; *guests > 0; --*guests)
		if (*wear_hats)
			std::cout << "Woohoo!\n";
		else
			std::cout << "Yay!\n";
}


int main()
{
	Vector va{ 1, 2 };
	Vector vb{ radius{2.f}, angle{1.68f} };

	party(guests{ int(vb.x + vb.y) }, wear_hats{ va.x < va.y });


    Sumthing s{ b(3.5f), a(2) }; // { b: 3.5f, a: 2 };

	int x = 2;
	int y;
    auto z = from(s.sum()); // auto z = from: s.sum();
	auto foo = a{ 2 };
	copy(to{y}, from{ x }); // copy(to: y, from: x);
    copy(to{y}, from(int(*z))); // copy(to: y, from: int(z));

    return y;
}
