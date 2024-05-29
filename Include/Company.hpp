#pragma once

#include <LList.hpp>
#include <Mission.hpp>
#include <News.hpp>
#include <Sale.hpp>
#include <UplinkObject.hpp>

class Company : public UplinkObject
{
public:
	int sharePrices[12] = {0};
	int sharePriceLastMonth = 0;
	char name[0x40] = " ";
	char boss[0x80] = "Unlisted";
	char admin[0x80] = "Unlisted";
	int size;
	int type;
	int growth;
	int alignment;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void Grow(int amount);
	void VaryGrowth();
	void SetName(const char* name);
	void SetSize(int value);
	void SetTYPE(int value);
	void SetGrowth(int value);
	void SetAlignment(int value);
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
