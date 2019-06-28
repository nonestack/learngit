#ifndef __DICTINDEX_H__
#define __DICTINDEX_H__

#include "DictProducer.h"

#include <string>

using std::string;

namespace tsk{

class DictIndex{
	public:
		DictIndex(DictProducer &dict);
		void bulidIndex(const string &readfile, const string &writefile);
	private:
		DictProducer &_dict;
};

}//end of namespace tsk

#endif
