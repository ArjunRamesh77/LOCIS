#include <iostream>
/*
PLEASE READ THE FOLLOWING TUTORIAL CAREFULLY. IT WILL HELP YOU TO MAKE CUSTOM EXPRESSIONS.

___________________________________________________________________________________________________________________________________________
LESSON 1: The 'Search and replace' that you use so often!!

Regex is nothing other than a glorified search fuction. Many a time we have used the search utility to convert one word to another ie. 
in your class report, PhD Thesis ;), or otherwise. For example lets take the sentence "My Transport Phenomena Professor is an idiot. infact
he defines the word idiot". Lets say for the sake of not getting fired we want to replace idiot with angel, this is how regex would do it:

string = "My Transport Phenomena Professor is an idiot. infact he defines the word idiot"
regex = "idiot"
replace = "angel"
new_string = "My Transport Phenomena Professor is an angel. infact he defines the word angel"

Pretty Trivial right!! Well I suppose this example was unnecessary. Just for fun!

___________________________________________________________________________________________________________________________________________
LESSON 2: I like your character!!

Lets say you want to find all the a's and b's in a word. For whatever reason. In regex a single character search is represented by [], whats
inside the [] determines what you want to find. For eg. [a] will find all the a's in the sentence, then [b] will find all the b's
in the sentences. then how do we find all a's and b's, simple [ab], thats it. This basically means a single character which is either a or b.

string = "My uncle arnab was abslutely right"
regex = "[ab]"
search results = a, b, a, b

So lets say you want to search for all the alphabets in the english language. So we use the range operator "-". So the regex becomes
[a-z]. This will search for all the alphabets in the sentence. So basically the search results for the above string becomes
search results = y, u, n, c, l, e, r, r, n, a, b, a, b, s, o, l, u, t, e, l, y r, i, g, h, t

You might be wondering how did I miss the "M" in My and the "A" in Arnab, well thats because regex is case sensitive. To search for that
we need to make regex = "[A-Za-z]". This will get all the alphabets. similary to find numerals we can use [0-9] and combining letters and
numerals we can have [A-Za-z0-9], this will match any number or alphabet in a sentence.

___________________________________________________________________________________________________________________________________________
LESSON 3: I love the combination of dal and rice!!

Regex is most powerful when we combine the above two lessons. Lets say you want to find all versions of a software in a sentence. 

string = "Version_1 was way better that Version_2 and Version_0"
regex = "Versionn_[0-9]"
search results = Version_1, Version_2, Version_0

Wow!! now we are getting somewhere. This found two unique string's just differing in the number. I think this is pretty clear.
All is well and good until....

string = "Version_10 was way better than Version_9"
regex = "Versionn_[0-9]"
search results = Version_9

Huh?? why didnt it get "Version_10". Well as I told you earlier [] represents only one character. so [0-9] would represent just one 
number satisfying Version_1, Version_2 and Version_9 but not Version_10. How do we deal with this.

Well if you put in plain English "I want to find versions with 1 or more numbers" like 0, 10, 100 etc. Then the new regex becomes
regex = "Version_[0-9]+"

Here the + represents "the previous character can occur one or more times. But even with this Version_0 and Version_00 etc is allowed. I 
dont know any software company that started with version 0, or 00!!! LOL. Lets say we want to eliminate this problem. Then our new regex 
becomes 

regex = "Version_[1-9][0-9]+" 

This means the first character must be betweeen 1-9 and then the second character can be any thing between 0-9 one or more times. So
for new search:
string = "Version_14 was way better than Version_1 and Version_01 and Version_0"
regex = "Version_[1-9][0-9]+" 
search results = Version_14, Version_1

Clever huh!!

___________________________________________________________________________________________________________________________________________
LESSON 4: I think I can't find my character (sad music playing in the background)

So with all this information lets define a variable in C++. The rules are:-
	1) Must Start with an alphabet
	2) Can have alphabets, underscore or number after that

So regex is easy
regex = "[A-Za-z][A-Za-z0-9_]+

string = "Temp Temp222 Pres_23 are all variables"
regex = "[A-Za-z][A-Za-z0-9_]+
search results = Temp, Temp222, Pres_23, are, all, variables

Great Worked perfectly!!!

Lets take another string

string = "a is a variable"
regex = "[A-Za-z][A-Za-z0-9_]+
search results = is, variable

Huh!! where is "a", why can't I find "a" !!! :(
Well dont be sad, "+" means the previous character should be one or more times so obviously simply "a" would not be found. Lets modify this

string = "a is a variable"
regex = "[A-Za-z][A-Za-z0-9_]*
search results = a, is, a, variable

I converted the "+" to a "*". A star means the previous character can occur 0 or more times!!!
Recap: + means previous character can occur 1 or more times
	   * means previous character can occur 0 or more times

___________________________________________________________________________________________________________________________________________
LESSON 5: What's my purpose Oh Lord!

Lets see some real life uses of regex. 

1) Phone number
regex = "\([0-9]{3}\)-[0-9]{3}-[0-9]{4}"

	This will match (412)-608-4535 eg.
	As you may have figured out {n} matches the previous character exactly n times. "(" is represented as "\(" because "(" itself has
	a special meaning in regex. This will be explained later.

2) Email ID
regex = "[A-Za-Z0-9]+@[A-Za-Z0-9]+\.com"

	Again "." has a special meaning in regex so must be escaped. Will be explained later.

3) Hexadecimal number
regex = 0x[0-9A-F]+

4) ip address
regex = [1-9][0-9]{2}\.[1-9][0-9]{2}\.[1-9[0-9]{2}

Pretty nifty huh!!

___________________________________________________________________________________________________________________________________________
LESSON 6: We should make a whatsapp group!!

All this while you might have noticed we indetified the whole regex in a sentence. There was no way to partition the "search" that we 
searched. For example lets take the email example.

string = "arjun@gmail.com"
regex = "[A-Za-Z0-9]+@[A-Za-Z0-9]+\.com"
search results = arjun@gmail.com

but what if I wanted to get the first parts of the email. i.e arjun ??

Simple lets group it!!
regex = "([A-Za-Z0-9]+)@([A-Za-Z0-9]+)\.com"
search results = arjun@gmail.com

then conceptually search results[0] = arjun@gmail.com
				  search results[1] = arjun
				  search results[2] = gmail

I hope you got what "(" means here!
Cool right!! 

___________________________________________________________________________________________________________________________________________
LESSON 6: Code!!
*/

