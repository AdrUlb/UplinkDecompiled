#include <Eclipse.hpp>

#include <BTree.hpp>
#include <DArray.hpp>
#include <LList.hpp>
#include <Util.hpp>
#include <sys/time.h>

static char* currenthighlight = nullptr;
static char* currentclick = nullptr;

static ClearDrawFunc clear_draw = nullptr;
static ButtonDrawFunc default_draw = nullptr;
static ButtonMouseUpFunc default_mouseup = nullptr;
static ButtonMouseDownFunc default_mousedown = nullptr;
static ButtonMouseMoveFunc default_mousemove = nullptr;
static SuperhighlightDrawFunc superhighlight_draw = nullptr;

static LList<Button*> buttons;
static LList<const char*> editablebuttons;
static DArray<Animation*> anims;
static BTree<int> superhighlightedbuttons;

static int superhighlight_borderwidth = 0;

static void* dirtyrectangles = nullptr;
static void* data_7b3788 = nullptr;

static bool animsenabled = true;
static bool animsfasterenabled = false;
static double animsfasterspeed = 2.0;

static void EclDirtyClear()
{
	data_7b3788 = dirtyrectangles;

	for (auto i = buttons.Size() - 1; i >= 0; i--)
		if (buttons.ValidIndex(i) != 0)
			buttons[i]->Dirty = false;
}

static int EclLookupIndex(const char* name)
{
	if (name == nullptr)
		return -1;

	for (int i = 0; i < buttons.Size(); i++)
	{
		if (buttons.ValidIndex(i) && strcmp(buttons[i]->Name, name) == 0)
			return i;
	}

	return -1;
}

void EclEnableAnimations()
{
	animsenabled = true;
}

void EclDisableAnimations()
{
	animsenabled = false;
}

void EclEnableFasterAnimations(const double speed)
{
	animsfasterenabled = true;
	animsfasterspeed = speed;
}

void EclDisableFasterAnimations()
{
	animsfasterenabled = false;
}

double EclGetAccurateTime()
{
	static bool initted = false;
	static timeval startTime;

	timeval thisTime;
	gettimeofday(&thisTime, nullptr);

	if (!initted)
	{
		initted = true;
		startTime = thisTime;
		startTime.tv_usec = 0;
		return 0.0;
	}

	return (((thisTime.tv_sec - startTime.tv_sec) * 1000.0) + ((thisTime.tv_usec - startTime.tv_usec) / 1000.0));
}

void EclRegisterClearDrawFunction(ClearDrawFunc func)
{
	clear_draw = func;
}

void EclRegisterDefaultButtonCallbacks(ButtonDrawFunc draw, ButtonMouseUpFunc mouseUp, ButtonMouseDownFunc mouseDown, ButtonMouseMoveFunc mouseMove)
{
	default_draw = draw;
	default_mouseup = mouseUp;
	default_mousedown = mouseDown;
	default_mousemove = mouseMove;
}

void EclRegisterButtonCallbacks(const char* name, ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc, ButtonMouseDownFunc mouseDownFunc,
								ButtonMouseMoveFunc mouseMoveFunc)
{
	const auto index = EclLookupIndex(name);
	if (!buttons.ValidIndex(index))
		return;

	buttons[index]->RegisterDrawFunction(drawFunc);
	buttons[index]->RegisterMouseUpFunction(mouseUpFunc);
	buttons[index]->RegisterMouseDownFunction(mouseDownFunc);
	buttons[index]->RegisterMouseMoveFunction(mouseMoveFunc);
}

void EclRegisterSuperHighlightFunction(int borderWidth, SuperhighlightDrawFunc func)
{
	superhighlight_borderwidth = borderWidth;
	superhighlight_draw = func;
}

void EclReset()
{
	if (currenthighlight != nullptr)
	{
		delete[] currenthighlight;
		currenthighlight = nullptr;
	}

	if (currentclick != nullptr)
	{
		delete[] currentclick;
		currentclick = nullptr;
	}

	while (buttons.ValidIndex(0))
		EclRemoveButton(buttons.GetData(0)->Name);

	while (true)
	{
		const auto data = editablebuttons.GetData(0);
		if (data == nullptr)
			break;

		editablebuttons.RemoveData(0);

		delete[] data;
	}
	buttons.Empty();
	editablebuttons.Empty();
	superhighlight_borderwidth = 0;
	EclDirtyClear();
}

bool EclIsClicked(const char* name)
{
	if (currentclick == 0)
		return false;

	return strcmp(currentclick, name) == 0;
}

