#include "../include/MyDict.h"
#include "../include/DictProducer.h"
#include "../include/DictIndex.h"

#include <iostream>

using std::cout;
using std::endl;

namespace tsk{


MyDict::MyDict()
:_conf()
{
	string confInfo("SrcEnglish");
	//cout << confInfo << endl;
	string path = _conf.getPath(confInfo);
	//cout << path << endl;
	DictProducer dir(path);
	dir.build_dict();
	//cout << "ok" << endl;

	//write IndexLinkEnglish file
	confInfo = "IndexLinkEnglish";
	path = _conf.getPath(confInfo);
	dir.store_dict(path.c_str());

	//init _linkEnglish
	dir.getWord(_linkEnglish);

	//build english index file and init _dictEnglish
	DictIndex idx(dir, _dictEnglish);
	confInfo = "IndexEnglish";
	string indexfile = _conf.getPath(confInfo);
	idx.bulidIndex(path, indexfile);
	dir.show_files();
}

MyDict *MyDict::_pInstance = nullptr;

MyDict *MyDict::getInstance(){
	if(_pInstance == nullptr){
		_pInstance = new MyDict();
	}
	return _pInstance;
}

void MyDict::destory(){
	if(_pInstance != nullptr){
		delete _pInstance;
		_pInstance = nullptr;
	}
}

void MyDict::print(){
	auto iter = _dictEnglish.begin();
	//int i = 0;
	for(; iter != _dictEnglish.end(); ++iter){
		cout << iter->first << " : " << iter->second.size()<< endl;
	}
}


}//end of namespace tsk