#include "stdafx.h"
#include <string>
#include <regex>


// Function phi
std::string PHI(int n, int i)
{
	switch (n)
	{
	case 1:
		if (i == 1)
		{
			return "1";
		}
		if (i == 2)
		{
			return "0";
		}

	case 2:
		if (i == 1)
		{
			return "0";
		}
		if (i == 2)
		{
			return "1";
		}
	default:
		return "<NA>";
	}
}

// Function dphi
std::string DPHI(int n, int i)
{
	switch (n)
	{
	case 1:
		return "(-1/h)";

	case 2:
		return "(1/h)";
	default:
		return "<NA>";
	}
}


//int main()
//{
//	/*
//	Consider the string "#DPHI_1.#DPHI_1 + #PHI_1.#PHI_1 +  #DPHI_2.#DPHI_1 + #PHI_2.#PHI_1"
//
//	Objective:
//		Replace for #DPHI_n and #PHI_n depending on where you want to evaluate.
//	*/
//
//	std::string str = "#DPHI_1.#DPHI_1 + #PHI_1.#PHI_1 +  #DPHI_2.#DPHI_1 + #PHI_2.#PHI_1";
//	std::regex reg("#[D]?PHI_([0-9]+)");
//	std::smatch matches;
//
//	/*
//	You might wonder whats "?" in the expression. You guessed it! It means the previous character may or may not be there!!
//	So basically it will match both PHI and DPHI
//	*/
//
//	// Lets evaluate the str at i = 1
//	std::string copy_str = str;          // Make a copy of the original string, this will be modified at each interation
//	std::string mod_str = str;			 // This is the final string, will start of with original string 
//	std::string temp("");				 // To hold the return value of PHI and DPHI 
//	int nval = 0;						 // Holds the n value for PHI and DPHI 
//
//	/*
//	regex_search will search a string for only the first occurance of reg, thats why we need a loop.
//	*/
//	while (std::regex_search(copy_str, matches, reg)) 
//	{
//		
//		/*
//		as Expected 
//		matches[0] will contain the entire match for eg. #DPHI_1
//		matches[1] will contain the first group in reg, in this case this is ([0-9]+)
//		*/
//
//		// Get n from matches[1]
//		nval = stoi(matches[1]);
//		std::cout << nval << "\n";
//		std::cout << matches[0] << "\n";
//		// Check whether DPHI or PHI, simply look at the second character of matches[0] as this contains the whole match
//		// if your wondering first character is obviously "#" :P
//		// And call appropriate function 
//		if (*(matches[0].first + 1) == 'D')
//		{	
//			temp = DPHI(nval, 1);	// DPHI evaluated at i == 1
//		}
//		else
//		{
//			temp = PHI(nval, 1);    // PHI evaluated at i == 1
//		}
//
//		/*
//		regex_replace takes the modified string and makes a replacement on itself with temp, Unfortunately regex_replace replaces
//		all occurances of reg, so to coordinate with regex_search we use the std::regex_constants::format_first_only flag
//		that restricts to first replacement, which coincides with the first search from regex_search
//		*/
//		mod_str = std::regex_replace(mod_str, reg, temp, std::regex_constants::format_first_only);
//
//		/*
//		this is how regex gets each search of reg, by replacing the original_string = original_string - match[0]. Without
//		this regex_search will be stuck in the first occurance!!	
//		*/
//		copy_str = matches.suffix().str();
//		std::cout << copy_str << "\n";
//	}
//
//    return 0;
//}

int main()
{
	std::string str = "#Du.#Dv - #v";
	std::regex reg;
	std::smatch matches;

	reg = "#[D]?[uv]";
	std::string copy_str = str;          // Make a copy of the original string, this will be modified at each interation
	std::string mod_str = str;			 // This is the final string, will start of with original string 
	std::string temp(""), tempd("");				 // To hold the return value of PHI and DPHI 

	std::cout << std::regex_search(copy_str, matches, reg) << "\n";
	std::cout << matches[0] << "\n";
	tempd = "(x[globalno] * phid[globalno - localno] )";
	if (*(matches[0].first + 1) == 'D')
	{
		// replace with phid
		mod_str = std::regex_replace(mod_str, reg, tempd, std::regex_constants::format_first_only);
	}
	std::cout << mod_str << "\n";
	//while (std::regex_search(copy_str, matches, reg))
	//{

	//}
}