bool EclIsHighlighted(const char* name)
{
	if (currenthighlight == nullptr)
		return false;

	return strcmp(currenthighlight, name) == 0;
}

bool EclIsSuperHighlighted(const char* name)
{
	return superhighlightedbuttons.LookupTree(name) != 0;
}

bool EclIsButtonEditable(const char* name)
{
	if (EclGetButton(name) == 0)
		return false;

	for (auto i = 0; i < editablebuttons.Size(); i++)
	{
		if (!editablebuttons.ValidIndex(i))
			continue;

		if (strcmp(editablebuttons.GetData(i), name) == 0)
			return true;
	}
	return false;
}

void EclUnHighlightButton()
{
	EclGetButton(currenthighlight);

	if (currenthighlight != nullptr)
		delete[] currenthighlight;

	currenthighlight = nullptr;
}

void EclSuperUnHighlight(const char* name)
{
	if (superhighlightedbuttons.LookupTree(name) != 0)
	{
		superhighlightedbuttons.RemoveData(name);

		char buffer[0x80];
		sprintf(buffer, "Ecl_superhighlight %s", name);
		EclRemoveButton(buffer);
	}
}

char const* EclGetHighlightedButton()
{
	return currenthighlight;
}

void EclHighlightButton(const char* name)
{
	if (EclIsHighlighted(name) != 0)
		return;

	EclUnHighlightButton();

	if (currenthighlight != nullptr)
		delete[] (currenthighlight);

	currenthighlight = new char[strlen(name) + 1];
	strcpy(currenthighlight, name);
}

void EclMakeButtonEditable(const char* name)
{
	if (EclGetButton(name) == nullptr)
		return;

	char* str = new char[strlen(name) + 1];
	strcpy(str, name);
	editablebuttons.PutDataAtEnd(str);
}

void EclMakeButtonUnEditable(const char* name)
{
	for (auto i = 0; i < editablebuttons.Size(); i++)
	{
		if (!editablebuttons.ValidIndex(i))
			continue;

		if (strcmp(editablebuttons.GetData(i), name) != 0)
			continue;

		auto buttonName = editablebuttons.GetData(i);
		if (buttonName != nullptr)
			delete[] buttonName;
		editablebuttons.RemoveData(i);
		return;
	}
}

void EclDrawButton(int index)
{
	if (buttons.ValidIndex(index) == 0)
		return;

	const auto button = buttons[index];

	if (EclIsClicked(button->Name))
	{
		button->Draw(false, true);
		return;
	}

	if (EclIsHighlighted(button->Name))
	{
		button->Draw(true, false);
		return;
	}

	button->Draw(false, false);
}

void EclDrawAllButtons()
{
	for (auto i = buttons.Size() - 1; i >= 0; i--)
	{
		if (!buttons.ValidIndex(i))
			continue;

		const auto button = buttons[i];
		if (button->X < 0 && button->Y < 0)
			continue;

		EclDrawButton(i);
		button->Dirty = false;
	}
	data_7b3788 = dirtyrectangles;
}

void EclRegisterButton(int x, int y, int width, int height, const char* caption, const char* name)
{
	if (EclGetButton(name) != nullptr)
		return;

	const auto button = new Button(x, y, width, height, caption, name);
	buttons.PutDataAtStart(button);
	EclRegisterButtonCallbacks(name, default_draw, default_mouseup, default_mousedown, default_mousemove);
}

void EclRegisterButton(int x, int y, int width, int height, const char* caption, const char* tooltip, const char* name)
{
	(void)tooltip;
	EclRegisterButton(x, y, width, height, caption, name);
	EclGetButton(name)->SetTooltip(tooltip);
}

void EclRemoveButton(const char* name)
{
	const auto buttonIndex = EclLookupIndex(name);
	if (!buttons.ValidIndex(buttonIndex))
		return;

	const auto button = buttons[buttonIndex];

	if (EclIsHighlighted(name))
	{
		if (currenthighlight != nullptr)
			delete[] currenthighlight;

		currenthighlight = nullptr;
	}

	if (EclIsClicked(name))
	{
		if (currentclick != nullptr)
			delete[] currentclick;

		currentclick = 0;
	}

	if (EclIsSuperHighlighted(name))
		EclSuperUnHighlight(name);

	if (EclIsButtonEditable(name))
		EclMakeButtonUnEditable(name);

	for (auto index = anims.Size() - 1; index >= 0; index--)
	{
		if (!anims.ValidIndex(index))
			continue;

		if (strcmp(name, anims[index]->ButtonName) != 0)
			continue;

		EclRemoveAnimation(index);
	}

	buttons.RemoveData(buttonIndex);
	if (button != nullptr)
		delete button;
}

