#ifndef __LIVENESS_ANALYSIS__
#define __LIVENESS_ANALYSIS__

#include "Types.h"
#include "IR.h"
/*
void livenessAnalysis(Instructions instructions);
	for (auto it = instructions.rbegin(); it != instructions.rend(); it++)
	{
		(*it)->m_out.clear();
		(*it)->m_in.clear();

		for (auto itt = (*it)->m_succ.begin(); itt != (*it)->succ.end(); itt++) {
			for (auto it2 = (*itt)->in.begin(); it2 != (*itt)->in.end(); it2++) {
				(*it)->out.push_back(*it2);
			}
		}

		(*it)->m_out.sort();
		(*it)->out.unique();

		(*it)->in = (*it)->use;
		for (auto itt = (*it)->out.begin(); itt != (*it)->out.end(); itt++) {
			auto it2 = find((*it)->def.begin(), (*it)->def.end(), *itt);
			if (it2 != (*it)->def.end()) {

			}
			else {
				((*it)->in.push_back(*itt));
			}
		}

		(*it)->in.sort();
		(*it)->in.unique();
	}
}*/
#endif
