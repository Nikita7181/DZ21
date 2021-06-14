#pragma once
#include "record.h"

class student : virtual public record
{
std::vector<discipline> discipline_list;
public:
student();
virtual void print() const override;
virtual bool isNegative ();
virtual void add_discipline() override;
virtual void edit_discipline () override;
virtual void del(int indx) override;
virtual std::string serialize ()  override;
void findBegEnd (std::string & str, int & beg, int & end);
std::string getValue (std::string str, int beg, int end);
virtual void deserialize (std::string str) override;
};