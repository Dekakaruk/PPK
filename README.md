About
=====

PPK is a library that allows you to read data files in simple C-like format. Documentation is available on http://dekakaruk.github.io/PPK.



Building
========
While in main directory type:

~~~bash
cmake
make
make doc  # if you want a documentation
make install
~~~

The only dependency is Boost. If you want also a documentation, you need Doxygen too. It will be in the "html" directory.


Example file
============

~~~js
# empty nodes, their type is Null
null1;
null2 {}
null3 = {}
null4 = []

# some scalars

a-bool	=	true
some_float	=	83.2

hexadecimal_int	optional-id	=	0xff00ff
"iee 754 stuff"	some_name	=	-inf

some_string	=	"Bwahahahahahah\t\t\t\n\n \\ \"!@#$%^&\""


# some lists
a_vector = 200, 400
a_vector = 400, 231

big_list = a, true, 42, 0.2, [a, b, c], {var = x   var = y   another_var = z}
'also list' = [123, 233, 655]


# some groups
Tree larch {
	var = 3
	var = 7
	var with_identifier = 4
}

Tree oak {
	var = 3
	var = 7
	var with_identifier = 4
}

Tree some_other {
	var = 3
	var = 7
	var with_identifier = 4
}

Settings {
	setting1 = "aaaaaaa"
	setting2 = 7
	setting3 = nan, inf, nan
}
~~~


Code example
============

~~~cpp
ppk::FS fs;
if (!fs.read("some_file.cl"))
{
	std::cout << fs.getError();
	return false;
}

ppk::Node & root = fs.getRoot();
bool a = root["a-bool"];
int b = root["big_list"][2];
float c = root("nonexisting float", 7.0);  // returns default value

for (auto & node : root.only("Tree"))
	std::cout << node.getIdentifier() << "\n";

root["big_list"][0] = "some string";
root.emplace("name of new node") = 4;

fs.write("some_file.cl");
~~~


Features
========
- Custom types, hexadecimal integers, special floating-point values: inf, -inf, nan
- Nested variable groups, with multiple occurences allowed
- Lists, in nice, comma separated format
- Each variable can have an identifier
- Line comments, starting with '#'
- Parsing all files in directory and it's subdirectories
- Getters take and use default values

TODO
====
- Validity checker
- Includes
- Nodes should remember their file and position
- Simple configuration
