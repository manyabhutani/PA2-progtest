#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>

#endif /* __PROGTEST__ */


class CComponent {
public:
    virtual ~CComponent() {}

    virtual CComponent *clone() const = 0;

    virtual void
    print(std::ostream &os, bool isLastComponent = false, bool isOnly = false, bool isOnlyComputer = false) const = 0;
};

class CCPU : public CComponent {
    int cores;
    int frequency;
public:
    CCPU(int cores, int frequency) : cores(cores), frequency(frequency) {}

    CCPU *clone() const override {
        return new CCPU(*this);
    }

    void print(std::ostream &os, bool isLast = false, bool isOnly = false, bool isOnlyComputer = false) const override {
        os << "CPU, " << cores << " cores @ " << frequency << "MHz\n";
        if (!isOnly || !isLast) {
            os << "";
        }
    }
};

class CMemory : public CComponent {
    int size;
public:
    CMemory(int size) : size(size) {}

    CMemory *clone() const override {
        return new CMemory(*this);
    }

    void print(std::ostream &os, bool isLast = false, bool isOnly = false, bool isOnlyComputer = false) const override {
        os << "Memory, " << size << " MiB\n";
        if (!isOnly || !isLast) {
            os << "";
        }

    }
};

class CDisk : public CComponent {
public:
    enum Type {
        SSD, MAGNETIC
    } type;
private:
    int size;
    std::vector<std::pair<int, std::string>> partitions;

public:
    CDisk(Type type, int size) : type(type), size(size) {}

    CDisk *clone() const override {
        return new CDisk(*this);
    }

    CDisk &addPartition(int size, const std::string &name) {
        partitions.emplace_back(size, name);
        return *this;
    }

    void print(std::ostream &os, bool isLastComponent, bool isInLastComputer, bool isOnlyComputer) const override {
        os << (type == SSD ? "SSD" : "HDD") << ", " << size << " GiB";
        for (size_t i = 0, n = partitions.size(); i < n; i++) {
            if (isInLastComputer && !isOnlyComputer) {
                if (!isLastComponent) {
                    os << "\n" << (i == n - 1 ? "  | \\-" : "  | +-") << "[" << i << "]: "
                       << partitions[i].first << " GiB, " << partitions[i].second;
                } else {
                    os << "\n" << (i == n - 1 ? "    \\-" : "    +-") << "[" << i << "]: "
                       << partitions[i].first << " GiB, " << partitions[i].second;
                }
            } else if (isOnlyComputer) {
                if (!isLastComponent) {
                    os << "\n" << (i == n - 1 ? "| \\-" : "| +-") << "[" << i << "]: "

                       << partitions[i].first << " GiB, " << partitions[i].second;
                } else {
                    os << "\n" << (i == n - 1 ? "  \\-" : "  +-") << "[" << i << "]: "

                       << partitions[i].first << " GiB, " << partitions[i].second;
                }
            } else {
                os << "\n| " << (((isLastComponent) ? " " : "|")) << (i == n - 1 ? " \\-" : " +-") << "[" << i << "]: "
                   << partitions[i].first << " GiB, " << partitions[i].second;
            }
        }
        os << "\n";

    }
};

class CComputer {
    std::string name;
    std::vector<std::string> addresses;
    std::vector<CComponent *> components;

public:
    CComputer(const std::string &name) : name(name) {}

    ~CComputer() {
        for (auto comp: components) {
            delete comp;
        }
    }

    CComputer(const CComputer &other) : name(other.name), addresses(other.addresses) {
        for (auto comp: other.components) {
            components.push_back(comp->clone());
        }
    }

    CComputer &operator=(CComputer other) {
        std::swap(name, other.name);
        std::swap(addresses, other.addresses);
        std::swap(components, other.components);
        return *this;
    }

    CComputer &addAddress(const std::string &address) {
        addresses.push_back(address);
        return *this;
    }

    CComputer &addComponent(const CComponent &comp) {
        components.push_back(comp.clone());
        return *this;
    }

    std::string getName() const {
        return name;
    }


