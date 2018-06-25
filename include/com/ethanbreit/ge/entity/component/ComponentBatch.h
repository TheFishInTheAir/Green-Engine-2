#pragma once
#include "Component.h"
#include <vector>
#include <queue>


namespace ge
{
	struct ComponentBatch //TODO: make abstract super class and make this default batch
	{
		std::vector<Component*> components;


		void cycle();


		void softInsert(Component*);
		void softRemove(Component*);

		void hardInsert(Component*);
		void hardRemove(Component*);

	private:
		typedef enum _action
		{INSERT, REMOVE};
		std::queue<std::pair<_action, Component*>> actionBuffer;

		void performPendingActions();
	};
}
