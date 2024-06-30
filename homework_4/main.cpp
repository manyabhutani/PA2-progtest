#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate {
public:
    CDate(int y,int m,int d): m_Y(y),m_M(m),m_D(d) {
    }

    std::strong_ordering operator<=>(const CDate &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};
#endif /* __PROGTEST__ */


class CStudent {
public:
    CStudent(const std::string &name,const CDate &born,int enrolled):m_name(name) , m_born(born) , m_enrolled(enrolled) , {}

    bool operator==(const CStudent &other) const{
        return m_name == other.m_name && m_born == other.m_born && m_enrolled == other.m_enrolled;
    }

    bool operator!=(const CStudent &other) const{
        return !(*this == other);
    }

    bool operator < (const CStudent &other) const{
        if(m_name != other.m_name) return m_name > other.m_name;
        if(m_born != other.m_born) return  m_born > other.m_born;
        return m_enrolled < other.m_enrolled;
    }


    const std::string& getName() const{
        return  m_name;
    }

    const CDate& getDateOfBirth() const{
        return m_born;
    }

    int getEnrolledYear() const {
        return m_enrolled;
    }

    int getStudentId() const {
        return m_id;
    }


// use multiset and vector for sorted by names

private:
    std::string m_name;
    CDate m_born;
    int m_enrolled;
    int m_id;

};

class CFilter
{
public:
    CFilter(){}

    CFilter & name ( const std::string & name ){
        m_Name.emplace(name);
        return *this;
    }
    CFilter & bornBefore   ( const CDate  & date ){
        m_BornBefore.emplace(date);
        return *this;
    }
    CFilter  & bornAfter ( const CDate  & date ){
        m_BornAfter.emplace(date);
        return *this;
    }
    CFilter & enrolledBefore ( int year ){
        m_EnrolledBefore.emplace(year);
        return *this;
    }

    CFilter & enrolledAfter ( int  year ){
        m_EnrolledAfter.emplace(year);
        return *this;
    }

    bool matches(const CStudent& student) const{
        if (m_Name && !nameMatches(student.getName(), m_Name.value()))
            return false;
        if (m_BornBefore && !(student.getDateOfBirth() < m_BornBefore.value()))
            return false;
        if (m_BornAfter && !(student.getDateOfBirth() > m_BornAfter.value()))
            return false;
        if (m_EnrolledBefore && !(student.getEnrolledYear() < m_EnrolledBefore.value()))
            return false;
        if (m_EnrolledAfter && !(student.getEnrolledYear() > m_EnrolledAfter.value()))
            return false;
        return true;
    }

private:
    std::optional<std::string> m_Name;
    std::optional<CDate> m_BornBefore, m_BornAfter;
    std::optional<int> m_EnrolledBefore, m_EnrolledAfter;



    static bool nameMatches(const std::string& studentName, const std::string& filterName) {
        auto normStudentName = splitToLower(studentName);
        auto normFilterName = splitToLower(filterName);
        std::sort(normStudentName.begin(), normStudentName.end());
        std::sort(normFilterName.begin(), normFilterName.end());
        return normStudentName == normFilterName;
    }

    static std::vector<std::string> splitToLower(const std::string& str) {
        std::vector<std::string> words;
        std::string currentWord;

        for (char ch : str) {
            if (std::isspace(static_cast<unsigned char>(ch))) {
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
            } else {
                currentWord += std::tolower(static_cast<unsigned char>(ch));
            }
        }

        if (!currentWord.empty()) {
            words.push_back(currentWord);
        }

        return words;
    }


};

class CSort
{
public:
    CSort(){}
    CSort   & addKey( ESortKey  key ,bool ascending ){
        m_SortKeys.emplace_back(key,ascending);
        return *this;
    }

    bool isEmpty() const {
        return m_SortKeys.empty();
    }