    void print(std::ostream &os, bool isLastComputer = false, bool isOnlyComputer = true) const {

        if (isOnlyComputer) {
            os << "Host: " << name << "\n";
        } else {
            os << (isLastComputer ? "\\-Host: " : "+-Host: ") << name << "\n";
        }
        for (size_t j = 0; j < addresses.size(); j++) {
            if (isOnlyComputer) {
                os << "" << "+-" << addresses[j] << "\n";
            } else { os << (isLastComputer ? "" : "|") << (isLastComputer ? "  +-" : " +-") << addresses[j] << "\n"; }
        }
        for (size_t i = 0; i < components.size(); ++i) {
            bool isLastComponent = (i == components.size() - 1);


           os << (isLastComputer || isOnlyComputer ? "" : "| ")
               << ((isLastComponent && isLastComputer & !isOnlyComputer) ? "  \\" : isLastComponent ? "\\" : (
                       isLastComputer && !isOnlyComputer ? "  +" : "+")) << "-";

            components[i]->print(os, isLastComponent, isLastComputer, isOnlyComputer);
        }
    }
};


class CNetwork {
    std::string networkName;
    std::vector<CComputer *> computers;

public:

    CNetwork(const std::string &name) : networkName(name) {}

    ~CNetwork() {
        for (auto comp: computers)
            delete comp;
    }

    CNetwork(const CNetwork &other) : networkName(other.networkName) {
        for (auto comp: other.computers)
            computers.push_back(new CComputer(*comp));
    }

    CNetwork &operator=(CNetwork other) {
        swap(networkName, other.networkName);
        swap(computers, other.computers);
        return *this;
    }

    CNetwork &addComputer(const CComputer &computer) {
        computers.push_back(new CComputer(computer));
        return *this;
    }

    CComputer *findComputer(const std::string &name) const {
        for (auto comp: computers) {
            if (comp->getName() == name)
                return comp;
        }
        return nullptr;
    }

    void print(std::ostream &os) const {
        os << "Network: " << networkName << "\n";
        for (size_t i = 0; i < computers.size(); ++i) {
            computers[i]->print(os, i == computers.size() - 1, false);
        }
    }
};

std::ostream &operator<<(std::ostream &os, const CNetwork &net) {
    net.print(os);
    return os;
}

std::ostream &operator<<(std::ostream &os, const CComputer &comp) {
    comp.print(os, false, true);
    return os;
}


#ifndef __PROGTEST__