void EclRemoveAnimation(int index)
{
	if (!anims.ValidIndex(index))
		return;

	const auto animation = anims.GetData(index);
	if (animation != nullptr)
		delete animation;

	anims.RemoveData(index);
}

Button* EclGetButton(const char* name)
{
	const auto rax = EclLookupIndex(name);

	if (buttons.ValidIndex(rax))
		return buttons[rax];

	return nullptr;
}

void EclClearRectangle(int x, int y, int width, int height)
{
	if (clear_draw != nullptr)
		clear_draw(x, y, width, height);
}

int EclIsCaptionChangeActive(const char* buttonName)
{
	for (int i = 0; i < anims.Size(); i++)
	{
		if (!anims.ValidIndex(i))
			continue;

		const auto animation = anims[i];

		assert(animation != nullptr);

		// Button name does not match
		if (strcmp(buttonName, animation->ButtonName) != 0)
			continue;

		// Caption will change
		if (animation->TargetCaption != nullptr)
			return i;
	}

	return -1;
}

static void EclUpdateSuperHighlights(const char* name)
{
	const auto button = EclGetButton(name);

	if (button == nullptr)
	{
		EclSuperUnHighlight(name);
		return;
	}

	if (!EclIsSuperHighlighted(name))
		return;

	char highlightButtonName[0x80];
	sprintf(highlightButtonName, "Ecl_superhighlight %s", name);
	const auto highlightButton = EclGetButton(highlightButtonName);
	if (highlightButton == nullptr)
		return;

	if (highlightButton->X != button->X || highlightButton->Y != button->Y)
	{
		highlightButton->X = button->X - superhighlight_borderwidth;
		highlightButton->Y = button->Y - superhighlight_borderwidth;
	}
}

void EclUpdateAllAnimations()
{
	const auto superHighlightedButtons = superhighlightedbuttons.ConvertIndexToDArray();
	for (auto index = 0; index < superHighlightedButtons->Size(); index++)
	{
		if (!superHighlightedButtons->ValidIndex(index))
			continue;

		EclUpdateSuperHighlights(superHighlightedButtons->GetData(index));
	}

	delete superHighlightedButtons;

	for (int i = 0; i < anims.Size(); i++)
	{
		if (!anims.ValidIndex(i))
			continue;

		const auto animation = anims[i];
		assert(animation);

		const auto animationButton = EclGetButton(animation->ButtonName);
		animation->Button = animationButton;

		if (animationButton == 0)
		{
			EclRemoveAnimation(i);
			continue;
		}

		const auto currentTime = EclGetAccurateTime();
		const auto finishTime = animation->FinishTime;

		if (currentTime >= finishTime || !animsenabled)
		{
			const auto stepX = animation->StepX;
			const auto stepY = animation->StepY;
			if (stepX != 0.0f || stepY != 0.0f)
			{
				const auto button = animation->Button;
				button->X = animation->ToX;
				button->Y = animation->ToY;
			}

			const auto stepWidth = animation->StepWidth;
			const auto stepHeight = animation->StepHeight;
			if (stepWidth != 0.0f || stepHeight != 0.0f)
			{
				const auto button = animation->Button;
				button->Width = animation->ToWidth;
				button->Height = animation->ToHeight;
			}

			float stepCaption = animation->StepCaption;
			if (stepCaption != 0.0f)
			{
				animation->Button->SetCaption(animation->TargetCaption);
				if (EclIsSuperHighlighted(animation->ButtonName))
				{
					EclUpdateSuperHighlights(animation->ButtonName);
				}
			}

			if (animation->FinishedCallback != nullptr)
			{
				animation->FinishedCallback();
			}

			EclRemoveAnimation(i);
			continue;
		}

		switch (animation->MoveType)
		{
			case 1:
				if (animation->StepX != 0 || animation->StepY != 0)
				{
					animation->Button->X = animation->FromX + animation->StepX * (EclGetAccurateTime() - animation->StartTime);
					animation->Button->Y = animation->FromY + animation->StepY * (EclGetAccurateTime() - animation->StartTime);
				}
				break;
			case 2:
				UplinkAbort("TODO: implement EclUpdateAllAnimations()");
			case 3:
				UplinkAbort("TODO: implement EclUpdateAllAnimations()");
		}

		if (animation->StepWidth != 0.0f || animation->StepHeight != 0.0f)
		{
			animation->Button->Width = animation->FromWidth + animation->StepWidth * (EclGetAccurateTime() - animation->StartTime);
			animation->Button->Height = animation->FromHeight + animation->StepHeight * (EclGetAccurateTime() - animation->StartTime);
		}

		if (animation->StepCaption != 0.0f)
		{
			const size_t currentCharCount = animation->StepCaption * (EclGetAccurateTime() - animation->StartTime);
			const auto targetCharCount = strlen(animation->TargetCaption) + 1;
			const auto newCaption = new char[targetCharCount];
			strcpy(newCaption, animation->TargetCaption);

			if (currentCharCount < targetCharCount)
				newCaption[currentCharCount] = 0;

			animation->Button->SetCaption(newCaption);
			if (newCaption != nullptr)
				delete[] newCaption;
		}

		if (EclIsSuperHighlighted(animation->ButtonName))
			EclUpdateSuperHighlights(animation->ButtonName);
	}
}

