#include <DialogScreenWidget.hpp>

#include <Util.hpp>

DialogScreenWidget::~DialogScreenWidget()
{
	if (name != nullptr)
		delete[] name;

	if (caption != nullptr)
		delete[] caption;

	if (tooltip != nullptr)
		delete[] tooltip;

	if (stringData1 != nullptr)
		delete[] stringData1;

	if (stringData2 != nullptr)
		delete[] stringData2;
}

bool DialogScreenWidget::Load(FILE* file)
{
	if (!LoadDynamicString(name, file))
		return false;

	if (!FileReadData(&x, 4, 1, file))
		return false;

	if (!FileReadData(&y, 4, 1, file))
		return false;

	if (!FileReadData(&width, 4, 1, file))
		return false;

	if (!FileReadData(&height, 4, 1, file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	if (!LoadDynamicString(caption, file))
		return false;

	if (!LoadDynamicString(tooltip, file))
		return false;

	if (!FileReadData(&data1, 4, 1, file))
		return false;

	if (!FileReadData(&data2, 4, 1, file))
		return false;

	if (!LoadDynamicString(stringData1, file))
		return false;

	if (!LoadDynamicString(stringData2, file))
		return false;

	return true;
}

void DialogScreenWidget::Save(FILE* file)
{
	SaveDynamicString(name, file);
	fwrite(&x, 4, 1, file);
	fwrite(&y, 4, 1, file);
	fwrite(&width, 4, 1, file);
	fwrite(&height, 4, 1, file);
	fwrite(&type, 4, 1, file);
	SaveDynamicString(caption, file);
	SaveDynamicString(tooltip, file);
	fwrite(&data1, 4, 1, file);
	fwrite(&data2, 4, 1, file);
	SaveDynamicString(stringData1, file);
	SaveDynamicString(stringData2, file);
}

void DialogScreenWidget::Print()
{
	printf("DialogScreenWidget : Name:%s\n", name);
	printf("\tTYPE:%d, x:%d, y:%d, width:%d, height:%d\n", type, x, y, width, height);
	printf("\tcaption:%s, tooltip:%s\n", caption, tooltip);
	printf("\tdata1:%d, data2:%d, stringdata1:%s, stringdata2:%s\n", data1, data2, stringData1, stringData2);
}

const char* DialogScreenWidget::GetID()
{
	return "DLG_WIDG";
}

UplinkObjectId DialogScreenWidget::GetOBJECTID()
{
	return UplinkObjectId::DialogScreenWidget;
}

void DialogScreenWidget::SetName(const char* value)
{
	if (name != nullptr)
		delete[] name;

	name = new char[strlen(value) + 1];
	strcpy(name, value);
}

void DialogScreenWidget::SetTYPE(int value)
{
	type = value;
}

void DialogScreenWidget::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void DialogScreenWidget::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void DialogScreenWidget::SetCaption(const char* value)
{
	if (caption != nullptr)
		delete[] caption;

	caption = new char[strlen(value) + 1];
	strcpy(caption, value);
}

void DialogScreenWidget::SetTooltip(const char* value)
{
	if (tooltip != nullptr)
		delete[] tooltip;

	tooltip = new char[strlen(value) + 1];
	strcpy(tooltip, value);
}

void DialogScreenWidget::SetData(int data1, int data2)
{
	this->data1 = data1;
	this->data2 = data2;
}

void DialogScreenWidget::SetStringData(const char* stringData1, const char* stringData2)
{
	if (this->stringData1 != nullptr)
	{
		delete[] this->stringData1;
		this->stringData1 = nullptr;
	}

	if (stringData1 != nullptr)
	{
		this->stringData1 = new char[strlen(stringData1) + 1];
		strcpy(this->stringData1, stringData1);
	}

	if (this->stringData2 != 0)
	{
		delete[] this->stringData2;
		this->stringData2 = nullptr;
	}

	if (stringData2 != nullptr)
	{
		this->stringData2 = new char[strlen(stringData2) + 1];
		strcpy(this->stringData2, stringData2);
	}
}

const char* DialogScreenWidget::GetName()
{
	return name;
}

const char* DialogScreenWidget::GetCaption()
{
	return caption;
}

const char* DialogScreenWidget::GetTooltip()
{
	return tooltip;
}