template<typename T_>
std::string toString(const T_ &x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {


    CNetwork n("FIT network");
    n.addComputer(
            CComputer("progtest.fit.cvut.cz").
                    addAddress("147.32.232.142").
                    addComponent(CCPU(8, 2400)).
                    addComponent(CCPU(8, 1200)).
                    addComponent(CDisk(CDisk::MAGNETIC, 1500).
                    addPartition(50, "/").
                    addPartition(5, "/boot").
                    addPartition(1000, "/var")).
                    addComponent(CDisk(CDisk::SSD, 60).
                    addPartition(60, "/data")).
                    addComponent(CMemory(2000)).
                    addComponent(CMemory(2000))).
            addComputer(
            CComputer("courses.fit.cvut.cz").
                    addAddress("147.32.232.213").
                    addComponent(CCPU(4, 1600)).
                    addComponent(CMemory(4000)).
                    addComponent(CDisk(CDisk::MAGNETIC, 2000).
                    addPartition(100, "/").
                    addPartition(1900, "/data"))).
            addComputer(
            CComputer("imap.fit.cvut.cz").
                    addAddress("147.32.232.238").
                    addComponent(CCPU(4, 2500)).
                    addAddress("2001:718:2:2901::238").
                    addComponent(CMemory(8000)));

    assert (toString(n) ==
            "Network: FIT network\n"
            "+-Host: progtest.fit.cvut.cz\n"
            "| +-147.32.232.142\n"
            "| +-CPU, 8 cores @ 2400MHz\n"
            "| +-CPU, 8 cores @ 1200MHz\n"
            "| +-HDD, 1500 GiB\n"
            "| | +-[0]: 50 GiB, /\n"
            "| | +-[1]: 5 GiB, /boot\n"
            "| | \\-[2]: 1000 GiB, /var\n"
            "| +-SSD, 60 GiB\n"
            "| | \\-[0]: 60 GiB, /data\n"
            "| +-Memory, 2000 MiB\n"
            "| \\-Memory, 2000 MiB\n"
            "+-Host: courses.fit.cvut.cz\n"
            "| +-147.32.232.213\n"
            "| +-CPU, 4 cores @ 1600MHz\n"
            "| +-Memory, 4000 MiB\n"
            "| \\-HDD, 2000 GiB\n"
            "|   +-[0]: 100 GiB, /\n"
            "|   \\-[1]: 1900 GiB, /data\n"
            "\\-Host: imap.fit.cvut.cz\n"
            "  +-147.32.232.238\n"
            "  +-2001:718:2:2901::238\n"
            "  +-CPU, 4 cores @ 2500MHz\n"
            "  \\-Memory, 8000 MiB\n");


    CNetwork x = n;
    auto c = x.findComputer("imap.fit.cvut.cz");
    assert (toString(*c) ==
            "Host: imap.fit.cvut.cz\n"
            "+-147.32.232.238\n"
            "+-2001:718:2:2901::238\n"
            "+-CPU, 4 cores @ 2500MHz\n"
            "\\-Memory, 8000 MiB\n");


    c->addComponent(CDisk(CDisk::MAGNETIC, 1000).
            addPartition(100, "system").
            addPartition(200, "WWW").
            addPartition(700, "mail"));


    assert (toString(x) ==
            "Network: FIT network\n"
            "+-Host: progtest.fit.cvut.cz\n"
            "| +-147.32.232.142\n"
            "| +-CPU, 8 cores @ 2400MHz\n"
            "| +-CPU, 8 cores @ 1200MHz\n"
            "| +-HDD, 1500 GiB\n"
            "| | +-[0]: 50 GiB, /\n"
            "| | +-[1]: 5 GiB, /boot\n"
            "| | \\-[2]: 1000 GiB, /var\n"
            "| +-SSD, 60 GiB\n"
            "| | \\-[0]: 60 GiB, /data\n"
            "| +-Memory, 2000 MiB\n"
            "| \\-Memory, 2000 MiB\n"
            "+-Host: courses.fit.cvut.cz\n"
            "| +-147.32.232.213\n"
            "| +-CPU, 4 cores @ 1600MHz\n"
            "| +-Memory, 4000 MiB\n"
            "| \\-HDD, 2000 GiB\n"
            "|   +-[0]: 100 GiB, /\n"
            "|   \\-[1]: 1900 GiB, /data\n"
            "\\-Host: imap.fit.cvut.cz\n"
            "  +-147.32.232.238\n"
            "  +-2001:718:2:2901::238\n"
            "  +-CPU, 4 cores @ 2500MHz\n"
            "  +-Memory, 8000 MiB\n"
            "  \\-HDD, 1000 GiB\n"
            "    +-[0]: 100 GiB, system\n"
            "    +-[1]: 200 GiB, WWW\n"
            "    \\-[2]: 700 GiB, mail\n");


    assert (toString(n) ==
            "Network: FIT network\n"
            "+-Host: progtest.fit.cvut.cz\n"
            "| +-147.32.232.142\n"
            "| +-CPU, 8 cores @ 2400MHz\n"
            "| +-CPU, 8 cores @ 1200MHz\n"
            "| +-HDD, 1500 GiB\n"
            "| | +-[0]: 50 GiB, /\n"
            "| | +-[1]: 5 GiB, /boot\n"
            "| | \\-[2]: 1000 GiB, /var\n"
            "| +-SSD, 60 GiB\n"
            "| | \\-[0]: 60 GiB, /data\n"
            "| +-Memory, 2000 MiB\n"
            "| \\-Memory, 2000 MiB\n"
            "+-Host: courses.fit.cvut.cz\n"
            "| +-147.32.232.213\n"
            "| +-CPU, 4 cores @ 1600MHz\n"
            "| +-Memory, 4000 MiB\n"
            "| \\-HDD, 2000 GiB\n"
            "|   +-[0]: 100 GiB, /\n"
            "|   \\-[1]: 1900 GiB, /data\n"
            "\\-Host: imap.fit.cvut.cz\n"
            "  +-147.32.232.238\n"
            "  +-2001:718:2:2901::238\n"
            "  +-CPU, 4 cores @ 2500MHz\n"
            "  \\-Memory, 8000 MiB\n");


    CNetwork Test("Test Network");

    Test.addComputer(
            CComputer("imap.fit.cvut.cz").
                    addAddress("147.32.232.238").
                    addComponent(CCPU(4, 2500)).
                    addAddress("2001:718:2:2901::238").
                    addComponent(CMemory(8000)));

    std::cout << Test<<"\n";

    CComputer testComputer = CComputer("imap.fit.cvut.cz").
            addAddress("147.32.232.238").
            addComponent(CCPU(4, 2500)).
            addAddress("2001:718:2:2901::238").
    addComponent(CMemory(8000)).addComponent(CDisk(CDisk::SSD , 21).addPartition(21 , "manya") .addPartition(21 , "amnya"));

    std::cout << testComputer;


    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
