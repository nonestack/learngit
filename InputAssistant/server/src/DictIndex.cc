#include "../include/DictIndex.h"

#include <fstream>
#include <utility>

using std::ofstream;
using std::ifstream;

namespace tsk{

DictIndex::DictIndex(DictProducer &dict, 
		map<char, vector<int>> &dEnglish)
:_dict(dict)
,_dEnglish(dEnglish)
{}

void DictIndex::bulidIndex(const string &readfile, const string &writefile){
	ofstream ofs(writefile);

	char alp = 'a';
	string s1;
	int i;
	for(; alp <= 'z'; ++alp){
		vector<int> ivec;
		ofs << alp << '\t';
		size_t line = 1;
		ifstream ifs(readfile);
		while(ifs >> s1){
			for(i = 0; i < s1.size(); ++i){
				if(s1[i] == alp){
					ofs << line << '\t';
					ivec.push_back(line);
					break;
				}
			}
			++line;
		}
		auto tmp = std::make_pair(alp, ivec);

		_dEnglish.insert(tmp);
		ifs.close();
		ofs << '\n';
	}

	ofs.close();
}




}//end of namespace tsk
