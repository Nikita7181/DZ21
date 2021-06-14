#include "student2.h"

void student2::print() const 
{
    std::cout << "ID: " << getid() << std::endl;
    std::cout << "Name: " << getname() << std::endl;
    std::cout << "Chair: " << getchair() << std::endl;
    std::cout << "Semester: " << getsemester() << std::endl;
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

bool student2::isNegative ()
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

void student2::add_discipline()  
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

void student2::edit_discipline () 
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

void student2::del(int indx) 
{
    discipline_list.erase(discipline_list.begin() + indx);
}

std::string student2::serialize ()  
{
    std::string temp = "1";
    temp = temp + "[\"" + toString(getid()) + "\"" + ",\"" + getname()+"\"" + ",\"" + getchair()+ "\"" + ",\"" + toString(getsemester()) + "\"";
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

void student2::findBegEnd (std::string & str, int & beg, int & end)
{
    int pos = end + 1;
    beg = str.find("\"", pos);
    pos = beg + 1;
    end = str.find("\"", pos); 
}

std::string student2::getValue (std::string str, int beg, int end)
{
    beg++;
    return str.substr(beg, end - beg);
}

void student2::deserialize (std::string str) 
{
    std::string temp = "";
    int pos_b = 0, pos_e = 0, pos = 0, semester_num = 0, size = str.size(), o_pos = 0;
    discipline_list.clear();
    discipline ds;
    for (int i = 0; i < str.size()-6; i++)
    {
        
        findBegEnd (str, pos_b, pos_e);
        
        if (pos == 0)
        {
        try
        {
            setid(stoi (getValue (str, pos_b, pos_e)));
        }
        catch(std::exception &exc)
        {
            std::cout << exc.what() << std::endl;
        }
        pos++;
        i = pos_e + 1;
        }
        else if (pos == 1)    {setname(getValue(str, pos_b, pos_e)); pos++; i = pos_e + 1;}
        else if (pos == 2)    {setchair(getValue(str, pos_b, pos_e)); pos++; i = pos_e + 1;}
        else if (pos == 3)    
        {
            setsemestr(stoi(getValue(str, pos_b, pos_e)));
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
