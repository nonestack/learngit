#ifndef __DICTINDEX_H__
#define __DICTINDEX_H__

#include "DictProducer.h"

#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;


namespace tsk{

class DictIndex{
	public:
		DictIndex(DictProducer &dict, map<char, vector<int>> & dEnglish);
		void bulidIndex(const string &readfile, const string &writefile);
	private:
		DictProducer &_dict;
		map<char, vector<int>> &_dEnglish;
};

}//end of namespace tsk

#endif
