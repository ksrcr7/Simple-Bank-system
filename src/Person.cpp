#include "Person.h"
#include <regex>
#include <algorithm>
#include <iomanip>
#include <utility>



static inline void trim(string &name){
    auto not_space = [](unsigned char ch){return !isspace(ch);};
    name.erase(name.begin(),find_if(name.begin(),name.end(),not_space));
    name.erase(find_if(name.rbegin(),name.rend(),not_space).base(),name.end());
}

static inline string trim_copy(const string& id){
    auto start = find_if(id.begin(),id.end(),[]
            (unsigned char ch){return !isspace(ch);});
    auto end = find_if(id.rbegin(),id.rend(),[]
            (unsigned char ch){return !isspace(ch);}).base();

    if(start >= end)return {};
    return string(start,end);
}

static inline bool is_Digit(const string& id){
    return std::all_of(id.begin(),id.end(),
                       [](unsigned char ch){return isdigit(ch);});

}

static inline bool is_leap(int y){
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

static inline int days_in_month(int m, int y){
    switch (m) {
        case 1:case 3:case 5:case 7:case 8:case 10:case 12:return 31;
        case 4:case 6:case 9:case 11:return 30;
        case 2:return is_leap(y)? 29:28;
        default:return 0;

    }

}
static inline string pad2(int v){
    char buf[3];
    std::snprintf(buf, sizeof(buf),"%02d",v);
    return string(buf);
}




bool Person::ValidName(const string &name) {
    string out = name;
    trim(out);
    if(out.empty())return false;
    static const regex re(R"(^[A-Za-z]+( [A-Za-z]+)*$)");
    return regex_match(out,re);

}

bool Person::SetName(string name,string* err) {
    string s = std::move(name);
    trim(s);
    if(ValidName(s)){
        Name = std::move(s);
        if(err) err->clear();
        return true;
    }
    if (err) *err = "Error! invalid name.";
    return false;

}

bool Person::SetFamilyName(string familyname,string *err) {
    if(ValidName(familyname)){
        this->FamilyName = std::move(familyname);
        return true;
    }
    else{
        if(err) *err = "Error! invalid FamilyName.";
        return false;
    }
}

bool Person::SetNationality(string nationality,string *err) {
    if(ValidName(nationality)){
        this->Nationality = std::move(nationality);
        return true;
    }
    else{
        if(err) *err = "Error! invalid Nationality.";
        return false;
    }
}

bool Person::SetIdCode(const string& idcode,string *err) {
    string out = trim_copy(idcode);
    if(out.empty()){
        if(err) *err = "Error! IdCode must not be empty.";
        return false;
    }
    if(out.size() > 5){
        if(err) *err = "Error! IdCode must be 5 digits.";
        return false;
    }
    if(!is_Digit(out)){
        if(err) *err = "Error! IdCode must include only Digits.";
        return false;
    }


    this->IdCode = std::move(out);
    if (err) err->clear();
    return true;

}

bool Person::SetGender(const string &gender,string *err) {
    if(gender =="1"){
        this->Gender = true;
        return true;
    }
    else if(gender == "2"){
        this->Gender = false;
        return true;
    }
    else{
        if(err) *err = "Error! choose the right Gender.";
        return false;
    }
}

bool Person::SetBirthday(const string &D, const string &M, const string &Y, string *err) {
    auto fail = [&](const char* msg){
        if(err) *err = msg;
        return false;
    };

    string DAY = trim_copy(D);
    string MONTH = trim_copy(M);
    string YEAR = trim_copy(Y);

    if(!is_Digit(DAY))return fail("Error! day must be digits only.");
    if(!is_Digit(MONTH))return fail("Error! month must be digits only.");
    if(!is_Digit(YEAR))return fail("Error! year must be digits only.");

    if(YEAR.size() != 4)return fail("Error! year must be 4 digits(e.g. 2025)");
    if(MONTH.size() > 2)return fail("Error! month must be at most 2 digits.");
    if(DAY.size() > 2)return fail("Error! day must be at most 2 digits");

    int yi = stoi(YEAR);
    int mi = stoi(MONTH);
    int di = stoi(DAY);

    if(yi < 1900 || yi > 2007)return fail("Error! year out of range.");
    if(mi < 1 || mi > 12)return fail("Error! month out of range [1-12]");

    int dim = days_in_month(mi,yi);
    if(di < 1 || di > dim)return fail("Error! day out of range for given year/month");

    birthdate_.day = pad2(di);
    birthdate_.month = pad2(mi);
    birthdate_.year = YEAR;

    if(err) err->clear();
    return true;

}

string Person::GetName() const {
    return this->Name;
}

string Person::GetFamilyName() const {
    return this->FamilyName;
}

string Person::GetNationality() const {
    return this->Nationality;
}

string Person::GetIdCode() const {
    return this->IdCode;
}

string Person::GetGender(bool gender) {
    return gender ? "Man" : "Woman";
}

const Person::BirthDate &Person::GetBirthDate() const {
    return birthdate_;
}

void Person::DisplayPersonInfo() const {
    cout<<left<<setw(15)<<"Name:"<<this->Name<<endl;
    cout<<left<<setw(15)<<"FamilyName:"<<this->FamilyName<<endl;
    cout<<left<<setw(15)<<"Nationality:"<<this->Nationality<<endl;
    cout<<left<<setw(15)<<"IdCode:"<<this->IdCode<<endl;
    cout<<left<<setw(15)<<"Birthday:"<<birthdate_.day<<"/"<<birthdate_.month<<"/"<<birthdate_.year<<endl;
}


