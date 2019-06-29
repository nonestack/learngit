#include "../include/DictIndex.h"

#include <fstream>

using std::ofstream;
using std::ifstream;

namespace tsk{

	DictIndex::DictIndex(DictProducer &dict)
	:_dict(dict)
	{}

	void DictIndex::bulidIndex(const string &readfile, const string &writefile){
		ofstream ofs(writefile);

		char alp = 'a';
		string s1;
		int i;
		for(; alp <= 'z'; ++alp){
			ofs << alp << '\t';
			size_t line = 1;
			ifstream ifs(readfile);
			while(ifs >> s1){
				for(i = 0; i < s1.size(); ++i){
					if(s1[i] == alp){
						ofs << line << '\t';
						break;
					}
				}
				++line;
			}

			ifs.close();
			ofs << '\n';
		}

		ofs.close();
	}

}//end of namespace tsk
