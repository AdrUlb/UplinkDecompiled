#include <DialogScreenWidget.hpp>

#include <Util.hpp>

DialogScreenWidget::~DialogScreenWidget()
{
	if (_name != nullptr)
		delete[] _name;

	if (_caption != nullptr)
		delete[] _caption;

	if (_tooltip != nullptr)
		delete[] _tooltip;

	if (_stringData1 != nullptr)
		delete[] _stringData1;

	if (_stringData2 != nullptr)
		delete[] _stringData2;
}

bool DialogScreenWidget::Load(FILE* file)
{
	if (!LoadDynamicString(_name, file))
		return false;

	if (!FileReadData(&_x, 4, 1, file))
		return false;

	if (!FileReadData(&_y, 4, 1, file))
		return false;

	if (!FileReadData(&_width, 4, 1, file))
		return false;

	if (!FileReadData(&_height, 4, 1, file))
		return false;

	if (!FileReadData(&_type, 4, 1, file))
		return false;

	if (!LoadDynamicString(_caption, file))
		return false;

	if (!LoadDynamicString(_tooltip, file))
		return false;

	if (!FileReadData(&_data1, 4, 1, file))
		return false;

	if (!FileReadData(&_data2, 4, 1, file))
		return false;

	if (!LoadDynamicString(_stringData1, file))
		return false;

	if (!LoadDynamicString(_stringData2, file))
		return false;

	return true;
}

void DialogScreenWidget::Save(FILE* file)
{
	SaveDynamicString(_name, file);
	fwrite(&_x, 4, 1, file);
	fwrite(&_y, 4, 1, file);
	fwrite(&_width, 4, 1, file);
	fwrite(&_height, 4, 1, file);
	fwrite(&_type, 4, 1, file);
	SaveDynamicString(_caption, file);
	SaveDynamicString(_tooltip, file);
	fwrite(&_data1, 4, 1, file);
	fwrite(&_data2, 4, 1, file);
	SaveDynamicString(_stringData1, file);
	SaveDynamicString(_stringData2, file);
}

void DialogScreenWidget::Print()
{
	printf("DialogScreenWidget : Name:%s\n", _name);
	printf("\tTYPE:%d, x:%d, y:%d, width:%d, height:%d\n", _type, _x, _y, _width, _height);
	printf("\tcaption:%s, tooltip:%s\n", _caption, _tooltip);
	printf("\tdata1:%d, data2:%d, stringdata1:%s, stringdata2:%s\n", _data1, _data2, _stringData1, _stringData2);
}

const char* DialogScreenWidget::GetID()
{
	return "DLG_WIDG";
}

UplinkObjectId DialogScreenWidget::GetOBJECTID()
{
	return UplinkObjectId::DialogScreenWidget;
}

int DialogScreenWidget::GetX()
{
	return _x;
}

int DialogScreenWidget::GetY()
{
	return _y;
}

int DialogScreenWidget::GetWidth()
{
	return _width;
}

int DialogScreenWidget::GetHeight()
{
	return _height;
}

int DialogScreenWidget::GetType()
{
	return _type;
}

int DialogScreenWidget::GetData1()
{
	return _data1;
}

int DialogScreenWidget::GetData2()
{
	return _data2;
}

void DialogScreenWidget::SetName(const char* value)
{
	if (_name != nullptr)
		delete[] _name;

	_name = new char[strlen(value) + 1];
	strcpy(_name, value);
}

void DialogScreenWidget::SetTYPE(int value)
{
	_type = value;
}

void DialogScreenWidget::SetPosition(int x, int y)
{
	this->_x = x;
	this->_y = y;
}

void DialogScreenWidget::SetSize(int width, int height)
{
	this->_width = width;
	this->_height = height;
}

void DialogScreenWidget::SetCaption(const char* value)
{
	if (_caption != nullptr)
		delete[] _caption;

	_caption = new char[strlen(value) + 1];
	strcpy(_caption, value);
}

void DialogScreenWidget::SetTooltip(const char* value)
{
	if (_tooltip != nullptr)
		delete[] _tooltip;

	_tooltip = new char[strlen(value) + 1];
	strcpy(_tooltip, value);
}

void DialogScreenWidget::SetData(int data1, int data2)
{
	this->_data1 = data1;
	this->_data2 = data2;
}

void DialogScreenWidget::SetStringData(const char* stringData1, const char* stringData2)
{
	if (this->_stringData1 != nullptr)
	{
		delete[] this->_stringData1;
		this->_stringData1 = nullptr;
	}

	if (stringData1 != nullptr)
	{
		this->_stringData1 = new char[strlen(stringData1) + 1];
		strcpy(this->_stringData1, stringData1);
	}

	if (this->_stringData2 != 0)
	{
		delete[] this->_stringData2;
		this->_stringData2 = nullptr;
	}

	if (stringData2 != nullptr)
	{
		this->_stringData2 = new char[strlen(stringData2) + 1];
		strcpy(this->_stringData2, stringData2);
	}
}

const char* DialogScreenWidget::GetName()
{
	return _name;
}

const char* DialogScreenWidget::GetCaption()
{
	return _caption;
}

const char* DialogScreenWidget::GetTooltip()
{
	return _tooltip;
}
