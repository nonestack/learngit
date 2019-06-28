#ifndef __SPLITTOOLNLPIR_H__
#define __SPLITTOOLNLPIR_H__

#include "Configuration.h"
#include "SplitTool.h"


namespace tsk{

class SplitToolNLPIR : public SplitTool{
	public:
		SplitToolNLPIR();
		virtual ~SplitTool();
		virtual vector<string> cut(const string &sentence) = 0;
	private:
		Configuration &_conf;
};

}//end of namespace tsk

#endif
