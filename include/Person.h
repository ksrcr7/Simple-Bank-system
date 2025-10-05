

#ifndef BANK_ACCOUNT_PERSON_H
#define BANK_ACCOUNT_PERSON_H

#include <iostream>
using namespace std;



class Person{
public:
    Person()=default;
    struct BirthDate{
        string day,month,year;
    };
    bool SetName(string,string*);
    bool SetFamilyName(string,string*);
    bool SetNationality(string,string*);
    bool SetIdCode(const string&,string*);
    bool SetGender(const string&,string*);
    bool SetBirthday(const string&,const string&,const string&,string*);

    [[nodiscard]] string GetName()const;
    [[nodiscard]] string GetFamilyName()const;
    [[nodiscard]] string GetNationality()const;
    [[nodiscard]] string GetIdCode()const;
    [[nodiscard]] static string GetGender(bool);
    [[nodiscard]] const BirthDate& GetBirthDate()const;

    void DisplayPersonInfo()const;


private:
    string Name,FamilyName,Nationality,IdCode;
    bool Gender{};
    BirthDate birthdate_;
    static bool ValidName(const string&);

};



























#endif //BANK_ACCOUNT_PERSON_H
