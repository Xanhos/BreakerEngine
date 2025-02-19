#include "State.h"
#include "List.h"
#include "UI.h"
#include "SpriteManager.h"


stdList* stateList = NULL;

DECLARE_BLANK_STATE(NULLSTATE)

void __RegisterState(StateInfo stateInfo)
{
	if (stateList == NULL)
	{
		stateList = STD_LIST_CREATE(StateInfo, 0);
	}
	stateList->push_back(stateList, &stateInfo);
}

StateInfo GetState(const char* name)
{
	FOR_EACH_LIST(stateList, StateInfo, i, it,
		if (strcmp(name, it->name) == 0)
			return *it;
			);

	return (StateInfo) {
		.name = "null",
			.Init = &InitNULLSTATE,
			.UpdateEvent = &UpdateEventNULLSTATE,
			.Update = &UpdateNULLSTATE,
			.Render = &RenderNULLSTATE,
			.UIRender = &UIRenderNULLSTATE,
			.Destroy = &DestroyNULLSTATE
	};
}

