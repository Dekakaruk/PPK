#include "FS.hpp"
#include <cstdio>
#include <iostream>

#include "StandardConverters.hpp"

using namespace std;


int main()
{
	ppk::FS p;
	if (!p.read("../../test"))
		printf("%s\n", p.getError().c_str());
	
	//p.getRoot().emplace("w") = 2.3244;
	//p.getRoot().emplace("w") = 3.3244;
	
	//float f1, f2;
	//f1 = p.getRoot()("w", 1.);
	//f2 = p.getRoot()("w2", 1);
	
	//printf("%f %f\n", f1, f2);
	
	
	//ppk::Converter<a>::type_name = "aaaa";
	p.getRoot()["big_list"][2].as<unsigned long long>();
	
	p.print();
	
	for (auto & i : p.getRoot().all())
	{
		cout << (int)i.getType() << "\n";
	}
	
	p.write("paplik.out");
}

