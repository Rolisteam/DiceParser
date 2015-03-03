#include "jumpbackwardnode.h"

JumpBackwardNode::JumpBackwardNode()
{
}



qint64 JumpBackwardNode::getPriority() const
{
	return 1;
}
QString JumpBackwardNode::toString() const
{
	return QString();
}
void JumpBackwardNode::run(ExecutionNode* previous)
{
		ExecutionNode* parent = previous;
		bool found=false;
		int i = 1;
		Result* result=NULL;
		while((NULL!=parent)&&(found))
		{
			result = parent->getResult();
			if(NULL!=result)
			{
				--i;
				if(i==0)
				{
					found =true;
				}
			}
			parent = parent->getPreviousNode();

		}
		m_result = result;
}