int EclRegisterAnimation(const char* buttonName, int x, int y, int moveType, int width, int height, const char* caption, int time,
						 AnimationFinishedCallback finishedCallback)
{
	const auto button = EclGetButton(buttonName);

	if (button == 0)
		return -1;

	const auto animation = new Animation();
	animation->ButtonName = new char[strlen(buttonName) + 1];
	strcpy(animation->ButtonName, buttonName);

	animation->MoveType = moveType;
	animation->Button = button;
	animation->FromX = button->X;
	animation->ToX = x;
	animation->FromY = button->Y;
	animation->ToY = y;
	animation->FromWidth = button->Width;
	animation->FromHeight = button->Height;
	animation->ToWidth = width;
	animation->ToHeight = height;
	animation->FinishedCallback = finishedCallback;
	animation->Time = time;

	if (animsfasterenabled)
		animation->Time /= animsfasterspeed;

	animation->StartTime = EclGetAccurateTime();

	const auto currentTime = EclGetAccurateTime();
	const auto finishTime = currentTime + animation->Time;
	bool finishTimeIsStartTime = finishTime == animation->StartTime;

	animation->FinishTime = finishTime;
	if (finishTimeIsStartTime)
		animation->FinishTime = finishTime + 1;

	if (animation->ToX == animation->FromX)
	{
		animation->StepX = 0.0f;
		animation->StepY = 0.0f;
	}
	else
	{
		const float time = animation->FinishTime - animation->StartTime;
		animation->StepX = (animation->ToX - animation->FromX) / time;
		animation->StepY = (animation->ToY - animation->FromY) / time;
	}

	if (animation->ToWidth == animation->FromWidth)
	{
		animation->StepWidth = 0.0f;
		animation->StepHeight = 0.0f;
	}
	else
	{
		const float time = animation->FinishTime - animation->StartTime;
		animation->StepWidth = (animation->ToWidth - animation->FromWidth) / time;
		animation->StepHeight = (animation->ToHeight - animation->FromHeight) / time;
	}

	if (caption == nullptr)
	{
		animation->TargetCaption = nullptr;
		animation->StepCaption = 0.0f;
	}
	else
	{
		while (true)
		{
			const auto captionAnimation = EclIsCaptionChangeActive(buttonName);
			if (captionAnimation == -1)
				break;

			EclRemoveAnimation(captionAnimation);
		}

		char* targetCaption = animation->TargetCaption;
		if (targetCaption != nullptr)
			delete[] targetCaption;

		const auto captionLength = strlen(caption);
		animation->TargetCaption = new char[captionLength + 1];
		strcpy(animation->TargetCaption, caption);

		const float time = animation->FinishTime - animation->StartTime;
		animation->StepCaption = captionLength / time;
	}

	return anims.PutData(animation);
}

int EclRegisterCaptionChange(const char* buttonName, const char* caption, int time, AnimationFinishedCallback finishedCallback)
{
	const auto button = EclGetButton(buttonName);

	if (button == 0)
		return -1;

	return EclRegisterAnimation(buttonName, button->X, button->Y, 1, button->Width, button->Height, caption, time, finishedCallback);
}

int EclRegisterCaptionChange(const char* buttonName, const char* caption, AnimationFinishedCallback finishedCallback)
{
	return EclRegisterCaptionChange(buttonName, caption, strlen(caption) * 50, finishedCallback);
}