    bool operator()(const CStudent& lhs, const CStudent& rhs) const {
        for (const auto& [key, asc] : m_SortKeys) {
            std::strong_ordering comparison = std::strong_ordering::equal;
            switch (key) {
                case ESortKey::NAME:
                    if (auto cmp = lhs.getName().compare(rhs.getName()); cmp != 0)
                        return asc ? cmp < 0 : cmp > 0;
                    break;
                case ESortKey::BIRTH_DATE:
                    comparison = lhs.getDateOfBirth() <=> rhs.getDateOfBirth();
                    if (comparison != std::strong_ordering::equal)
                        return asc ? (comparison < 0) : (comparison > 0);
                    break;
                case ESortKey::ENROLL_YEAR:
                    if (int cmp = lhs.getEnrolledYear() - rhs.getEnrolledYear(); cmp != 0)
                        return asc ? cmp < 0 : cmp > 0;
                    break;
            }
        }
        return false;
    }


private:
    std::vector<std::pair<ESortKey, bool>> m_SortKeys;
};



class CStudyDept {
public:
    CStudyDept(){}

    bool addStudent(const CStudent &x){


    }

    bool delStudent(const CStudent &x) {

    }


    std::list<CStudent> search(const CFilter &flt, const CSort &sortOpt) const {

    }

    std::set<std::string> suggest(const std::string &name) const {

    }


private:

// map <Cstudent , and a list iterator > and list of student<Cstudent>

};




#ifndef __PROGTEST__
int main ( void )
{
    CStudyDept x0;
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) ) );
    assert ( x0 . addStudent ( CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ) ) );
    assert ( x0 . addStudent ( CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ) ) );
    assert ( x0 . addStudent ( CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ) ) );
    assert ( x0 . addStudent ( CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ) ) );
    assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ) ) );
    assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ) ) );
    assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ) ) );
    assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ) ) );
    assert ( x0 . addStudent ( CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ) ) );




    assert ( x0 . search ( CFilter (), CSort () ) == (std::list<CStudent>
            {
                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
            }) );


//
//    std::list<CStudent> result =  x0 . search ( CFilter (), CSort () . addKey ( ESortKey::NAME, true ));
//
//    for (const auto& student : result) {
//        std::cout << "Name: " << std::setw(20) << std::left << student.getName()
//                  << " Date of Birth: " << student.getDateOfBirth()
//                  << " Enrolled Year: " << student.getEnrolledYear() << std::endl;
//    }
//    assert ( x0 . search ( CFilter (), CSort () . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
//            {
//                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
//                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
//                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
//                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
//                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
//            }) );



//    assert ( x0 . search ( CFilter (), CSort () . addKey ( ESortKey::NAME, false ) ) == (std::list<CStudent>
//            {
//                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
//                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
//                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
//                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
//                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
//            }) );
//    assert ( x0 . search ( CFilter (), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
//            {
//                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
//                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
//                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
//                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
//                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
//                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
//            }) );
//    assert ( x0 . search ( CFilter () . name ( "james bond" ), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
//            {
//                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
//                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 )
//            }) );
//    assert ( x0 . search ( CFilter () . bornAfter ( CDate ( 1980, 4, 11) ) . bornBefore ( CDate ( 1983, 7, 13) ) . name ( "John Taylor" ) . name ( "james BOND" ), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
//            {
//                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
//                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
//                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
//            }) );
    assert ( x0 . search ( CFilter () . name ( "james" ), CSort () . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
            {
            }) );
    assert ( x0 . suggest ( "peter" ) == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }) );
    assert ( x0 . suggest ( "bond" ) == (std::set<std::string>
            {
                    "Bond James",
                    "James Bond"
            }) );
    assert ( x0 . suggest ( "peter joHn" ) == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }) );
    assert ( x0 . suggest ( "peter joHn bond" ) == (std::set<std::string>
            {
            }) );
    assert ( x0 . suggest ( "pete" ) == (std::set<std::string>
            {
            }) );
    assert ( x0 . suggest ( "peter joHn PETER" ) == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }) );
//    assert ( ! x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    assert ( x0 . delStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
//    assert ( x0 . search ( CFilter () . bornAfter ( CDate ( 1980, 4, 11) ) . bornBefore ( CDate ( 1983, 7, 13) ) . name ( "John Taylor" ) . name ( "james BOND" ), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
//            {
//                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
//                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
//                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
//                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
//            }) );
    assert ( ! x0 . delStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
