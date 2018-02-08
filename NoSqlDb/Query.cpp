////////////////////////////////////////////////////////////////
// Query.cpp - Implementing Query.h                           //
// ver 1.0                                                    //
// Yadav Murthy, CSE687 - Object Oriented Design, Spring 2017 //
////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the functions of Query<T> class
* Its purpose is to support various queries on the values present in the database
*
* Required Files:
* ---------------
* NoSqlDb.h
*
* Build Process:
* --------------
* devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 13 Feb 2017
* - first release
*
*/

#ifdef TEST_Query



#include "Query.h"
#include <set>
using StrData = std::string;
using Key = NoSqlDb<StrData>::Key;

int main()
{
	NoSqlDb<StrData> db;

	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.description = "elem1's description";
	elem1.data = "elem1's StrData";
	elem1.timeDate = elem1.getTimeDate();

	db.save(elem1.name, elem1);

	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.description = "elem2's description";
	elem2.data = "elem2's StrData";
	elem2.timeDate = elem2.getTimeDate();

	db.save(elem2.name, elem2);

	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.description = "elem3's description";
	elem3.data = "elem3's StrData";
	elem3.timeDate = elem3.getTimeDate();

	db.save(elem3.name, elem3);

	elem2.addChild(elem3.name);
	elem1.addChild(elem2.name);
	elem1.addChild(elem3.name);
	elem3.addChild(elem2.name);

	db.resave(elem2.name, elem2);
	db.resave(elem1.name, elem1);

	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";

	Query<StrData> query;

	title("Query1");
	Element<StrData> result1 = query.Query1(db, "elem");
	std::cout << result1.show();

	title("Query2");
	Keys result2 = query.Query2(db, "elem1");
	query.showResult(result2);
	
	title("Query3");
	Keys result3 = query.Query3(db, "elem");
	query.showResult(result3);
	
	title("Query4");
	Keys result4 = query.Query4(db, "elem");
	query.showResult(result4);
	
	title("Query5");
	Keys result5 = query.Query5(db, "te");
	query.showResult(result5);
	
	title("Query6");
	Keys result6 = query.Query6(db, "Data");
	query.showResult(result6);

	
	title("Query8 on result of Query4");
	Keys result8 = query.queryOnAQuery(db, "em2", result4);
	query.showResult(result8);

	
	title("Query4 on union of keys obtained from query2 and query3");
	std::set<Key> set1(result2.begin(), result2.end());
	std::set<Key> set2(result3.begin(), result3.end());
	Keys unionOfQueryResults;
	std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(unionOfQueryResults));
	Keys result9 = query.queryUnion(db, "lem", unionOfQueryResults);
	query.showResult(result9);
}

#endif // TEST_Query