int EclRegisterMovement(const char* buttonName, int x, int y, int time, int moveType, AnimationFinishedCallback callback)
{
	const auto button = EclGetButton(buttonName);

	if (button == nullptr)
		return -1;

	return EclRegisterAnimation(buttonName, x, y, moveType, button->Width, button->Height, nullptr, time, callback);
}

int EclRegisterMovement(const char* buttonName, int x, int y, int time, AnimationFinishedCallback callback)
{
	return EclRegisterMovement(buttonName, x, y, time, 1, callback);
}
int EclRegisterAnimation(const char* name, int x, int y, int width, int height, int time, AnimationFinishedCallback callback)
{
	return EclRegisterAnimation(name, x, y, 1, width, height, nullptr, time, callback);
}

int EclRegisterResize(const char* buttonName, int32_t width, int32_t height, int32_t time, AnimationFinishedCallback callback)
{
	const auto button = EclGetButton(buttonName);

	if (button == nullptr)
		return -1;

	return EclRegisterAnimation(buttonName, button->X, button->Y, width, height, time, callback);
}

int EclIsAnimationActive(const char* name)
{
	for (auto i = 0; i < anims.Size(); i++)
	{
		if (!anims.ValidIndex(i))
			continue;

		const auto anim = anims[i];
		assert(anim != nullptr);
		if (strcmp(name, anim->ButtonName) == 0)
			return i;
	}

	return -1;
}

void EclButtonBringToFront(const char* name)
{
	const auto index = EclLookupIndex(name);

	if (!buttons.ValidIndex(index))
		return;

	const auto button = buttons[index];
	buttons.RemoveData(index);
	buttons.PutDataAtStart(button);
}

void EclButtonSendToBack(const char* name)
{
	const auto index = EclLookupIndex(name);

	if (!buttons.ValidIndex(index))
		return;

	const auto button = buttons[index];
	buttons.RemoveData(index);
	buttons.PutDataAtEnd(button);
}

void EclRegisterButtonCallback(const char* name, ButtonMouseUpFunc func)
{
	const auto index = EclLookupIndex(name);
	if (buttons.ValidIndex(index))
		buttons[index]->RegisterMouseUpFunction(func);
}

const char* EclGetButtonAtCoord(int x, int y)
{
	for (auto i = 0; i < buttons.Size(); i++)
	{
		if (!buttons.ValidIndex(i))
			continue;

		const auto button = buttons[i];
		if (x >= button->X && x <= button->X + button->Width && y >= button->Y && y <= button->Y + button->Height)
			return button->Name;
	}
	return nullptr;
}

void EclClickButton(const char* name)
{
	if (EclIsClicked(name))
		return;

	EclUnClickButton();
	if (currentclick != nullptr)
		delete[] currentclick;

	currentclick = new char[strlen(name) + 1];
	strcpy(currentclick, name);
}

void EclUnClickButton()
{
	EclGetButton(currentclick);

	if (currentclick != nullptr)
		delete[] currentclick;

	currentclick = nullptr;
}

void GciSaveScreenshot(const char* filePath)
{
	(void)filePath;
	puts("TODO: implement GciSaveScreenshot()");
}

void EclHighlightNextEditableButton()
{
	if (currenthighlight != nullptr)
	{
		for (auto i = 0; i < editablebuttons.Size() - 1; i++)
		{
			if (!editablebuttons.ValidIndex(i))
				continue;

			if (strcmp(editablebuttons.GetData(i), currenthighlight) != 0)
				continue;

			if (!editablebuttons.ValidIndex(i + 1))
				continue;

			EclHighlightButton(editablebuttons.GetData(i + 1));
			return;
		}
	}

	if (editablebuttons.ValidIndex(0))
		EclHighlightButton(editablebuttons.GetData(0));
}

void textbutton_keypress(Button* button, char c)
{
	UplinkAssert(button != nullptr);

	// Backspace
	if (c == 8)
	{
		if (button->Caption[0] != 0)
			button->Caption[strlen(button->Caption) - 1] = 0;

		return;
	}
	// Escape

	if (c == 27)
	{
		button->SetCaption("");
		return;
	}

	const auto newLen = strlen(button->Caption) + 2;
	char* newStr = new char[newLen];

	// Enter
	if (c == '\r')
		c = '\n';

	if (newLen != 0)
		UplinkSnprintf(newStr, newLen, "%s%c", button->Caption, c);

	button->SetCaption(newStr);
	delete[] newStr;
}
