#include "../include/DictProducer.h"

#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <iterator>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

namespace tsk{

DictProducer::DictProducer(const string &dir)
:_dirEn(dir)
{}

void DictProducer::build_dict(){
	cout << _dirEn.c_str() << endl;
	DIR *dir = opendir(_dirEn.c_str());
	struct dirent *loopdir;
	string s1, s2;
	int i;

   	while((loopdir = readdir(dir)) != NULL){
		//cout << "ok?" << endl;
		//cout << "DictProducer::build_dict()" << endl;
		if(strcmp(loopdir->d_name, ".") == 0 ||
		   strcmp(loopdir->d_name, "..") == 0){
			continue;
		}
			
		string filename(_dirEn);
		filename += loopdir->d_name;
		_files.push_back(filename);

		//input temp file
		string tmp("tmp.txt");
		ifstream ifs1(filename);
		ofstream ofs1(tmp);
		while(ifs1 >> s1){
			ofs1 << s1 << endl;
		}
		ifs1.close();
		ofs1.close();

		ifstream ifs2(tmp);
		ofstream ofs2(filename);
		while(ifs2 >> s1){
			for(i = 0; i < s1.size(); ++i){
				if(isalpha(s1[i]) == 0){
					s1[i] = ' ';
				}
				else{
					s1[i] = tolower(s1[i]);
				}
			}
			ofs2 << s1 << endl;
		}

		ifs2.close();
		ofs2.close();
		unlink(tmp.c_str());
		
		ifstream ifs3(filename);
		while(ifs3 >> s1){
			++_dict[s1];
		}
		ifs3.close();
	}

	closedir(dir);
}

void DictProducer::store_dict(const char *filepath){
	ofstream ofs;
	ofs.open(filepath);

	auto beg = _dict.begin();
	while(beg != _dict.end()){
		ofs << beg->first << "\t" << beg->second << endl;
		++beg;
	}

	ofs.close();
}

void DictProducer::show_files() const{
	ofstream ofs;
	ofs.open("/home/genius/Program/InputAssistant/show.txt");
	auto iter = _dict.begin();
	while(iter != _dict.end()){
		ofs << iter->first << " : " << iter->second << endl;
		++iter;
	}
	ofs.close();
}

void DictProducer::show_dict() const{
	cout << _dirEn << endl;
	cout << _dirZh << endl;
}


}//end of namespace tsk
