#include <ge/entity/component/ComponentBatch.h>

void ge::ComponentBatch::cycle()
{
	performPendingActions();

	for(Component* component : components)
	{
		component->cycle();
	}
}

void ge::ComponentBatch::softInsert(Component* cmp)
{
	actionBuffer.push({INSERT, cmp});
}

void ge::ComponentBatch::softRemove(Component* cmp)
{
	actionBuffer.push({ REMOVE, cmp });
}

void ge::ComponentBatch::hardInsert(Component* cmp)
{
	for (unsigned int i = 0; i < components.size(); i++) //NOTE: might be a better way
	{
		if (components.at(i) == nullptr)
		{
			components.at(i) = cmp;
			return;
		}
	}

	components.push_back(cmp); //if full
}

void ge::ComponentBatch::hardRemove(Component* cmp)
{
	for(unsigned int i = 0; i < components.size(); i++) //NOTE: might be a better way
	{
		if(components.at(i)==cmp)
		{
			components.at(i) = nullptr;
			return;
		}
	}
}

void ge::ComponentBatch::performPendingActions()
{
	while (actionBuffer.size()!=0)
	{
		switch(actionBuffer.front().first)
		{
		case INSERT: 
			hardInsert(actionBuffer.front().second);
			break;
		case REMOVE: 
			hardRemove(actionBuffer.front().second); 
			break;
		}
		actionBuffer.pop();

	}
}
