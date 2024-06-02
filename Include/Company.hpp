#pragma once

#include <LList.hpp>
#include <Mission.hpp>
#include <News.hpp>
#include <Sale.hpp>
#include <UplinkObject.hpp>

class Company : public UplinkObject
{
	int _sharePrices[12] = {0};
	int _sharePriceLastMonth = 0;
	char _name[0x40] = " ";
	char _boss[0x80] = "Unlisted";
	char _admin[0x80] = "Unlisted";
	int _size;
	int _type;
	int _growth;
	int _alignment;

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	const char* GetName();
	int GetSize();
	void SetName(const char* name);
	void SetSize(int value);
	void SetTYPE(int value);
	void SetGrowth(int value);
	void SetAlignment(int value);
	void Grow(int amount);
	void VaryGrowth();
};

class CompanyUplink : public Company
{
	LList<Mission*> missions;
	LList<Sale*> hardwareSales;
	LList<Sale*> softwareSales;
	LList<News*> news;

public:
	CompanyUplink();
	~CompanyUplink() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
};
