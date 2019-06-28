#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include "Configuration.h"
#include "SplitTool.h"

namespace tsk{

class SplitToolCppJieba : public SplicTool{
	public:
		SplitToolCppJieba();
		virtual ~SplitToolCppJieba();
		virtual vector<string> cut(const string &sentence) = 0;
	private:
		Configuration &_conf;
};

}//end of namespace tsk

#endif
