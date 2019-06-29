#include "../include/Configuration.h"
#include "../include/DictIndex.h"
#include "../include/DictProducer.h"

#include <map>
#include <string>
#include <iterator>
#include <iostream>

using std::string;
using std::map;
using std::cout;
using std::endl;

using namespace tsk;

int main(){
	Configuration conf;
	//cout << conf.getConfigMap().size() << endl;
	//map<string, string> m(conf.getConfigMap());
	//cout << m.size() << endl;
	//auto iter = m.begin();
	//++iter;
	//cout << iter->second << endl;
	//++iter;
	//select source file of english
	string confInfo("SrcEnglish");
	cout << confInfo << endl;
	string path = conf.getPath(confInfo);
	DictProducer dir(path);
	dir.build_dict();

	//write IndexLinkEnglish file
	confInfo = "IndexLinkEnglish";
	path = conf.getPath(confInfo);
	dir.store_dict(path.c_str());

	//build english index file
	DictIndex idx(dir);
	confInfo = "IndexEnglish";
	string indexfile = conf.getPath(confInfo);
	idx.bulidIndex(path, indexfile);
	dir.show_files();
	return 0;
}
