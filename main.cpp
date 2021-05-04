#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <fstream>
#include <algorithm>

std::string toLowerCase(std::string str)
{
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

template <typename T>
std::string toString (const T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}


struct op_code 
{
    int id;
    std::string name;
    op_code (int id, std::string name) : id(id), name(name) {}; 
    void set (int id, std::string name)  {set (id); set(name);}; 
    void set (int id)  {this->id =id ;};
    void set (std::string name)
    {
        if (!checkName(name)) {this->name = ""; return; }
        this->name = name;
    }
    static bool checkName (std::string name)
    {
        if (name.size() > 0 ) 
        {
            for (int i = 0; i < name.size(); i++)
            {
                if (!isalnum(name[i])) {std::cout << "Wrong database name, please use only characters and digits" << std::endl; return false;}
            }
        }
        return true;
    }
};

class record 
{
    public: 
    int id;
    std::string name;
    std::string chair;
    int semester;

    void setId (int id)
    {
        this->id = id;
    }
    virtual void print() const = 0;
    virtual void add_discipline () = 0;
    virtual void edit_discipline () = 0;
    virtual void del(int indx) = 0;
    virtual std::string serialize () = 0;
    virtual void deserialize (std::string str) =0;
    virtual bool isNegative () = 0;



};

//std::vector <record *> db;

struct discipline
{
    std::string name;
    int mark;
    discipline (): name(""), mark(0)  {};
    discipline (std::string name, int mark) : mark(mark), name(name) {};
    void print ()
    {
        std::cout << "\t\tdiscipline: " << name << " - " << mark << std::endl;
    }
};

class student : public record
{
public: 

    std::vector<discipline> discipline_list;


    student() {id = 0; discipline_list.clear();}
    void print() const override
    {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Chair: " << chair << std::endl;
        std::cout << "Semester: " << semester << std::endl;
        for (auto i : discipline_list)
        {
            std::cout << "\t\tdiscipline: " << i.name << " - " << i.mark << std::endl;

        }
    }

    bool isNegative ()
    {
        for (auto i : discipline_list)
        {
            if (i.mark < 3) {return true;}
        }
        return false;
    }

    void add_discipline() override
    {
        bool isOk = true;
        do {
            discipline ds;
            std::cout << "Enter discipline name, type exit - for break input or leave name blank for skip" << std::endl;
            std::getline (std::cin, ds.name);
            if (ds.name == "exit") {return;}
            if (ds.name.size() == 0) 
            {
                break;
            }
            std::cout << "Enter disciplene mark, type exit or leave in blank for defaul value (2)";
            std::string temp = "";
            std::getline (std::cin, temp);
            if (temp == "exit") {return;}
            try {
                if (temp.size() == 0) {ds.mark = 2;}
                else {ds.mark = stoi(temp);}

                discipline_list.push_back(ds);
                std::cout << "Do you want to enter new discipline? (y - yes, n - no)" << std::endl;
                char ch2;
                bool isOk2 = true;
                do
                {
                    ch2 = std::getchar();
                    std::cin.ignore(1);
                    switch (ch2)
                    {
                    case 'y':
                        isOk = false;
                        break;
                    case 'n':
                        isOk = true;
                        break;
                    
                    default:
                        isOk2 = false;
                        break;
                    }
                } while (!isOk2);
                
            }
            catch (std::exception exc)
            {
                std::cout << "Wrong value" << std::endl;
                isOk = false;
            }
        } while (!isOk);

    } 

    void edit_discipline () override
    {
       char ch;
       bool isOk = false;
       for (int i = 0; i < discipline_list.size(); i++)
       {
            discipline_list[i].print();
            do {
                std::cout << "Do you want edit this discipline? (y - yes, n - no) " << std::endl;
                ch = std::getchar();
                std::cin.ignore(1);

                switch (ch)
                {
                    case 'y':
                        {
                            std::string temp = ""; 
                            std::cout << "Enter NEW discipline name or type exit" << std::endl;
                            std::getline(std::cin, temp);
                            if (temp.substr(0, 4) == "exit") {return;}
                            discipline_list[i].name = temp;
                            temp = "";
                            //------------------  
                            std::cout << "Enter NEW discipline mark or type exit" << std::endl;
                            std::getline(std::cin, temp);
                            if (temp.substr(0, 4) == "exit") {return;}
                            discipline_list[i].mark = stoi(temp);
                            temp = "";
                            //------------------                                    
                            
                            
                            isOk = true;
                            break;
                        }
                    case 'n':
                        {
                            isOk = true;
                            break;
                        }
                    default:
                        isOk = false;
                        break;
                }

            } while (!isOk);
        } 
    }


    void del(int indx) override
    {
        discipline_list.erase(discipline_list.begin() + indx);
    }

    std::string serialize ()  override
    {
        std::string temp = "0";
        temp = temp + "[\""+ toString(id) + "\"" + ",\"" +name+"\"" + ",\"" + chair+ "\"" + ",\"" + toString(semester) + "\""+ "[";
        for (auto i : discipline_list)
        {
            temp = temp + "[\""+ i.name + "\",\"" + toString(i.mark) + "\"],";
        } 
        temp = temp + "]]";
        return temp;
    }

    void findBegEnd (std::string & str, int & beg, int & end)
    {
        int pos = end + 1;
        beg = str.find("\"", pos);
        pos = beg + 1;
        end = str.find("\"", pos); 
    }

    std::string getValue (std::string str, int beg, int end)
    {
        beg++;
        return str.substr(beg, end - beg);
    }

    void deserialize (std::string str) override
    {
        std::string temp = "";
        int pos_b = 0;
        int pos_e = 0;
        int pos = 0;
        int size = str.size();
        discipline_list.clear();
        discipline ds;
        for (int i = 0; i < str.size()-4; i++)
        {
            findBegEnd (str, pos_b, pos_e);
            
            if (pos == 0)         {id = stoi(getValue(str, pos_b, pos_e)); pos++; i = pos_e + 1;}
            else if (pos == 1)    {name = getValue(str, pos_b, pos_e); pos++; i = pos_e + 1;}
            else if (pos == 2)    {chair = getValue(str, pos_b, pos_e); pos++; i = pos_e + 1;}
            else if (pos == 3)    
            {
                semester = stoi(getValue(str, pos_b, pos_e)); 
                pos++; i = pos_e + 1;
            }
            else if (pos == 4)    
            {
                ds.name = getValue(str, pos_b, pos_e); 
                findBegEnd (str, pos_b, pos_e);
                ds.mark = stoi(getValue(str, pos_b, pos_e));   
                i = pos_e + 1; 
                discipline_list.push_back(ds);            
            }

        }
    } 

};

class student2 : public record
{
public: 
    std::vector< std::vector<discipline>> discipline_list;
    student2 () {id = 0; discipline_list.clear();}
    void print() const override
    {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Chair: " << chair << std::endl;
        std::cout << "Semester: " << semester << std::endl;
        int semester_num = 0;
        for (auto semester : discipline_list)
        {
            semester_num++;
            if (semester.size() > 0) 
            {
                std::cout << "Semester number: "<<semester_num << std::endl;
                for (auto i : semester)
                {
                    std::cout << "\t\tdiscipline: " << i.name << " - " << i.mark << std::endl;
                }
            }
        }
    }

    bool isNegative ()
    {
        int semester_num = 0;
        for (auto semester : discipline_list)
        {
            semester_num++;
            if (semester.size() > 0) 
            {
                for (auto i : semester)
                {
                    if (i.mark < 3) {return true;}
                }
            }
        }
        return false;
    }

    void add_discipline() override 
    {
        bool isOk = true;
        do {
            int semester_num = 0;
            std::string temp = "";

            std::cout << "Enter semester number, type exit - for break input or leave name blank for skip" << std::endl;
            std::getline (std::cin, temp);
            if (temp == "exit") {return;}
            try {
                if (temp.size() == 0) {break;}
                else 
                {
                    semester_num = stoi(temp);
                    while (discipline_list.size() < semester_num)
                    {
                        discipline_list.push_back(std::vector<discipline>());
                    }
                }
            }
            catch (std::exception exc)
            {
                std::cout << "Wrong value" << std::endl;
                isOk = false; 
                continue;               
            }

            discipline ds;

            std::cout << "Enter discipline name, type exit - for break input or leave name blank for skip" << std::endl;
            std::getline (std::cin, ds.name);
            if (ds.name == "exit") {return;}
            if (ds.name.size() == 0) 
            {
                break;
            }
            std::cout << "Enter disciplene mark, type exit or leave in blank for defaul value (2)";

            std::getline (std::cin, temp);
            if (temp == "exit") {return;}
            try {
                if (temp.size() == 0) {ds.mark = 2;}
                else {ds.mark = stoi(temp);}

                discipline_list[semester_num-1].push_back(ds);
                std::cout << "Do you want to enter new discipline? (y - yes, n - no)" << std::endl;
                char ch2;
                bool isOk2 = true;
                do
                {
                    ch2 = std::getchar();
                    std::cin.ignore(1);
                    switch (ch2)
                    {
                    case 'y':
                        isOk = false;
                        break;
                    case 'n':
                        isOk = true;
                        break;
                    
                    default:
                        isOk2 = false;
                        break;
                    }
                } while (!isOk2);
                
            }
            catch (std::exception exc)
            {
                std::cout << "Wrong value" << std::endl;
                isOk = false;
            }
            


        } while (!isOk);
    }

    void edit_discipline () override
    {
       char ch;
       bool isOk = false;
       for (int sem = 0; sem < discipline_list.size(); sem++)
       {
            //auto tmp = discipline_list[sem].data();

            for (int i = 0; i < discipline_list[sem].size(); i++)
            {
   
                discipline * ds = discipline_list[sem].data() + i;
                ds->print();
                do {
                    std::cout << "Do you wnat edit this discipline? (y - yes, n - no) " << std::endl;
                    ch = std::getchar();
                    std::cin.ignore(1);

                    switch (ch)
                    {
                        case 'y':
                            {
                                std::string temp = ""; 
                                std::cout << "Enter NEW discipline name or type exit" << std::endl;
                                std::getline(std::cin, temp);
                                if (temp.substr(0, 4) == "exit") {return;}
                                ds->name = temp;
                                temp = "";
                                //------------------  
                                std::cout << "Enter NEW discipline mark or type exit" << std::endl;
                                std::getline(std::cin, temp);
                                if (temp.substr(0, 4) == "exit") {return;}
                                ds->mark = stoi(temp);
                                temp = "";
                                //------------------                                    
                                
                                
                                isOk = true;
                                break;
                            }
                        case 'n':
                            {
                                isOk = true;
                                break;
                            }
                        default:
                            isOk = false;
                            break;
                    }

                } while (!isOk);



            }
            
       } 
    }

    void del(int indx) override
    {
        discipline_list.erase(discipline_list.begin() + indx);
    }

    std::string serialize ()  override
    {
        std::string temp = "1";
        temp = temp + "[\"" + toString(id) + "\"" + ",\"" + name+"\"" + ",\"" + chair+ "\"" + ",\"" + toString(semester) + "\"";
        int semester_num = 0;
        temp = temp + "[";
        for (auto semester : discipline_list)
        {
            semester_num++;
            if (semester.size() > 0)
            {
                temp = temp + "\"" + toString(semester_num) + "\",[";
                for (auto i : semester)
                {
                    
                    temp = temp + "[\""+ i.name + "\",\"" + toString(i.mark) + "\"],";
                }
                temp = temp  + "],";
            }
        } 
        temp = temp + "]]";
        return temp;
    }

    void findBegEnd (std::string & str, int & beg, int & end)
    {
        int pos = end + 1;
        beg = str.find("\"", pos);
        pos = beg + 1;
        end = str.find("\"", pos); 
    }

    std::string getValue (std::string str, int beg, int end)
    {
        beg++;
        return str.substr(beg, end - beg);
    }

    void deserialize (std::string str) override
    {
        std::string temp = "";
        int pos_b = 0, pos_e = 0, pos = 0, semester_num = 0, size = str.size(), o_pos = 0;
        discipline_list.clear();
        discipline ds;
        for (int i = 0; i < str.size()-6; i++)
        {
            
            findBegEnd (str, pos_b, pos_e);
           
            if (pos == 0)         {id = stoi(getValue(str, pos_b, pos_e)); pos++; i = pos_e + 1;}
            else if (pos == 1)    {name = getValue(str, pos_b, pos_e); pos++; i = pos_e + 1;}
            else if (pos == 2)    {chair = getValue(str, pos_b, pos_e); pos++; i = pos_e + 1;}
            else if (pos == 3)    
            {
                semester = stoi(getValue(str, pos_b, pos_e)); 
                pos++; i = pos_e + 1;
            }
            else if (pos == 4)    
            {
                while (i < str.find(",]]", i))
                {
                    
                    semester_num = stoi(getValue(str, pos_b, pos_e)); 
                    while (discipline_list.size() < semester_num)
                    {
                        discipline_list.push_back(std::vector<discipline>());
                    }

                    while (i < str.find("],]", i))
                    {
                        findBegEnd (str, pos_b, pos_e);
                        ds.name = getValue(str, pos_b, pos_e); 
                        findBegEnd (str, pos_b, pos_e);
                        ds.mark = stoi(getValue(str, pos_b, pos_e));   
                        i = pos_e + 1; 
                        discipline_list[semester_num-1].push_back(ds);

                    }
                    i++;
                    o_pos = i; 
                    findBegEnd (str, pos_b, pos_e);
                    if (pos_b == -1) {i = o_pos; break;}
                                        
                }           
            }
        }
    } 
};

std::vector <record *> db;

int getLastDbId (std::vector <record *> db)
{
    int result = 0;
    if (db.size() > 0) 
    {
        result = db.back()->id;
    }
    return result;
}

void saveData (op_code select, std::vector <record *> db) 
{

    auto cur_p =  std::filesystem::current_path();
    std::string file_name = "";
    if (select.name == "") {return;}
    file_name = file_name + cur_p.c_str() + "/" + select.name + ".db";
    std::ofstream fout(file_name);
    if (!fout) 
    {
        std::cout << "Error creating database" << std::endl;
    }
    else
    {
        for ( record * rec : db)
        {
            fout << rec->serialize() << std::endl;

        }
    }
    fout.close();

}

void loadData (op_code select, std::vector <record *> & db) 
{

    auto cur_p =  std::filesystem::current_path();
    std::string file_name = "";
    if (select.name == "") {return;}
    file_name = file_name + cur_p.c_str() + "/" + select.name + ".db";
    std::ifstream fin(file_name);
    if (!fin) 
    {
        std::cout << "Error read database" << std::endl;
    }
    else
    {
        db.clear();
        while (!fin.eof())
        {
            record * tmp = nullptr;
            std::string temp;
            getline(fin, temp);
            int r_type;

            int size = temp.size();
            
            if (size > 0) 
            {
                try 
                {
                    
                    try
                    {
                        r_type = stoi(temp.substr(0,1));
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "Error read some data from batabase file" << std::endl;
                        continue;
                    }                   
                    
                    if (r_type == 0) {tmp = new student();}
                    else if (r_type == 1) {tmp = new student2();}
                    else 
                    {
                        std::cerr << "Error read some data from batabase file" << std::endl;
                        continue;
                    }
                    temp = temp.substr(1,size-1);

                    tmp->deserialize(temp);
                    //mp->setId(getLastDbId(db)+1);
                    db.push_back(tmp);
                }
                catch (std::exception & exc)
                {}
            }
        }
        
    }
    fin.close();

}

std::vector <record *> getStudentsWithNegativeMarks(std::vector <record *> db)
{
    std::vector <record *> new_db;
    for (auto i : db)
    {
        if (i->isNegative()) {new_db.push_back(i);}
    }
    return new_db;
}

int getCountStudentsWithNegativeMarks(std::vector <record *> db)
{
    int result = 0;
    for (auto i : db)
    {
        if (i->isNegative()) {result++;}
    }
    return result;
}


bool compareByName (record * first, record * second)
{
    std::string first_name = toLowerCase(first->name);
    std::string second_name = toLowerCase(second->name);

    return (first_name < second_name);
}

bool compareBySemester (record * first, record * second)
{
    return (first->semester < second->semester);
}

bool compareByChair (record * first, record * second)
{
    return (toLowerCase(first->chair) < toLowerCase(second->chair));
}

std::vector <record *> sortDatabase(std::vector <record *> db, int sortType)
{
    switch (sortType)
    {
        case 1: //by name
        {
            std::sort (db.begin(), db.end(), compareByName);
        }
        break;
        case 2: //by semester
        {
            std::sort (db.begin(), db.end(), compareBySemester);
        }
        break;
        case 3: //by chair
        {
            std::sort (db.begin(), db.end(), compareByChair);
        }
        break;
       
    }
    return db;
}

std::vector <record *> findDatabase(std::vector <record *> db, int findType, std::string param)
{
    std::vector <record *> new_db;
    switch (findType)
    {
        case 1: //by name
        {
            for (auto i : db)
            {
                if (toLowerCase(i->name) == toLowerCase(param)) {new_db.push_back(i);}
            }
            return new_db;
        }
        break;
        case 2: //by semester
        {
            for (auto i : db)
            {
                if (i->semester == stoi(param)) {new_db.push_back(i);}
            }
            return new_db;
        }
        break;
        case 3: //by chair
        {
            for (auto i : db)
            {
                if (toLowerCase(i->chair) == toLowerCase(param)) {new_db.push_back(i);}
            }
            return new_db;
        }
        break;
        case 4: //by id
        {
            for (auto i : db)
            {
                if (i->id == stoi(param)) {new_db.push_back(i);}
            }
            return new_db;
        }
        break;
       
    }
    return new_db;
}

void editRecord(std::vector <record *> db, int editType, std::string param)
{
    record * rec = nullptr;
    int size = db.size();
    switch (editType)
    {
        case 1: //by id
        {
            for (auto i : db)
            {
                if (i->id == std::stoi(param))
                {
                    rec = i;
                    break;
                }
            }
        }
        break;
        case 2: //by name
        {
            for (auto i : db)
            {
                if (toLowerCase(i->name) == toLowerCase(param))
                {
                    rec = i;
                    break;
                }
            }
        }
        break;
    }

    if (rec) 
    {
        rec->print();
        char ch;
        bool isOk = false;
        do {
            std::cout << "Do you wnat edit this record? (y - yes, n - no) " << std::endl;
            ch = std::getchar();
            std::cin.ignore(1);

            switch (ch)
            {
                case 'y':
                    {
                        isOk = true;
                        break;
                    }
                case 'n':
                    {
                        return;
                        break;
                    }
                default:
                    isOk = false;
                    break;
            }

        } while (!isOk);
        std::string temp = "";
        std::cout << "Enter NEW student name or type exit" << std::endl;
        std::getline(std::cin, temp);
        if (temp.substr(0, 4) == "exit") {return;}
        rec->name = temp;
        temp = "";
        //------------------
        std::cout << "Enter NEW student chair or type exit" << std::endl;
        std::getline(std::cin, temp);
        if (temp.substr(0, 4) == "exit") {return;}
        rec->chair = temp;
        temp = "";
        //------------------

        std::cout << "Enter NEW student semester or type exit" << std::endl;
        std::getline(std::cin, temp);
        if (temp.substr(0, 4) == "exit") {return;}
        try {
            rec->semester = stoi (temp);
        }
        catch (std::exception exc)
        {
            rec->semester = 1;
        }
        temp = "";
        //------------------  

        rec->edit_discipline();
    }
    else
    {
        std::cout << "Cannot find data" << std::endl;
    }

}

op_code * readCommand(op_code * result, std::string command)
{
    bool isOk = true;
    int pos = 0;
    int size = command.size();
    std::string name = result->name;


    if (size == 0) {result->set (1); }
    try
    {
        if (command.substr(0, 4) == "exit" ) {result->set(0);}
        else if (command.substr(0, 4) == "help" ) {result->set(1);}
        else if (command.substr(0, 8) == "database") 
        {
            if (command.substr(9, 3) == "new" ) 
            {
                name = command.substr(13, size - 13);
                if (name.size() == 0) {isOk = false;}
                result->set(10, name);
            }
            else if (command.substr(9, 4) == "list") 
            {
                result->set(11);
            }
            else if (command.substr(9, 3) == "del" ) 
            {
                result->set(12, command.substr(13, size - 13));
            }
            else if (command.substr(9, 8) == "selected" ) 
            {
                result->set(14);
            }            
            else if (command.substr(9, 6) == "select" ) 
            {
                name = command.substr(16, size - 16);
                if (name.size() == 0) {isOk = false;}
                result->set(13, name);
                //result->set(13, command.substr(16, size - 16));
            } 
           else if (command.substr(9, 6) == "rename" ) 
            {
                
                if (size > 16) 
                {
                    name = command.substr(16, size - 16); 
                    result->set(15, name);
                }
                else
                {
                    result->set(15);
                }
            }

        }
        else if (command.substr(0, 6) == "insert") 
        {
            result->set(20);
        }
        else if (command.substr(0, 5) == "print") 
        {
            result->set(21);
        }
        else if (command.substr(0, 7) == "save as") 
        {
            std::string old_name = result->name;
            name = command.substr(8, size - 8);
            if (name.size() == 0) {isOk = false; result->set(1, old_name);}
            result->set(23,name);

        }
        else if (command.substr(0, 4) == "save") 
        {
            result->set(22);
        }
        else if (command.substr(0, 7) == "sort by") 
        {
            if (command.substr(8, 4) == "name")
            {
                db = sortDatabase(db, 1);
            }
            else if (command.substr(8, 8) == "semester")
            {
                db = sortDatabase(db, 2);
            }
            else if (command.substr(8, 5) == "chair")
            {
                db = sortDatabase(db, 3);
            }
            
            result->set(22);
        }
        else if (command.substr(0, 7) == "find by") 
        {
            if (command.substr(8, 2) == "id")
            {
                std::string param = command.substr(11, size - 11);
                if (param.size() == 0) {isOk = false;}
                db = findDatabase(db, 4,param);
            }
            else if (command.substr(8, 4) == "name")
            {
                std::string param = command.substr(13, size - 13);
                if (param.size() == 0) {isOk = false;}
                db = findDatabase(db, 1, param);
            }
            else if (command.substr(8, 8) == "semester")
            {
                std::string param = command.substr(17, size - 17);
                if (param.size() == 0) {isOk = false;}
                db = findDatabase(db, 2,param);
            }
            else if (command.substr(8, 5) == "chair")
            {
                std::string param = command.substr(14, size - 14);
                if (param.size() == 0) {isOk = false;}
                db = findDatabase(db, 3, param);
            }
            
            result->set(24);
        }
        else if (command.substr(0, 7) == "edit by") 
        {
            if (command.substr(8, 2) == "id")
            {
                std::string param = command.substr(11, size - 11);
                if (param.size() == 0) {isOk = false;}
                editRecord(db, 1, param);
            }
            else if (command.substr(8, 4) == "name")
            {
                std::string param = command.substr(13, size - 13);
                if (param.size() == 0) {isOk = false;}
                editRecord(db, 2, param);
            }            
            result->set(26);
        }
        else if (command.substr(0, 4) == "save") 
        {
            result->set(22);
        }
        else if (command.substr(0, 13) == "get_bad_count") 
        {
            result->set(25);
        }               
        else
        {
            isOk = false;
            result->set(name);
        }
    }
    catch (const std::out_of_range& err) 
    {
        std::cerr << "Cannot do that:" << err.what() << std::endl;
        
        isOk = false;
    }

    if (!isOk)
    {
        std::cout << "Wrong syntax!" << std::endl;
        result->set(1);
    }
    return result;
}


void printDatabases()
{
    auto cur_p =  std::filesystem::current_path();
    std::string file_ext, file_name;
    //std::cout << "current path:" << cur_p << std::endl;
    for (const auto & files : std::filesystem::directory_iterator(cur_p))
    {
        file_ext = files.path().filename().extension().c_str();
        file_name = files.path().filename().c_str();
        file_name = file_name.substr(0, file_name.size() - 3);
        if (file_ext == ".db")
        {
            std::cout << file_name << "  "; //std::endl; 
        }
    }
    std::cout << std::endl << "Use \"database select [db_name]\" for select database" << std::endl;
}
void createDatabase(op_code select)
{
    auto cur_p =  std::filesystem::current_path();
    std::string file_name = "";
    if (select.name == "") {return;}
    file_name = file_name + cur_p.c_str() + "/" + select.name + ".db";
    std::ofstream ofs(file_name);
    if (!ofs) 
    {
        std::cout << "Error creating database" << std::endl;
    }
    else
    {
        ofs.close();
        std::cout << "Database \'"<< select.name <<"\' create sucsessfuly" << std::endl;
    }
}



void selectDatabase(op_code select)
{
    auto cur_p =  std::filesystem::current_path();
    std::string file_name = "";

    file_name = file_name + cur_p.c_str() + "/" + select.name + ".db";

    if (!std::filesystem::exists(std::filesystem::path(file_name)))
    {
        std::cout << "Database \'"  + select.name + "\' is not exist" << std::endl << "Use: \'database new [db_name]\' to create database" << std::endl;
        db.clear();
        return;
    }

    loadData(select, db);
}

void printSelectedDatabase (op_code select)
{
    if (select.name == "")
    {
        std::cout << "Please select database first" << std::endl;
        return;
    }
    std::cout << "Curent selected database is \'" << select.name << "\'" << std::endl;
}

void deleteDatabase(op_code select)
{
    auto cur_p =  std::filesystem::current_path();
    std::string file_name = "";
    if (select.name == "") {std::cout << "Database name is empty" << std::endl; return;}
    file_name = file_name + cur_p.c_str() + "/" + select.name + ".db";
    uint n = std::filesystem::remove_all(std::filesystem::path( file_name)); 
    if (n==0) 
    {
        std::cout << "Error deleting database" << std::endl;
    }
    else
    {
        std::cout << "Database \'"<< select.name <<"\' delete sucsessfuly" << std::endl;
        select.name = "";
    }
}

void renameDatabase(op_code select)
{
    auto cur_p =  std::filesystem::current_path();
    std::string old_file_name = "", new_file_name = "";
    std::cout << "Enter new database name: " << std::endl;
    std::string new_name = "";
    std::getline(std::cin, new_name);
    if (select.name == "" || !op_code::checkName(new_name)) {return;}

    new_file_name = new_file_name + cur_p.c_str() + "/" + new_name + ".db";
    old_file_name = old_file_name + cur_p.c_str() + "/" + select.name + ".db";
    
    try {
        std::filesystem::rename(std::filesystem::path(old_file_name), std::filesystem::path(new_file_name));
    }
    catch (std::exception & exc)
    {
        std::cout << "Error rename database" << std::endl;
        return;
    }
 
    std::cout << "Database \'"<< select.name <<"\' sucsessfuly renamed to \'" << new_name << "\'" << std::endl;

}


void insertDatabase(op_code select)
{

    char ch;
    bool isOk = true;
    record * new_student;
    do {
        std::cout << "What type of student do you wont do add (1 - general, 2 - individual, 0 - break input) " << std::endl;
        ch = std::getchar();
        std::cin.ignore(1);

        switch (ch)
        {
        case '0':
            {
                return;
            }
        case '1':
            {
                new_student = new student();
                isOk = true;
                break;
            }
        case '2':
            {
                new_student = new student2();
                isOk = true;
                break;
            }
        default:
            isOk = false;
            break;
        }

    } while (!isOk);

                std::string temp = "";
                std::cout << "Enter student name or type exit" << std::endl;
                std::getline(std::cin, new_student->name);
                if (new_student->name.substr(0, 4) == "exit") {return;}
                std::cout << "Enter student chair or type exit" << std::endl;
                std::getline(std::cin, new_student->chair);
                if (new_student->chair.substr(0, 4) == "exit") {return;}

                std::cout << "Enter student semester or type exit" << std::endl;
                std::string tmp;
                std::getline(std::cin, tmp);
                try {
                    new_student->semester = stoi (tmp);
                }
                catch (std::exception exc)
                {
                    new_student->semester = 1;
                }
                if (new_student->chair.substr(0, 4) == "exit") {return;}  

                new_student->add_discipline();
                new_student->setId(getLastDbId(db)+1);
                db.push_back(new_student);    

}

void printData (op_code select) 
{
    for ( record * rec : db)
    {
        rec->print();
    }

}



void mnuMain ()
{
    std::cout << "Input commands or type \"help\" for syntax: " << std::endl;
}
void mnuHelp ()
{
    std::cout << "Command \"exit\" - exit programm" << std::endl;
    std::cout << "database new [db_name] - create database" << std::endl;
    std::cout << "database list - print database list" << std::endl;
    std::cout << "database del [db_name] - delete database" << std::endl;
    std::cout << "database select [db_name] - select database" << std::endl;
    std::cout << "database selected - print selected database name" << std::endl;
    std::cout << "database rename [db_name] - rename database" << std::endl;
    std::cout << "database rename - rename curently selected database" << std::endl << std::endl ;
    
    std::cout << "insert - add sudent to curently selected database" << std::endl;
    std::cout << "print - print curently selected database" << std::endl;
    std::cout << "save - save curently selected database" << std::endl;
    
    

}



int main ()
{
    op_code * select = new op_code(-1,"");
    std::string command;
    while (select->id != 0)
    {    
        mnuMain();

        std::getline(std::cin, command);
    
        select = readCommand(select, command);
        switch (select->id)
        {
            case 0:
                return 0;
            case 1:
                mnuHelp();
                break;
            case 10:
                createDatabase(*select);
                break;
            case 11:
                printDatabases();
                break;
            case 12:
                deleteDatabase(*select);
                break;
            case 13:
                selectDatabase(*select);
                break;
            case 14:
                printSelectedDatabase(*select);
                break;
            case 15:
                renameDatabase(*select);
                break;
            case 20:
                insertDatabase(*select);
                break;
            case 21:
                printData(*select);
                break;
            case 22:
                saveData(*select, db);
                break;
            case 23:
                {
                    createDatabase(*select);
                    saveData(*select, db);
                }
                break;
            case 24:
                {
                    printData(*select);
                }
                break;
            case 25:
                std::cout << "Bad count: " << getCountStudentsWithNegativeMarks(db) << std::endl;
                break;
            case 26:
                //std::cout << "Bad count: " << getCountStudentsWithNegativeMarks(db) << std::endl;
                break;
            default:
                mnuHelp();
                break;

        }
    }
}
