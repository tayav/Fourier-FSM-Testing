#ifndef UIO_METHOD_H
#define UIO_METHOD_H
#include<set>
#include<list>
#include"state.h"
#include"transition.h"


using namespace std;


class uio_method final
{
public:
	
	 static set<list<string>> run(const pair<string, list<transition>> transitions);
};

#endif

